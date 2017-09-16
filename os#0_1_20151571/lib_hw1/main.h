#include <stdio.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "round.h"
#include "debug.h"

struct bitmap a;
struct list b;
struct hash h;

const char *command[] = {
    "create",
    "delete",
    "command",
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

