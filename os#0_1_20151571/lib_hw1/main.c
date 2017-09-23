#include <stdio.h>
#include "command.h"

#define uprint(size) printf("%zu\n", size);

struct list list[11];
struct hash hashtable[11];
struct bitmap bitmap[11];

int main(void){
    char input[256];
    char *token, *type, *ds_name, **argument, *ds;
    char *arg[8];
    int argnum, command_num, len;
    char *sep[] = { ' ', '_'}
    while(1){
        argnum = 0;
        fgets(input,sizeof(input), stdin);
        len = strlen(input); input[len - 1] = '\0';
        token = strtok(input, sep);
        while( token != NULL ){
            arg[argnum++] = token;
            token = strtok(input, sep);
        }
        arg[argnum] = NULL;
        type = arg[0];
        for ( int i = 0; i < 4; ++i ){
            if ( strcmp(type, command[i]) == 0 ){
                command_num = i;
                break;
            }
        }
        switch(command_num){
            case 0: // create
                ds = arg[1]; ds_name = arg[2];
                if ( arg[3] != NULL )
                    command_create(ds, ds_name, atoi(arg[3]));
                else
                    command_create(ds, ds_name, 0);
                break;

            case 1: //delete
                ds_name = arg[1];
                command_delete(ds_name);
                break;
            case 2: //dumpdata
                command_dump(ds_name);
                break;

            case 3: // quit
                exit(EXIT_SUCCESS);
                break;
            default: // command
                command = arg[0]; &argument = arg[1];
                command_argument(command, argument); 
                break;
        }
    } 
    return 0;
}

void
print_list(struct list *list){
    struct list_elem *e;
    struct list_item *item;
    for (e = list_begin(list); e != list_end(list); e = list_next(e)){
        item = list_entry(e, struct list_item, elem);
        printf("%d ", item->data);
    }
    if ( !list_empty(list) ) puts("");
}

void
print_hash(struct hash *h){
    struct hash_iterator i;
    struct hash_item *hash_item;
    hash_first(&i, h);
    while (hash_next (&i)){
        hash_item = hash_entry(hash_cur(&i), struct hash_item, elem);
        printf("%d ", hash_item->data);
    }
    if ( !hash_empty(h) ) printf("\n");
}

void
print_bitmap(struct bitmap *b){

}

void
command_create(char *ds, char *ds_name, int bit_cnt){
    char *bit, *name;
    int num;
    if ( strcmp(ds, "list") == 0 ){
        num = ds_name[4] -'0';
        list_command_create(&list[num]);
    }

    if ( strcmp(ds, "hashtable") == 0 ){
        num = ds_name[4] -'0';
        hash_command_create(&hash[num]);
    }

    if ( strcmp(ds, "bitmap") == 0 ){
        name = strtok(ds_name, " ");
        bit = strtok(NULL, " ");
        bit_cnt = atoi(bit);
        num = name[2] -'0';
        bitmap_command_create(&bitmap[name], bit_cnt);
    }
}

void
command_delete(char *ds_name){
    int len = strlen(ds_name);
    int num = ds_name[len - 1] - '0';
    ds_name[len - 1] = '\0';
    if ( strcmp(ds_name, "list") == 0 ){
        list_command_delete(&list[num]); 
    }

    if ( strcmp(dt, "hashtable") == 0 ){
        hash_command_delete(&hash[num]);
    }


    if ( strcmp(dt, "bitmap") == 0 ){
        bitmap_command_delete(&bitmap[num]);
    }
}

void
command_argument(char *command, char **argument){
    int len = strlen(ds_name);

    if ( strstr(command, "list") != NULL ){
        list_commands(command, argument); 
    }

    if ( strstr(command, "hashtable") != NULL ){
        hash_commands(command, argument);
    }


    if ( strstr(command, "bitmap") != NULL ){
        bitmap_commands(command, argument);
    }
}

void command_dump(char *ds_name){
    int len = strlen(ds_name);
    int num = ds_name[len - 1] - '0';
    if ( strstr(command, "list") != NULL ){
        print_list(&list[num]);
    }

    if ( strstr(command, "hashtable") != NULL ){
        print_hash(&hash[num]);
    }


    if ( strstr(command, "bitmap") != NULL ){
        print_bitmap(&bitmap[num]);
    }
}

void
list_commands (char *command, char **argument){
    size_t command_size = (sizeof list_command) / (sizeof char *);
    int command_num;
    for ( int i = 0; i <  command_size; ++i){
        if ( strcmp(list_command[i], command ) == 0 ){
            command_num = i;
            break;
        }
    }
    switch(command_num){
        case 0:
            list_command_insert(argument);
            break;
        case 1:
            list_command_splice(argument);
            break;
        case 2:
            list_command_push_front(argument);
            break;
        case 3:
            list_commnand_push_back(argument);
            break;
        case 4:
            list_command_push_back(argument);
            break;
        case 5:
            list_command_remove(argument);
            break;
        case 6:
            list_command_pop_front(argument);
            break;
        case 7:
            list_command_pop_back(argument);
            break;
        case 8:
            list_command_front(argument);
            break;
        case 9:
            list_command_back(argument);
            break;
        case 10:
            list_command_size(argument);
            break;
        case 11:
            list_command_empty(argument);
            break;
        case 12:
            list_command_reverse(argument);
            break;
        case 13:
            list_command_sort(argument);
            break;
        case 14:
            list_command_insert_ordered(argument);
            break;
        case 15:
            list_command_unique(argument);
            break;
        case 16:
            list_command_max(argument);
            break;
        case 17:
            list_command_min(argument);
            break;
        case 18:
            list_command_swap(argument);
            break;
        case 19:
            list_command_shuffle(argument);
            break;
        default:
            break;
    }
}

bool
list_less(struct list_elem *list_elem1, struct list_elem *list_elem2){
    struct list_item *list_item1, *list_item2;
    list_item1 = list_entry(list_elem1, struct list_item, elem);
    list_item2 = list_entry(list_elem2, struct list_item, elem);
    return (list_item1->data) < (list_item2->data);
}

struct list *
list_command_create(struct list *list){
    list_init(&list);
}

void
list_command_insert (char **argument){
    char *name = argument[0];
    int pos = atoi(argument[1]);
    int data = atoi(argument[2]);
    int list_num = name[4] - '0';
    struct list *nlist = &list[list_num];
    struct list_elem *list_elem;
    struct list_item *new_list = malloc(sizeof list_item);
    new_list->data = data;
    for ( int i = 0, e = list_begin(nlist); i < pos && e != list_end(nlist); ++i, e = list_next(e) );
    list_insert(e, &(new_list->elem));
}

void
list_command_splice (struct list_item *list_item){
    char *name1, *name2;
    int pos, from, to; 
    int list1_num, list2_num;
    struct list *list1, *list2;
    struct list_elem *before, *first, *last;

    name1 = argument[0]; name2 = argument[2];
    pos = atoi(argument[1]);
    from = atoi(argument[3]); to = atoi(argument[4]);
    list1_num = name1[4] -'0'; list2_num = name2[4] - '0';
    list1 = list[list1_num]; list2 = list[list2_num];

    for ( int i = 0, before = list_begin(list1);  i < pos && before != list_end(list1); ++i, before = list_next(before) );

    for ( int i = 0, first = list_begin(list2);  i < from && first != list_end(list2); ++i, first = list_next(first) );

    for ( int i = 0, last = list_begin(list2);  i < to && last != list_end(list2); ++i, last = list_next(last) );

    list_splice(before, first, last);
}

void
list_command_push_front (char **argument){
    char *name;
    int data, list_num;
    struct list *nlist;
    struct list_item *new_list = malloc(sizeof list_item);

    name = argument[0];
    data = atoi(argument[1]);
    list_num = name[4] - '0';

    nlist = &list[list_num];
    new_list->data = data;

    list_push_front(nlist, &(new_list->elem));
}

void
list_command_push_back (char **argumnet){
    char *name;
    int data, list_num;
    struct list *nlist;
    struct list_item *new_list = malloc(sizeof list_item);

    name = argument[0];
    data = atoi(argument[1]);
    list_num = name[4] - '0';

    nlist = &list[list_num];
    new_list->data = data;

    list_push_back(nlist, &(new_list->elem));
}
void
list_command_remove (char **argument){
    char *name;
    int pos, list_num;
    struct list *nlist;
    struct list_elem *e;
    name = argument[0];
    pos = atoi(argument[1]);
    list_num = name[4] - '0';

    nlist = &list[list_num];
    for ( int i = 0, e = list_begin(nlist); i < pos && e != list_end(nlist); ++i, e = list_next(e));
    list_remove(e);
}

void
list_command_pop_front (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    list_pop_front(nlist);
}

void
list_command_pop_back (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    list_pop_back(nlist);
}

void
list_command_front (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    e = list_front(nlist);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_back (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    e = list_back(nlist);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_size (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    size_t size;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    size = list_size(nlist);
    printf("%zu\n", size);
}

void
list_command_empty (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    size_t size;
    bool is_empty;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    is_empty = list_empty(nlist);
    if ( is_empty ) puts("true");
    else puts("false");
}

void
list_command_reverse (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    list_reverse(nlist);
}

void
list_command_sort (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &list[list_num];
    list_sort(nlist, list_less, NULL );

}

void
list_command_insert_ordered(char **argument){
    char *name;
    int list_num, data;
    struct list *nlist;
    struct list_elem *elem;
    struct list_item *new_list = malloc(sizeof list_item);

    name = argument[0];
    list_num = name[4] - '0';

    data = atoi(argument[1]);
    list_item->data = data;

    nlist = &list[list_num];

    list_insert_ordered(nlist, &(new_list->elem), less, NULL);
}

void
list_command_unique (char **argument){
    char *name1, *name2;
    int list_num1, list_num2;
    struct list *list1, *list2;

    name1 = argument[0]; name2 = argument[1];
    list_num1 = name1[4] - '0';
    list_num2 = name2[4] - '0';
    list1 = &list[list_num1]; list2 = &list[list_num2];
    list_unique(list1, list2, list_less, NULL);
}

void
list_command_max (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';
    nlist = &list[list_num];
    e = list_max(nlist, list_less, NULL);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_min (char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';
    nlist = &list[list_num];
    e = list_min(nlist, list_less, NULL);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_swap(char **argument){
    char *list_name;
    int list_num, first, second;
    struct list *nlist;
    struct list_elem *a, *b;
    name = argument[0];
    list_num = name[4] - '0';
    first = atoi(argument[1]); second = atoi(argument[2]);
    nlist = &list[list_num];
    for ( int i = 0, a = list_begin(nlist); i < first && a != list_end(nlist);
            ++i, a = list_next(a) );
    for ( int i = 0, b = list_begin(nlist); i < first && b != list_end(nlist);
            ++i, b = list_next(b) );
    list_swap(a, b);
}

void
list_command_shuffle(char **argument){
   char *list_name;
   int list_num;
   struct list *l;
   name = argument[0];
   list_num = name[4] - '0';
   l = &list[list_num] ;
   list_shuffle(l);
}

/* hash functions */
void
hash_commands (char *command, char **argument){
    size_t command_size = (sizeof hash_command) / (sizeof char *);
    int command_num;
    for ( int i = 0; i < command_size; ++i){
        if ( strcmp(hash_command[i], command)  == 0){
            command_num = i;
            break;
        }
    }
    switch(command_num){
        case 0:
            hash_command_insert(argument);
            break;
        case 1:
            hash_command_replace(argument);
            break;
        case 2:
            hash_command_find(argument);
            break;
        case 3:
            hash_command_clear(argument);
            break;
        case 4:
            hash_command_delete(argument);
            break;
        case 5:
            hash_command_size(argument);
            break;
        case 6:
            hash_command_empty(argynebt);
            break;
        case 7:
            hash_command_apply(argument);
            break;
        default:
            break;
    }
}

bool
hash_less(struct hash_elem *A, struct hash_elem *B, void *aux){
    struct hash_item *a, *b;
    a = hash_entry(A, struct hash_item, elem);
    b = hash_entry(B, struct hash_item, elem);
    return (a->data) < (b->data);
}

bool
hash_destructor(struct hash_elem *e, void *aux){
    struct hash_item *hash_item;
    hash_item = (e, struct hash_elem, elem );
    free(hash_item);
}

void
hash_square(struct hash_elem *e, void *aux){
    struct hash_item *hash_item;
    int data;
    hash_item = (e, struct hash_elem, elem);
    data = hash_item->data;
    hash_item->data = data * data;
}


void
hash_triple(struct hash_elem *e, void *aux){
    struct hash_item *hash_item;
    int data;
    hash_item = (e, struct hash_elem, elem);
    data = hash_item->data;
    hash_item->data = data * data * data;
}

void
hash_command_create (char *hash_name){
    int hash_num = hash_name[4] -'0';
    hash_init(&hashtable[hash_num], hash_int, hash_less, NULL);
}

void
hash_command_insert (char **argument){
    char *hash_name = argument[0];
    int data = atoi(argument[1]);
    int hash_num = hash_name[4] - '0';
    struct hash_item *hash_item = malloc(sizeof hash_item);
    hash_item->data = data;
    hash_insert(&hashtable[hash_num], &(hash_item->elem)); 
}

void
hash_command_replace (char **argument){
    char *hash_name = argument[0];
    int item = atoi(argument[1]);
    int hash_num = hash_name[4] - '0';
    struct hash_item *hash_item = malloc(sizeof hash_item);
    hash_item->item = item;
    hash_replace(&hashtable[hash_num], &(hash_item->elem));
}

void
hash_command_find (char **argument){
    char *hash_name = argument[0];
    int data = atoi(argument[1]);
    int hash_num = hash_name[4] - '0';
    struct hash_item *hash_item;
    struct hash_elem *e = hash_find(&hashtable[hash_num], &(hash_item->elem));
    hash_item = hash_entry(e, struct hash_item, elem);
    printf("%d\n", hash_item->data);
}

void
hash_command_delete (char **argumnet){
    char *hash_name = argument[0];
    int data = atoi(argument[1]) ;
    int hash_num = hash_name[4] - '0';
    struct hash_item *hash_item;
    struct hash_elem *e = hash_find(&hashtable[hash_num], &(hash_item->elem));
    hash_delete(&hashtable[hash_num], e);
}

void
hash_command_clear (char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    hash_clear(&hashtable[hash_num], hash_destructor); 
}

void
hash_command_size (char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    size_t size = hash_size(&hashtable[hash_num]);
    printf("%zu\n", size);
}

void
hash_command_empty (char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    bool is_empty = hash_empty(&hashtable[hash_num]);
    if ( is_empty ) puts("true");
    else puts("false");
}

void
hash_command_apply (char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    char *hash_func = argument[1];
    if ( strcmp(hash_func, "square") == 0){
        hash_apply(&hashtable[hash_num], hash_square);
    }
    if ( strcmp(hash_func, "triple") == 0){
        hash_apply(&hash_size[hash_num], hash_triple);
    }
}

void
hash_command_size (char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    size_t size;
    size = hash_size(&hashtable[hash_num]);
    printf("%zu\n", size);
}

void
hash_command_empty (char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    bool is_empty;
    is_empty = hash_empty(&hashtable[hash_num]);
}

/* bitmap functions */
void
bitmap_commands (char *command, char **argument){
    size_t command_size = (sizeof bitmap_command) / (sizeof char *);
    int command_num;
    for ( int i = 0; i < command_size; ++i){
        if ( strcmp(bitmap_command[i], command)  == 0){
            command_num = i;
            break;
        }
    }
    switch(command_num){
        case 0:
            bitmap_command_size(argument);
            break;
        case 1:
            bitmap_command_set(argument);
            break;
        case 2:
            bitmap_command_mark(argument);
            break;
        case 3:
            bitmap_command_reset(argument);
            break;
        case 4:
            bitmap_command_flip(argument);
            break;
        case 5:
            bitmap_command_test(argument);
            break;
        case 6:
            bitmap_command_set_all(argument);
            break;
        case 7:
            bitmap_command_set_multiple(argument);
            break;
        case 8:
            bitmap_command_count (argument);
            break;
        case 9:
            bitmap_command_contains (argument);
            break;
        case 10:
            bitmap_command_any (argument);
            break;
        case 11:
            bitmap_command_none (argument);
            break;
        case 12:
            bitmap_command_all (argument);
            break;
        case 13:
            bitmap_command_scan (argument);
            break;
        case 14:
            bitmap_command_scan_and_flip (argument);
            break;
        case 15:
            bitmap_command_dump (argument);
            break;
        case 16:
            bitmap_command_expand (argument);
        default:
            break;
    }
}

void
bitmap_command_create (char **argument){
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    b = bitmap_create(bit_cnt);
}

void
bitmap_command_size (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    size_t size;
    size = bitmap_size(&bitmap[bitmap_num]);
    printf("%zu\n", size);
}

void 
bitmap_command_set (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    int bit_idx = atoi(argument[1]);
    char *bool_value = argument[2];
    bool value;
    value = ( strcmp(bool_value, "true") == 0 ) ? true : false;
    bitmap_set(&bitmap[bitmap_num], bit_idx, value);
}

void 
bitmap_command_mark (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bitmap_mark(b, bit_idx);
}

void 
bitmap_command_reset (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bitmap_reset(b, bit_idx); 
}

void 
bitmap_command_flip (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bitmap_flip(b, bit_idx);
}

void
bitmap_command_test (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bool result;
    result = bitmap_test(b, bit_idx);
    if ( result ) printf("true\n");
    else printf("false\n");
}

void 
bitmap_command_set_all (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    bool value = ( strcmp(argument[1], "true") == 0 ) ? true : false;
    bitmap_set_all(b, value);
}


void 
bitmap_command_set_multiple (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    bitmap_set_multiple(b, start, cnt, value);
}

void
bitmap_command_count (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    size_t value_cnt;
    value_cnt = bitmap_count(b, start, cnt, value);
    printf("%zu\n", value_cnt);
}

void
bitmap_command_contains (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    bool is_contain;
    is_contain = bitmap_contains(b, start, cnt, value);
    if ( is_contain ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_any (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool is_any_contain;
    is_any_contain = bitmap_any(b, start, cnt);
    if ( is_any_contain ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_none (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool is_none;
    is_none = bitmap_none(b, start, cnt);
    if ( is_none ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_all (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool is_all;
    is_all = bitmap_all(b, start, cnt);
    if ( is_all ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_scan (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    size_t bit_idx;
    bit_idx = bitmap_scan(b, start, cnt, value);
    printf("%zu\n", bit_idx);
}

void
bitmap_command_scan_and_flip (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    size_t bit_idx;
    bit_idx = bitmap_scan(b, start, cnt, value);
    printf("%zu\n", bit_idx);
}

void
bitmap_command_dump (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    bitmap_dump(b); 
}

void
bitmap_command_expand (char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = &bitmap[bitmap_num];
    int size = atoi(argument[1]);
    b = bitmap_expand(b, size);
    bitmap[bitmap_num] = b;
}
