#include "command.h"

void command_process(char *arg, int argnum, int index){
    switch(i){
        case 0:
            create_struct(&arg[1], argnum);
            break;
        case 1:
            break;

        case 2:
            break;
        case 3:
            break;

        case 4:
            break;
        default:
            break;
    }
}

void create_struct(char *arg, int argnum){
    if ( strcmp(arg[0], structrue[0]) == 0 ){
        create_list(arg[1]);
    }
    else if ( strcmp(arg[0], structrue[1]) == 0 ) {
        create_hashtable(arg[1]);
    }
    else{
        create_bitmap(arg[1]);
    }
}

void command_list(char *arg, int argnum){
    
}

void command_hash(char *arg, int argnum){

}

void command_bitmap(char *argm, int argnum){

}

void create_list(char *list_name){
    list = malloc(sizeof(struct list));
    list_init(list);
}

void create_hashtable(char *hashtable_name){

}

void create_bitmap(char *bitmap_name){

}
