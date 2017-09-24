#ifndef __LIB_MAIN_H
#define __LIB_MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"

struct list_item{
    struct list_elem elem;
    int data;
};

struct hash_item{
    struct hash_elem elem;
    int data;
};

struct all_in_one{
    struct list lists[10];
    struct hash hash_table[10];
    struct bitmap *bitmaps[10];
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

/* delete function */

void delete_list(struct list *list);
void delete_hash(struct hash *hash);
void delete_bitmap(struct bitmap **b);
/* command process function */
void command_create(struct all_in_one *, char *data_structure, char *name, int bit_cnt);
void command_delete(struct all_in_one *, char *ds_name);
void command_argument(struct all_in_one *, char *command, char **argument);
void command_dump(struct all_in_one *, char *ds_name);


/* list functions */
void list_commands (struct list lists[10], char *command, char **argument);
bool list_less(struct list_elem *list_elem, struct list_elem *list_elem2);
void list_command_create(struct list *list);
void list_command_insert (struct list lists[10], char **argument);
void list_command_splice (struct list lists[10], char **argument);
void list_command_push_front (struct list lists[10], char **argument);
void list_command_push_back (struct list lists[10], char **argument);
void list_command_remove (struct list lists[10], char **argument);
void list_command_pop_front (struct list lists[10], char **argument);
void list_command_pop_back (struct list lists[10], char **argument);
void list_command_front (struct list lists[10], char **argument);
void list_command_back (struct list lists[10], char **argument);
void list_command_size (struct list lists[10], char **argument);
void list_command_empty (struct list lists[10], char **argument);
void list_command_reverse (struct list lists[10], char **argument);
void list_command_sort (struct list lists[10], char **argument);
void list_command_insert_ordered(struct list lists[10], char **argument);
void list_command_unique (struct list lists[10], char **argument);
void list_command_max (struct list lists[10], char **argument);
void list_command_min (struct list lists[10], char **argument);
void list_command_swap(struct list lists[10], char **argument);
void list_command_shuffle(struct list lists[10], char **argument);

/* hash command functions */
void hash_commands (struct hash hash_table[10], char *command, char **argument);

unsigned hash_func(struct hash_elem *e, void *aux);
bool hash_less(struct hash_elem *, struct hash_elem *, void *aux);
void hash_destructor(struct hash_elem *, void *aux);
void hash_square(struct hash_elem *, void *aux);
void hash_triple(struct hash_elem *, void *aux);

void hash_command_create (struct hash *h);
void hash_command_insert (struct hash hash_table[10], char **argument);
void hash_command_replace (struct hash hash_table[10], char **argument);
void hash_command_find (struct hash hash_table[10], char **argument);
void hash_command_delete (struct hash hash_table[10], char **argument);
void hash_command_clear (struct hash hash_table[10], char **argument);
void hash_command_size (struct hash hash_table[10], char **argument);
void hash_command_empty (struct hash hash_table[10], char **argument);
void hash_command_apply (struct hash hash_table[10], char **argument);

/* bitmap functions */
void bitmap_commands (struct bitmap *bitmaps[10], char *command, char **argument);
void bitmap_command_create (struct bitmap **b, int bit_cnt);
void bitmap_command_size (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_set (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_mark (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_reset (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_flip (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_test (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_set_all (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_set_multiple (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_count (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_contains (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_any (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_none (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_all (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_scan (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_scan_and_flip (struct bitmap *bitmaps[10], char **argument); 
void bitmap_command_dump (struct bitmap *bitmaps[10], char **argument);
void bitmap_command_expand (struct bitmap *bitmaps[10], char **argument);

#endif
