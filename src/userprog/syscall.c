#include "userprog/syscall.h"
#include "userprog/process.h"
#include "userprog/exception.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <list.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
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
        args[0] = (int *)esp + 1;
    switch(syscallnum){
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
            args[1] = (int *)esp + 2;
            break;
        case SYS_REMOVE:
            break;
        case SYS_OPEN:
            break;
        case SYS_FILESIZE:
            break;
        case SYS_READ:
            args[1] = (int *)esp + 2;
            args[2] = (int *)esp + 3;
            break;
        case SYS_WRITE:
            args[1] = (int *)esp + 2;
            args[2] = (int *)esp + 3;
            break;
        case SYS_SEEK:
            args[1] = (int *)esp + 2;
            break;
        case SYS_TELL:
            break;
        case SYS_CLOSE:
            break;
            /* Project 3 and optionally project 4. */
        case SYS_MMAP:
            args[1] = (int *)esp + 2;
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

/* YH updated..... */
static void
syscall_handler (struct intr_frame *f UNUSED) 
{
    int syscallnum;
    void *args[4];
    printf ("system call!\n");
    syscallnum = *((int *)(f->esp));
    syscall_get_args(f->esp, args, syscallnum);
    switch(syscallnum){
        /* jimin */
        case SYS_HALT:
            syscall_halt();
            break;
        case SYS_EXIT:
            syscall_exit((int)*(int *)args[0]);
            break;
        case SYS_EXEC:
            syscall_exec((char *)*(int *)args[0]);
            break;
        case SYS_CREATE:
            break;
        case SYS_REMOVE:
            break;
        case SYS_OPEN:
            break;
        case SYS_FILESIZE:
            break;
            /* yonghyuk */
        case SYS_WAIT:
            syscall_wait((int)*(int *)args[0]);
            break;
        case SYS_READ:
            syscall_read ((int)*(int *)args[0], (void *)*(int *)args[1] , (size_t)*(int *)args[2]);
            break;
        case SYS_WRITE:
            syscall_write((int)*(int *)args[0], (void *)(int *)args[1], (size_t)*(int *)args[2]);
            break;
        case SYS_SEEK:
            args[1] = (int *)esp + 2;
            break;
        case SYS_TELL:
            break;
        case SYS_CLOSE:
            break;
            /* Project 3 and optionally project 4. */
        case SYS_MMAP:
            args[1] = (int *)esp + 2;
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
syscall_halt (void){
    shutdown_power_off();
}

/* Terminates the current user program, returning status to kenel.
 * If the process's parent waits for it, this status value will be returned.
 * a status of 0 indicates sucess and nonzero values indicate errors. */
void
syscall_exit (int status){

}

/* Run tehe executable whose name is givenen in cmd_line, passing any given
 * arguments and returns the new process's program id(pid).
 * Must return pid -1. */
pid_t
syscall_exec (const char *cmd_line){

}


/*----------------------------------------------------- */
/* yonghyuk */

/* Find file by fd(file descripter) */
struct file_list *
find_file_by_fd(int fd){
    struct thread *t;
    struct list *list;
    struct list_elem *e;

    t = thread_current();
    list = t->file_list;
    for ( e = list_begin(&list); e != list_end(&list); e != list_next(&list)){
        struct file_list f = list_entry(e, struct file_list, file_elem);
        if ( f.fd == fd ){
            return f;
        }
    }
    return NULL;
}



/* Waits for a child process pid and retrieves the child's exit status.
 * If pid did call exit(), but was terminated, wait(pid) return -1.
 * It must be ensured that Pintos does not terminate until the initial
 * process exits*/
int
syscall_wait (pid_t pid){
   process_wait (pid); 
}

bool
syscall_create (const char *file, unsigned initial_size){
    struct thread *t;
    
}

bool
syscall_remove (const char *file){

}

int
syscall_open (const char *file){

}

int
syscall_filesize (int fd){
    struct file_list *fl = find_file_by_fd (fd);
    if ( fl == NULL )
        return -1;
    return (int)file_length (&(fl->file));
}

/* Reads size bytes from the file open as fd into buffer.
 * Return the number of bytes actually read, or -1 if the file could not be read.
 * Fd 0 reads from the keyboard.
 * Actually, we use only fd is 0. */
#define READ_FROM_KEYBORAD 0
int
syscall_read (int fd, void *buffer, unsigned size){
    char *buf = (char *)buffer;
    int i;
    if ( fd == READ_FROM_KEYBORAD ){
        for ( i = 0; i < size; ++i )
            buf[i] = input_getc();
        return size;
    }
    else if ( fd < 0 || fd == 1) { }
    else {
         struct file_list *fl = find_file_by_fd(fd);
         if ( fl == NULL )
             return -1;
         int readed_bytes = (int)file_read(&(fl->file), buffer, (int)size);
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
syscall_write (int fd, const void *buffer, unsigned size){
    if ( fd == WRITE_TO_CONSOLE ) {
        putbuf(buffer, size);
        return size;
    }
    else if ( fd <= 0 ) {}
    else {
        struct file_list *fl = find_file_by_fd (fd);
        if ( fl == NULL )
            return -1;
        int bytes_written = (int)file_write (&(fl->file), buffer, size);
        file_deny_write ( &(f1->file) );
        return bytes_written;
    }
    return -1;
}

void
syscall_seek (int fd, unsigned positioin){
    struct file_list *fl = find_file_by_fd (fd);
    file_seek ( &(fl->file), (int)positioin );
}

unsigned
syscall_tell ( int fd ){
    struct file_list *fl = find_file_by_fd (fd);
    return (unsigned) file_tell ( &(fl->file) );

}

/* not finished */
void syscall_close ( int fd ){
   struct file_list *f1 = find_file_by_fd (fd);
   file_close ( &(fl->file) );
   list_remove ( &(fl->file_elem) );
}

