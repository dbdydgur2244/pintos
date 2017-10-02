#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "../lib/string.h"
#include "../devices/shutdown.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void syscall_get_args(void *esp, void *args[], int syscallnum){
    
    switch(syscallnum){
        case SYS_HALT:
            break;
        case SYS_EXIT:
            args[0] =  (char *)esp + 4;
            break;
        case SYS_EXEC:
            args[0] = (char *)esp + 4;
            break;
        case SYS_WAIT:
            args[0] = (char *)esp + 4;
            break;
        case SYS_READ:
            args[0] = (char *)esp + 4;
            args[1] = (char *)esp + 8;
            args[2] = (char *)esp + 12;
            break;
        case SYS_WRITE:
            args[0] = (char *)esp + 4;
            args[1] = (char *)esp + 8;
            args[2] = (char *)esp + 12;
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
    syscall_get_args()
    syscallnum = *((int *)(f->esp));
    switch(syscallnum){
        /* jimin */
        case SYS_HALT:
            syscall_halt();
            break;
        case SYS_EXIT:
            syscall_exit(*(int *)args[0]);
            break;
        case SYS_EXEC:
            syscall_exec((char *)args[0]);
            break;
        /* yonghyuk */
        case SYS_WAIT:
            syscall_wait(*(pid_t *)args[0]);
            break;
        case SYS_READ:
            sysca_read (*(int *)args[0], args[1] , *(size_t *)args[2]);
            break;
        case SYS_WRITE:
            syscall_write(*(int *)args[0], args[1], *(size_t *)args[2]);
            break;
        default:
            break;
    }
    handle(syscallnum, f);
    thread_exit ();
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
exec (const char *cmd_line){
    
}


/*----------------------------------------------------- */
/* yonghyuk */

/* Waits for a child porcess pid and retrieves the child's exit status.
 * If pid did call exit(), but was terminated, wait(pid) return -1.
 * It must be ensured that Pintos does not terminate until the initial
 * process exits*/
int
syscall_wait (pid_t pid){

}

/* Reads size bytes fro mthe file open as fd into buffer.
 * Return the number of bytes actually read, or -1 if the file could not be read.
 * Fd 0 reads from the keyboard.
 * Actually, we use only fd is 0. */
int
syscall_read (int fd, void *buffer, unsigned size){
    char *buf = (char *)buffer;
    int i;
    if ( fd == READ_FROM_KEYBORAD ){
        for ( i = 0; i < size; ++i )
            buf[i] = input_getc();
        return size;
    }
    return -1;
}

/* Writes ssize bytes from buffer to the open file fd.
 * Returns the number of bytes actually written.
 * Write as many bytes as possible up.
 * Fd 1 writes to the console.
 * Actually, we use only fd is 1. */
int
syscall_write (int fd, const void *buffer, unsigned size){
    if ( fd == WRITE_TO_CONSOLE ) {
        putbuf(buffer, size);
        return size;
    }
    return -1;
}
