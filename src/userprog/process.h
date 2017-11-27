#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

#define arg_limit 128

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

int parse_filename(char*,char*args[arg_limit]);
void construct_ESP(void **esp,int argnum,char *args[arg_limit]);

#endif /* userprog/process.h */
