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
    struct list_elem elem;
    int item;
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
    "list_insert_ordered", "list_unique", "list_max", "list_min"
};

const char *hash_command[] = {
    "hash_insert", "hash_replace", "hash_find", "hash_delete",
    "hash_clear", "hash_size", "hash_empty", "hash_apply"
};

const char *bitmap_command[] = {
    "bitmap_size", "bitmap_set", "bitmap_mark", "bitmap_reset", "bitmap_flip", "bitmap_test",
    "bitmap_set_all", "bitmap_set_multiple", "bitmap_count", "bitmap_contains",
    "bitmap_any", "bitmap_none", "bitmap_all",
    "bitmap_scan", "bitmap_scan_and_flip", "bitmap_dump"
};

/* print function */
void print_list(struct list *list);
void print_hash(struct hash *h);
void print_bitmap(struct bitmap *b);

/* command process function */
void command_create(char *data_structure, char *name);
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
void list_command_delete(char **argument);

/* hash functions */
void hash_commands (char *command, char **argument);
void create_hashtable (struct hash *h);
struct hash_elem *hash_command_insert (struct hash *h, struct hash_item *hash_item);
struct hash_elem *hash_command_replace (struct hash *h, struct hash_item *hash_item);
struct hash_elem *hash_command_find (struct hash *h, struct hash_item *hash_item);
struct hash_elem *hash_command_delete (struct hash *h, struct hash_item *hash_item);
void hash_command_clear (struct hash *h);
size_t hash_command_size (struct hash *h);
bool hash_command_empty (struct hash *h);
void hash_command_apply (struct hash *h, hash_action_func *action);
size_t hash_command_size (struct hash *h);
bool hash_command_empty (struct hash *h);
void hash_command_delete (struct hash *h);

/* bitmap functions */
void bitmap_commands (char *command, char **argument);
void bitmap_command_create (struct bitmap *b);
size_t bitmap_command_size (struct bitmap *b);
void bitmap_command_set (struct bitmap *b, size_t idx, bool value)
void bitmap_command_mark (struct bitmap *b, size_t bit_idx);
void bitmap_command_reset (struct bitmap *b, size_t bit_idx);
void bitmap_command_flip (struct bitmap *b, size_t bit_idx);
bool bitmap_command_test (const struct bitmap *b, size_t bit_idx);
void bitmap_command_set_all (struct bitmap *b, bool value);
void bitmap_command_set_multiple (struct bitmap *b, size_t start, size_t cnt, bool value);
size_t bitmap_command_count (const struct bitmap *b, size_t start, size_t cnt, bool vaiue);
bool bitmap_command_contains (const struct bitmap *b, size_t start, size_t cnt, bool value);
bool bitmap_command_any (const struct bitmap *b, size_t start, size_t cnt);
bool bitmap_command_none (const struct bitmap *b, size_t start, size_t cnt);
bool bitmap_command_all (const struct bitmap *b, size_t start, size_t cnt);
size_t bitmap_command_scan (const struct bitmap *b, size_t start, size_t cnt, bool value);
size_t bitmap_command_scan_and_flip (struct bitmap *b, size_t start, size_t cnt, bool value);
void bitmap_command_dump (const struct bitmap * b);
void hash_command_delete (struct bitmap *b);

#endif
