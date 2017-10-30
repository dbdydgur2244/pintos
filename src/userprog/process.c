#include "userprog/process.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "userprog/syscall.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static thread_func start_process NO_RETURN;
static bool load (const char *cmdline, void (**eip) (void), void **esp);

void construct_ESP(void **esp,int argnum,char *args[arg_limit]){
    int i;
    char **arg_addr;
    int first = (int) *esp;

    arg_addr = (char **) malloc ( sizeof (char *) * argnum);
    for ( i = argnum - 1; i >= 0; i-- ) {
        (*esp) -= ( strlen (args[i]) + 1 );
        memcpy (*esp, args[i], strlen (args[i]));
        arg_addr[i] = (char *)(*esp);
    }

    if ( (first - (int) (*esp)) % 4 != 0){
        int j;
        for( j = 0; j < 4 - (first - (int) * esp) % 4;j++) {
            (*esp) -= 1;
            memset (*esp, 0, 1);
        }
    }

    *esp -= 4;
    *( (int *)*esp) = 0;

    for(i = argnum - 1; i >= 0; i--){
        (*esp) -= 4;
        *( (char**) *esp) = arg_addr[i];
    }

    *esp -= 4;
    *( (char**)*esp) = (*esp) + 4;
    *esp -= 4;
    *( (int*)*esp ) = argnum;
    *esp -= 4;
    *( (int*)*esp ) = 0;
    free(arg_addr);
}

/*parsing arguments, new function*/
int parse_filename(char * temp,char * args[arg_limit]){
    char *now;
    char *next;
    int argnum=0;
    
    now = strtok_r (temp," ",&next);
    while(now != NULL){
        args[argnum] = now;
        argnum++;
        now = strtok_r (NULL," ",&next);
    }
    return argnum;
}

/* Starts a new thread running a user program loaded from
   FILENAME.  The new thread may be scheduled (and may even exit)
   before process_execute() returns.  Returns the new process's
   thread id, or TID_ERROR if the thread cannot be created. */
tid_t
process_execute (const char *file_name) 
{
    char *fn_copy;
    tid_t tid;

    /* Make a copy of FILE_NAME.
       Otherwise there's a race between the caller and load(). */
    fn_copy = palloc_get_page (0);
    if (fn_copy == NULL)
        return TID_ERROR;
    strlcpy (fn_copy, file_name, PGSIZE);

    /*
    struct file *f = file_open (file_name);
    if ( f == NULL ){
        return -1;
    }
    else{
        file_close (f);
    }
    */

    /* Create a new thread to execute FILE_NAME. */
    tid = thread_create (file_name, PRI_DEFAULT, start_process, fn_copy);
    if (tid == TID_ERROR)
        palloc_free_page (fn_copy);
    
    /* YH added */
    sema_down(&thread_current()->load);
    return tid;
}

/* A thread function that loads a user process and starts it
   running. */
static void
start_process (void *file_name_)
{
    char *file_name = file_name_;
    struct intr_frame if_;
    bool success;

    /* Initialize interrupt frame and load executable. */
    memset (&if_, 0, sizeof if_);
    if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
    if_.cs = SEL_UCSEG;
    if_.eflags = FLAG_IF | FLAG_MBS;
    /* add */
    struct thread *child_t = thread_current();

    /* YH added for project 2-2 */

    success = load (file_name, &if_.eip, &if_.esp);
    /* If load failed, quit. */
    palloc_free_page (file_name);
    
    /* YH added */
    if (!success) {
        list_remove (&child_t->child_elem);
        /* YH added for project 2-2 */
        struct status_info *s = find_status_by_tid (child_t->parent, child_t->tid);
        s->status = -1;
        //child_t->parent->exit_status = -1;
        sema_up (&child_t->parent->load);
        thread_exit ();
    }
    else {
        sema_up(&child_t->parent->load);
        sema_down(&child_t->exec);
    }
    /* Start the user process by simulating a return from an
       interrupt, implemented by intr_exit (in
       threads/intr-stubs.S).  Because intr_exit takes all of its
       arguments on the stack in the form of a `struct intr_frame',
       we just point the stack pointer (%esp) to our stack frame
       and jump to it. */
    asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (&if_) : "memory");
    NOT_REACHED ();
}

/* Waits for thread TID to die and returns its exit status. If
   it was terminated by the kernel (i.e. killed due to an
   exception), returns -1.  If TID is invalid or if it was not a
   child of the calling process, or if process_wait() has already
   been successfully called for the given TID, returns -1
   immediately, without waiting.

   This function will be implemented in problem 2-2.  For now, it
   does nothing. */
/* YH has modified */
int
process_wait (tid_t child_tid UNUSED) 
{
    struct thread *child_t = find_child_by_tid (thread_current(), child_tid);
    int exit_status = -1;
    if ( child_t != NULL){
        sema_up(&child_t->exec);
        sema_down(&thread_current()->wait);
        //exit_status = thread_current()->exit_status;
        /* YH added for project 2-2 */
        struct status_info *s = find_status_by_tid (thread_current(), child_tid);
        exit_status = s->status;
    }
    return exit_status;
}

/* Free the current process's resources. */
void
process_exit (void)
{
    struct thread *cur = thread_current ();
    uint32_t *pd;
    /* Destroy the current process's page directory and switch back
       to the kernel-only page directory. */
    
    /* YH added for project 2-2*/
    if ( cur->exec_file != NULL ) {
        struct file_info *fi = find_exec_by_file (cur->exec_file);
        list_remove (&fi->elem);
        free (fi); /* It must exist */
    }

    int i;
    for ( i = 2; i < MAX_FILE_NUM; ++i ) {
        if ( cur->file[i] != NULL )
            close(i);
    }

    struct status_info *s;
    struct list_elem *e;
    while ( !list_empty (&cur->status_list)){
        e = list_pop_back (&cur->status_list);
        s = list_entry (e, struct status_info, elem);
        free(s);
    }
    /* ------ */
    
    pd = cur->pagedir;
    if (pd != NULL) 
    {
        /* Correct ordering here is crucial.  We must set
           cur->pagedir to NULL before switching page directories,
           so that a timer interrupt can't switch back to the
           process page directory.  We must activate the base page
           directory before destroying the process's page
           directory, or our active page directory will be one
           that's been freed (and cleared). */
        cur->pagedir = NULL;
        pagedir_activate (NULL);
        pagedir_destroy (pd);
    }
    sema_up(&cur->parent->wait);
}

/* Sets up the CPU for running user code in the current
   thread.
   This function is called on every context switch. */
void
process_activate (void)
{
    struct thread *t = thread_current ();

    /* Activate thread's page tables. */
    pagedir_activate (t->pagedir);

    /* Set thread's kernel stack for use in processing
       interrupts. */
    tss_update ();
}

/* We load ELF binaries.  The following definitions are taken
   from the ELF specification, [ELF1], more-or-less verbatim.  */

/* ELF types.  See [ELF1] 1-2. */
typedef uint32_t Elf32_Word, Elf32_Addr, Elf32_Off;
typedef uint16_t Elf32_Half;

/* For use with ELF types in printf(). */
#define PE32Wx PRIx32   /* Print Elf32_Word in hexadecimal. */
#define PE32Ax PRIx32   /* Print Elf32_Addr in hexadecimal. */
#define PE32Ox PRIx32   /* Print Elf32_Off in hexadecimal. */
#define PE32Hx PRIx16   /* Print Elf32_Half in hexadecimal. */

/* Executable header.  See [ELF1] 1-4 to 1-8.
   This appears at the very beginning of an ELF binary. */
struct Elf32_Ehdr
{
    unsigned char e_ident[16];
    Elf32_Half    e_type;
    Elf32_Half    e_machine;
    Elf32_Word    e_version;
    Elf32_Addr    e_entry;
    Elf32_Off     e_phoff;
    Elf32_Off     e_shoff;
    Elf32_Word    e_flags;
    Elf32_Half    e_ehsize;
    Elf32_Half    e_phentsize;
    Elf32_Half    e_phnum;
    Elf32_Half    e_shentsize;
    Elf32_Half    e_shnum;
    Elf32_Half    e_shstrndx;
};

/* Program header.  See [ELF1] 2-2 to 2-4.
   There are e_phnum of these, starting at file offset e_phoff
   (see [ELF1] 1-6). */
struct Elf32_Phdr
{
    Elf32_Word p_type;
    Elf32_Off  p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
};

/* Values for p_type.  See [ELF1] 2-3. */
#define PT_NULL    0            /* Ignore. */
#define PT_LOAD    1            /* Loadable segment. */
#define PT_DYNAMIC 2            /* Dynamic linking info. */
#define PT_INTERP  3            /* Name of dynamic noader. */
#define PT_NOTE    4            /* Auxiliary info. */
#define PT_SHLIB   5            /* Reserved. */
#define PT_PHDR    6            /* Program header table. */
#define PT_STACK   0x6474e551   /* Stack segment. */

/* Flags for p_flags.  See [ELF3] 2-3 and 2-4. */
#define PF_X 1          /* Executable. */
#define PF_W 2          /* Writable. */
#define PF_R 4          /* Readable. */

static bool setup_stack (void **esp);
static bool validate_segment (const struct Elf32_Phdr *, struct file *);
static bool load_segment (struct file *file, off_t ofs, uint8_t *upage,
        uint32_t read_bytes, uint32_t zero_bytes,
        bool writable);

/* Loads an ELF executable from FILE_NAME into the current thread.
   Stores the executable's entry point into *EIP
   and its initial stack pointer into *ESP.
   Returns true if successful, false otherwise. */
bool
load (const char *file_name, void (**eip) (void), void **esp) 
{
  struct thread *t = thread_current ();
  struct Elf32_Ehdr ehdr;
  struct file *file = NULL;
  off_t file_ofset;
  bool success = false;
  int i;
  int argnum;//number of arguments-for constructing esp
  char *args[arg_limit];//storing arguments-for constructing esp
  char *temp;

  /* Allocate and activate page directory. */
  t->pagedir = pagedir_create ();
  if (t->pagedir == NULL) 
    goto done;
  process_activate ();

    temp = (char *)malloc (sizeof (char) * (strlen (file_name) + 1));
    strlcpy (temp,file_name,strlen(file_name)+1);

    argnum = parse_filename(temp,args);//parsing-Argument Passing

    /* pintos code .... */
    /* Open executable file. */
    file = filesys_open (args[0]);
    if (file == NULL) 
    {
        printf ("load: %s: open failed\n", file_name);
        goto done; 
    }

    /* Read and verify executable header. */
    if (file_read (file, &ehdr, sizeof ehdr) != sizeof ehdr
            || memcmp (ehdr.e_ident, "\177ELF\1\1\1", 7)
            || ehdr.e_type != 2
            || ehdr.e_machine != 3
            || ehdr.e_version != 1
            || ehdr.e_phentsize != sizeof (struct Elf32_Phdr)
            || ehdr.e_phnum > 1024) 
    {
        printf ("load: %s: error loading executable\n", file_name);
        goto done; 
    }

    /* Read program headers. */
    file_ofset = ehdr.e_phoff;
    for (i = 0; i < ehdr.e_phnum; i++) 
    {
        struct Elf32_Phdr phdr;

        if (file_ofset < 0 || file_ofset > file_length (file))
            goto done;
        file_seek (file, file_ofset);

        if (file_read (file, &phdr, sizeof phdr) != sizeof phdr)
            goto done;
        file_ofset += sizeof phdr;
        switch (phdr.p_type) 
        {
            case PT_NULL:
            case PT_NOTE:
            case PT_PHDR:
            case PT_STACK:
            default:
                /* Ignore this segment. */
                break;
            case PT_DYNAMIC:
            case PT_INTERP:
            case PT_SHLIB:
                goto done;
            case PT_LOAD:
                if (validate_segment (&phdr, file)) 
                {
                    bool writable = (phdr.p_flags & PF_W) != 0;
                    uint32_t file_page = phdr.p_offset & ~PGMASK;
                    uint32_t mem_page = phdr.p_vaddr & ~PGMASK;
                    uint32_t page_offset = phdr.p_vaddr & PGMASK;
                    uint32_t read_bytes, zero_bytes;
                    if (phdr.p_filesz > 0)
                    {
                        /* Normal segment.
                           Read initial part from disk and zero the rest. */
                        read_bytes = page_offset + phdr.p_filesz;
                        zero_bytes = (ROUND_UP (page_offset + phdr.p_memsz, PGSIZE)
                                - read_bytes);
                    }
                    else 
                    {
                        /* Entirely zero.
                           Don't read anything from disk. */
                        read_bytes = 0;
                        zero_bytes = ROUND_UP (page_offset + phdr.p_memsz, PGSIZE);
                    }
                    if (!load_segment (file, file_page, (void *) mem_page,
                                read_bytes, zero_bytes, writable))
                        goto done;
                }
                else
                    goto done;
                break;
        }
    }
    /* Set up stack. */
    if (!setup_stack (esp))
        goto done;

    /* Start address. */
    *eip = (void (*) (void)) ehdr.e_entry;
    /* pintos code end */

    construct_ESP (esp, argnum, args);
    //hex_dump (*esp, *esp, (uint32_t)PHYS_BASE - (uint32_t) *esp, true);
    success = true;

done:
    /* We arrive here whether the load is successful or not. */
    file_close (file);
    free (temp); /* YH added */
    return success;
}
/* load() helpers. */

static bool install_page (void *upage, void *kpage, bool writable);

/* Checks whether PHDR describes a valid, loadable segment in
   FILE and returns true if so, false otherwise. */
static bool
validate_segment (const struct Elf32_Phdr *phdr, struct file *file) 
{
    /* p_offset and p_vaddr must have the same page offset. */
    if ((phdr->p_offset & PGMASK) != (phdr->p_vaddr & PGMASK)) 
        return false; 

    /* p_offset must point within FILE. */
    if (phdr->p_offset > (Elf32_Off) file_length (file)) 
        return false;

    /* p_memsz must be at least as big as p_filesz. */
    if (phdr->p_memsz < phdr->p_filesz) 
        return false; 

    /* The segment must not be empty. */
    if (phdr->p_memsz == 0)
        return false;

    /* The virtual memory region must both start and end within the
       user address space range. */
    if (!is_user_vaddr ((void *) phdr->p_vaddr))
        return false;
    if (!is_user_vaddr ((void *) (phdr->p_vaddr + phdr->p_memsz)))
        return false;

    /* The region cannot "wrap around" across the kernel virtual
       address space. */
    if (phdr->p_vaddr + phdr->p_memsz < phdr->p_vaddr)
        return false;

    /* Disallow mapping page 0.
       Not only is it a bad idea to map page 0, but if we allowed
       it then user code that passed a null pointer to system calls
       could quite likely panic the kernel by way of null pointer
       assertions in memcpy(), etc. */
    if (phdr->p_vaddr < PGSIZE)
        return false;

    /* It's okay. */
    return true;
}

/* Loads a segment starting at offset OFS in FILE at address
   UPAGE.  In total, READ_BYTES + ZERO_BYTES bytes of virtual
   memory are initialized, as follows:

   - READ_BYTES bytes at UPAGE must be read from FILE
   starting at offset OFS.

   - ZERO_BYTES bytes at UPAGE + READ_BYTES must be zeroed.

   The pages initialized by this function must be writable by the
   user process if WRITABLE is true, read-only otherwise.

   Return true if successful, false if a memory allocation error
   or disk read error occurs. */
static bool
load_segment (struct file *file, off_t ofs, uint8_t *upage,
        uint32_t read_bytes, uint32_t zero_bytes, bool writable) 
{
    ASSERT ((read_bytes + zero_bytes) % PGSIZE == 0);
    ASSERT (pg_ofs (upage) == 0);
    ASSERT (ofs % PGSIZE == 0);

    file_seek (file, ofs);
    while (read_bytes > 0 || zero_bytes > 0) 
    {
        /* Calculate how to fill this page.
           We will read PAGE_READ_BYTES bytes from FILE
           and zero the final PAGE_ZERO_BYTES bytes. */
        size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
        size_t page_zero_bytes = PGSIZE - page_read_bytes;

        /* Get a page of memory. */
        uint8_t *knpage = palloc_get_page (PAL_USER);
        if (knpage == NULL)
            return false;

        /* Load this page. */
        if (file_read (file, knpage, page_read_bytes) != (int) page_read_bytes)
        {
            palloc_free_page (knpage);
            return false; 
        }
        memset (knpage + page_read_bytes, 0, page_zero_bytes);

        /* Add the page to the process's address space. */
        if (!install_page (upage, knpage, writable)) 
        {
            palloc_free_page (knpage);
            return false; 
        }

        /* Advance. */
        read_bytes -= page_read_bytes;
        zero_bytes -= page_zero_bytes;
        upage += PGSIZE;
    }
    return true;
}

/* Create a minimal stack by mapping a zeroed page at the top of
   user virtual memory. */
static bool
setup_stack (void **esp) 
{
    uint8_t *kpage;
    bool success = false;

    kpage = palloc_get_page (PAL_USER | PAL_ZERO);
    if (kpage != NULL) 
    {
        success = install_page (((uint8_t *) PHYS_BASE) - PGSIZE, kpage, true);
        if (success)
            *esp = PHYS_BASE-12;
        else
            palloc_free_page (kpage);
    }
    return success;
}

/* Adds a mapping from user virtual address UPAGE to kernel
   virtual address KPAGE to the page table.
   If WRITABLE is true, the user process may modify the page;
   otherwise, it is read-only.
   UPAGE must not already be mapped.
   KPAGE should probably be a page obtained from the user pool
   with palloc_get_page().
   Returns true on success, false if UPAGE is already mapped or
   if memory allocation fails. */
static bool
install_page (void *upage, void *kpage, bool writable)
{
    struct thread *th = thread_current ();

    /* Verify that there's not already a page at that virtual
       address, then map our page there. */
    return (pagedir_get_page (th->pagedir, upage) == NULL
            && pagedir_set_page (th->pagedir, upage, kpage, writable));
}
