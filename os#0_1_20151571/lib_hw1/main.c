#include <stdio.h>
#include "command.h"

struct list list[11];
struct hash hashtable[11];
struct bitmap bitmap[11];
int list_num, hash_num, bitmap_num;

int main(void){
    char input[256];
    char *token, *type, *ds_name, **argument, *ds;
    char *arg[8];
    int arnum, command_num, len;
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
                command_create(ds, ds_name);
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

}

void
print_bitmap(struct bitmap *b){

}

void
command_create(char *ds, char *ds_name){
    char *bit, *name;
    int bit_cnt;
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
        num = name[6] -'0';
        bitmap_command_create(&bitmap[name]);
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
        default:
            list_command_min(argument);
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
    printf("%d\n", size);
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

/* hash functions */
void
hash_commands (char *command, char **argument){

}

struct hash *
hash_command_create (char *hash_name){

}

struct hash_elem *
hash_command_insert (struct hash *h, struct hash_item *hash_item){

}

struct hash_elem *
hash_command_replace (struct hash *h, struct hash_item *hash_item){

}

struct hash_elem *
hash_command_find (struct hash *h, struct hash_item *hash_item){

}
struct hash_elem *
hash_command_delete (struct hash *h, struct hash_item *hash_item){

}

void
hash_command_clear (struct hash *h){

}

size_t
hash_command_size (struct hash *h){

}

bool
hash_command_empty (struct hash *h){

}

void
hash_command_apply (struct hash *h, hash_action_func *action){

}

size_t
hash_size (struct hash *h){

}

bool
hash_empty (struct hash *h){

}

/* bitmap functions */
void
bitmap_commands (char *command, char **argument){

}
struct bitmap *
bitmap_command_create (char* bitmap_name){

}

size_t 
bitmap_command_size (struct bitmap *b){

}

void 
bitmap_command_set (struct bitmap *b, size_t idx, bool value){

}

void 
bitmap_command_mark (struct bitmap *b, size_t bit_idx){

}

void 
bitmap_command_reset (struct bitmap *b, size_t bit_idx){

}

void 
bitmap_command_flip (struct bitmap *b, size_t bit_idx){

}

bool
bitmap_command_test (const struct bitmap *b, size_t bit_idx){

}

void 
bitmap_command_set_all (struct bitmap *b, bool value){

}

void 
bitmap_command_set_multiple (struct bitmap *b, size_t start, size_t cnt, bool value){

}

size_t 
bitmap_command_count (const struct bitmap *b, size_t start, size_t cnt, bool vaiue){

}

bool
bitmap_command_contains (const struct bitmap *b, size_t start, size_t cnt, bool value){

}

bool
bitmap_command_any (const struct bitmap *b, size_t start, size_t cnt){

}

bool
bitmap_command_none (const struct bitmap *b, size_t start, size_t cnt){

}

bool
bitmap_command_all (const struct bitmap *b, size_t start, size_t cnt){

}

size_t
bitmap_command_scan (const struct bitmap *b, size_t start, size_t cnt, bool value){

}

size_t
bitmap_command_scan_and_flip (struct bitmap *b, size_t start, size_t cnt, bool value){


}

void
bitmap_command_dump (const struct bitmap * b){

}
