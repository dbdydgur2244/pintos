#ifndef __LIB_MAIN_H
#define __LIB_MAIN_H

#include <stdio.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "round.h"
#include "debug.h"

struct list_item{
    struct list_elem elem;
    int data;
};

struct hash_item{
    struct hash_elem elem;
    int data;
};

const char *command[] = {
    "create",
    "delete",
    "dumpdata",
    "quit"
};

const char *list_command[] = {
    "list_insert", "list_splice", "list_push_front", "list_push_back",
    "list_remove", "list_pop_front", "list_pop_back", "list_front",
    "list_back", "list_size", "list_empty", "list_reverse", "list_sort",
    "list_insert_ordered", "list_unique", "list_max", "list_min",
    "list_swap", "list_shuffle"
};

const char *hash_command[] = {
    "hash_insert", "hash_replace", "hash_find", "hash_delete",
    "hash_clear", "hash_size", "hash_empty", "hash_apply", 
};

const char *bitmap_command[] = {
    "bitmap_size", "bitmap_set", "bitmap_mark", "bitmap_reset", "bitmap_flip", "bitmap_test",
    "bitmap_set_all", "bitmap_set_multiple", "bitmap_count", "bitmap_contains",
    "bitmap_any", "bitmap_none", "bitmap_all",
    "bitmap_scan", "bitmap_scan_and_flip", "bitmap_dump", "bitmap_expand"
};

/* print function */
void print_list(struct list *list);
void print_hash(struct hash *h);
void print_bitmap(struct bitmap *b);

/* command process function */
void command_create(char *data_structure, char *name, int bit_cnt);
void command_delete(char *ds_name);
void command_argument(char *command, char **argument);
void command_dump(char *ds_name)

/* list functions */
void list_commands (char *command, char **argument);
bool list_less(struct list_elem *list_elem1, struct list_elem *list_elem2);
void list_command_create(struct list *list);
void list_command_insert (char **argument);
void list_command_splice (char **argument);
void list_command_push_front (char **argument);
void list_command_push_back (char **argument);
void list_command_remove (char **argument);
void list_command_pop_front (char **argument);
void list_command_pop_back (char **argument);
void list_command_front (char **argument);
void list_command_back (char **argument);
void list_command_size (char **argument);
void list_command_empty (char **argument);
void list_command_reverse (char **argument);
void list_command_sort (char **argument);
void list_command_insert_ordered(char **argument);
void list_command_unique (char **argument);
void list_command_max (char **argument);
void list_command_min (char **argument);
void list_command_swap(char **argument);
void list_command_shuffle(char **argument);

/* hash command functions */
void hash_commands (char *command, char **argument);

bool hash_less(struct hash_elem *, struct hash_elem *);
void hash_destructor(struct hash_elem *, void *aux);
void hash_square(struct hash_elem *, void *aux);
void hash_triple(struct hash_elem *, void *aux);

void hash_command_create (char *hash_name);
void hash_command_insert (char **argument);
void hash_command_replace (char **argument);
void hash_command_find (char **argument);
void hash_command_delete (char **argument);
void hash_command_clear (char **argument);
void hash_command_size (char **argument);
void hash_command_empty (char **argument);
void hash_command_apply (char **argument);
void hash_command_size (char **argument);
void hash_command_empty (char **argument);

/* bitmap functions */
void bitmap_commands (char *command, char **argument);
void bitmap_command_create (struct bitmap *b, int bit_cnt);
void bitmap_command_size (char **argument);
void bitmap_command_set (char **argument)
void bitmap_command_mark (char **argument);
void bitmap_command_reset (char **argument);
void bitmap_command_flip (char **argument);
void bitmap_command_test (char **argument);
void bitmap_command_set_all (char **argument);
void bitmap_command_set_multiple (char **argument);
void bitmap_command_count (char **argument);
void bitmap_command_contains (char **argument);
bool bitmap_command_any (char **argument);
void bitmap_command_none (char **argument);
void bitmap_command_all (char **argument);
void bitmap_command_scan (char **argument);
void bitmap_command_scan_and_flip (char **argument); 
void bitmap_command_dump (char **argument);
void bitmap_command_expand (char **argument);

#endif
