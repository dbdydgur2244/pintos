#include "userprog/syscall.h"
#include "userprog/process.h"
#include "userprog/exception.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <list.h>
#include <string.h>
#include <console.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "devices/input.h"
#include "devices/shutdown.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
    intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void
syscall_get_args(void *esp, void *args[], int syscallnum){
    if ( syscallnum != SYS_HALT )
        args[0] = (void*)((int *)esp + 1);
    switch (syscallnum) {
        /* project #1 */
        case SYS_HALT:
            break;
        case SYS_EXIT:
            break;
        case SYS_EXEC:
            break;
        case SYS_WAIT:
            break;
        case SYS_CREATE:
            args[1] = (void*)((int *)esp + 2);
            break;
        case SYS_REMOVE:
            break;
        case SYS_OPEN:
            break;
        case SYS_FILESIZE:
            break;
        case SYS_READ:
            args[1] = (void*)((int *)esp + 2);
            args[2] = (void*)((int *)esp + 3);
            break;
        case SYS_WRITE:
            args[1] = (void*)((int *)esp + 2);
            args[2] = (void*)((int *)esp + 3);
            break;
        case SYS_SEEK:
            args[1] = (void*)((int *)esp + 2);
            break;
        case SYS_TELL:
            break;
        case SYS_CLOSE:
            break;
        case SYS_PIBO:
            args[1] = (void*)((int*)esp+2);
            break;
        case SYS_SUM:
            args[1] = (void*)((int*)esp+2);
            args[2] = (void*)((int*)esp+3);
            args[3] = (void*)((int*)esp+4);
            break;
            /* Project 3 and optionally project 4. */
        case SYS_MMAP:
            break;
        case SYS_MUNMAP:
            break;
            /* Project 4 only. */
        case SYS_CHDIR:
            break;
        case SYS_MKDIR:
            break;
        case SYS_READDIR:
            break;
        case SYS_INUMBER:
            break;
        default:
            break;
    }
    int i;
    for ( i = 0; i < 4; ++i ){
        if ( args[i] != NULL ) {
            if ( is_page_fault((void *)*args) )
                exit(-1);
        }
    }
}

/* YH updated..... */
static void
syscall_handler (struct intr_frame *f UNUSED) 
{
    int syscallnum;
    void *args[4] = { NULL, NULL, NULL, NULL };
    syscallnum = *((int *)(f->esp));
    syscall_get_args(f->esp, args, syscallnum);
    switch(syscallnum){
        /* jimin */
        case SYS_HALT:
            halt ();
            break;
        case SYS_EXIT:
            exit ((int)*(int *)args[0]);
            break;
        case SYS_EXEC:
            f->eax = exec ((char *)*(int *)args[0]);
            break;
        case SYS_CREATE:
            if ( is_page_fault ( (char *)*(int *)args[0] ) )
                exit(-1);
            f->eax = create ((char *)*(int *)args[0], (unsigned)*(int *)args[1]);
            break;
        case SYS_REMOVE:
            if ( is_page_fault ( (char *)*(int *)args[0] ) )
                exit(-1);
            f->eax = remove ((char *)*(int *)args[0]);
            break;
        case SYS_OPEN:
            if ( is_page_fault ( (char *)*(int *)args[0] ) )
                exit(-1);
            f->eax = open ((char *)*(int *)args[0]);
            break;
        case SYS_FILESIZE:
            f->eax = filesize ((int)*(int *)args[0]);
            break;
            /* yonghyuk */
        case SYS_WAIT:
            f->eax = wait ((int)*(int *)args[0]);
            break;
        case SYS_READ:
            if(is_page_fault((char*)*(int*)args[1]))
                exit(-1);
            f->eax = read ((int)*(int *)args[0], (void *)*(int *)args[1] , (size_t)*(int *)args[2]);
            break;
        case SYS_WRITE:
            f->eax = write ((int)*(int *)args[0], (void *)*(int *)args[1], (size_t)*(int *)args[2]);
            break;
        case SYS_SEEK:
            seek ((int)*(int *)args[0], (unsigned)*(int *)args[1]);
            break;
        case SYS_TELL:
            f->eax = tell ((int)*(int *)args[0]);
            break;
        case SYS_CLOSE:
            close ((int)*(int *)args[0]);
            break;
        case SYS_PIBO:
            f->eax = pibonacci ((int)*(int *)args[0]);
            break;
        case SYS_SUM:
            f->eax = sum_of_four_integers ((int)*(int *)args[0], (int)*(int *)args[1], (int)*(int *)args[2], (int)*(int *)args[3]);
            break;
            /* Project 3 and optionally project 4. */
        case SYS_MMAP:
            break;
        case SYS_MUNMAP:
            break;
            /* Project 4 only. */
        case SYS_CHDIR:
            break;
        case SYS_MKDIR:
            break;
        case SYS_READDIR:
            break;
        case SYS_INUMBER:
            break;
        default:
            break;
    }
}

/*----------------------------------------------------- */
/* jimin  */

/* Terminates Pintos. */
void
halt (void){
    shutdown_power_off();
}

/* Terminates the current user program, returning status to kenel.
 * If the process's parent waits for it, this status value will be returned.
 * a status of 0 indicates sucess and nonzero values indicate errors. */
void
exit (int status){

    /* YH added
     * This is for close all opened files */
    struct thread *t = thread_current();
    struct thread *parent = t->parent;
    int i;
    for ( i = 0; i < MAX_FILE_NUM; ++i ) {
        if ( t->file[i] != NULL )
            close(i);
    }
    status = (status < -1) ? -1 : status;
    parent->exit_status = status;
    struct thread *tmp = find_child_by_tid(parent, t->tid);
    if ( tmp != NULL )
        list_remove(&tmp->child_elem);

    char *name = malloc (sizeof(strlen(t->name) + 1));
    char *next = NULL, *now = NULL;
    strlcpy (name, t->name, strlen(t->name) + 1);
    now = strtok_r (name, " ", &next);
    printf("%s: exit(%d)\n", now, status);
    free(name);
    
    thread_exit();
}

/* Run tehe executable whose name is givenen in cmd_line, passing any given
 * arguments and returns the new process's program id(pid).
 * Must return pid -1. */
pid_t
exec (const char *cmd_line){
    //if(thread_current()->exit_status==-1) return -1;

    pid_t tid=process_execute(cmd_line);
    if ( thread_current()->exit_status == - 1) return -1;
    if(tid == TID_ERROR) return tid;
    /*do i have something to do..?*/
    /*struct lock file_exe;
     *lock_acquire(&file_exe);
     *tid=process_execute(cmd_line);
     *lock_release(&file_exe);*/
    return tid;
}


/*----------------------------------------------------- */
/* yonghyuk */

/* Waits for a child process pid and retrieves the child's exit status.
 * If pid did call exit(), but was terminated, wait(pid) return -1.
 * It must be ensured that Pintos does not terminate until the initial
 * process exits*/
int
wait (pid_t pid){
   return process_wait (pid); 
}

/* Creates a new file called file initially initial_size bytes in size.
 * Return true if sucessful, false otherwise. */
bool
create (const char *file, unsigned initial_size){
    return filesys_create (file, (int)initial_size);
}

/* Deletes the file called file.
 * Returns true if successful, false otherwise. */
bool
remove (const char *file){
    return filesys_remove (file);
}

/* Opens the file called file.
 * Returns a nonnegative integer handle called a "file descriptor",
 * or -1 if the file could not be opened.
 * Fd 0 and 1 are reserved for the console. fd are not inherited by child process.
 * Each process has an independent set of file descriptors. */
int
open (const char *file){
    struct file *f;
    struct thread *t = thread_current();
    int fd = -1, i; /* if cannot open file or invalid file, then return -1*/ 
    f = filesys_open (file);
    if (f==NULL) return -1;
    for ( i = 2; i < MAX_FILE_NUM; ++i ){
        if ( t->file[i] == NULL){
            t->file[i] = f; fd = i;
            break;
        }
    }
    return fd;
}


/* Returns the size, in bytes, of the file open as fd. */
int
filesize (int fd){
    struct file *f = thread_current()->file[fd];
    if ( f == NULL )
        return -1;
    return (int)file_length (f);
}

/* Reads size bytes from the file open as fd into buffer.
 * Return the number of bytes actually read, or -1 if the file could not be read.
 * Fd 0 reads from the keyboard.
 * Actually, we use only fd is 0. */
#define READ_FROM_KEYBORAD 0
int
read (int fd, void *buffer, unsigned size){
    char *buf = (char *)buffer;
    unsigned i;
    if ( fd == READ_FROM_KEYBORAD ){
        if(is_page_fault(buffer)) return -1;
        for ( i = 0; i < size; ++i )
            buf[i] = (char )input_getc();
        return size;
    }
    else if ( fd < 0 || fd == 1) { } /* error */
    else {
         struct file *f = thread_current()->file[fd];
         if ( f == NULL || is_page_fault (buffer) )
             return -1;
         int readed_bytes = (int)file_read(f, buffer, (int)size);
         return readed_bytes;
    }
    return -1;
}

/* Writes ssize bytes from buffer to the open file fd.
 * Returns the number of bytes actually written.
 * Write as many bytes as possible up.
 * Fd 1 writes to the console.
 * Actually, we use only fd is 1. */
#define WRITE_TO_CONSOLE 1
int
write (int fd, const void *buffer, unsigned size){
    char *buf = (char *)buffer;
    if ( fd == WRITE_TO_CONSOLE ) {
        putbuf(buf, size);
        return size;
    }
    else if ( fd <= 0 ) {} /* error */
    else {
        struct file *f = thread_current()->file[fd];
        if ( f == NULL || is_page_fault (buffer) )
            return -1;
        int bytes_written = (int)file_write (f, buffer, size);
        file_deny_write (f);
        return bytes_written;
    }
    return -1;
}

void
seek (int fd, unsigned positioin){
    struct file *f = thread_current()->file[fd];
    if ( f == NULL ) {
        exit(-1);
    }
    file_seek (f, (int)positioin );
}

unsigned
tell ( int fd ){
    struct file *f = thread_current()->file[fd];
    if ( f == NULL ) {
        exit(-1);
    }
    return (unsigned) file_tell (f);
}

/* not finished */
void
close ( int fd ){
    struct file *f = thread_current()->file[fd];
    if ( f == NULL ) {
        exit (-1);
    }
    file_close (f);
    thread_current()->file[fd] = NULL;
    //list_remove (f);
}

int pibonacci (int n){
    int i=1,k;
    int temp1=1,temp2;
    if(n<3) return 1;
    for(k=3;k<=n;k++){
        temp2=i+temp1;
        i=temp1;
        temp1=temp2;
    }
    return temp2;
}

int sum_of_four_integers ( int a, int b, int c, int d ) {
    return a + b + c + d;
}
