#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

const int READ_FROM_KEYBORAD = 0;
const int WRITE_TO_CONSOLE = 1;

void syscall_init (void);
void syscall_get_args(void *esp, void *args[], int syscallnum);
/* jimin */
void syscall_halt (void);
void syscall_exit (int status);
pid_t syscall_exec (const char *cmd_line);

/* yonghyuk */
int syscall_wait(pid_t pid);
int syscall_read (int fd, void *buffer, unsigned size);
int syscall_write (int fd, const void *buffer, unsigned size);

#endif /* userprog/syscall.h */
