
//your code here

#include <stdlib.h>
#include <stdio.h>

typedef struct item {
    int value;
    struct item* rest;
} Item;

Item* new_item(int value){
    Item* newitem = (Item *) malloc(sizeof(Item));
    newitem->value = value;
    newitem->rest = NULL;
    return newitem;
}

Item* set_item(Item* listptr, Item* rest){
    /* This is the skeleton of an implementation; it doesn't do anything meaningful yet.*/
    listptr->rest = rest;
    return listptr;
}

Item* insert_front(Item* listptr, int value){
    Item* newitem = new_item(value);
    newitem->rest = listptr;
    return newitem;
}

Item* insert_back(Item* listptr, int value){
    Item* p = listptr;
    while(p->rest !=NULL){
        p = p->rest;
    }
    p->rest = new_item(value);
    return listptr;
}

int get(Item* listptr, int index){
    int ctr = 0;
    Item* p;
    for(p = listptr; p!= NULL; p = p->rest){
        if (ctr==index){
            return p->value;
        }
        ctr++;
    }
    return -1;
}

int get_index(Item* listptr, int value){
    /*returns the index at which the value is found, and otherwise -1.*/
    Item* p;
    int ctr = 0;
    Item* next;
    for(p = listptr; p!= NULL; p = next){
        if (p->value == value){
            return ctr;
        }
        next = p->rest;
        ctr++;
    }
    return -1;
}

Item* remove_item(Item* listptr, int value){
    /*Be careful to return NULL if the item with value was not in the list, and make sure that deletion 
    repoints the pointer of the previous item to the next item.*/
    Item* p;
    Item* newItem = new_item(0);
    int found = 0;
     
    for(p = listptr; p!= NULL; p = p->rest){
        while (p->value == value){
            p = p->rest;
            found = 1;
        }
        if (newItem->value == 0){
            newItem = new_item(p->value);
        } 
        else{
            newItem = insert_back(newItem,p->value);
        }
        
    }
    
    if (found==1){
        return newItem;
    } 
    else {
        return NULL;
    }
}
   
void free_all(Item* listptr) {
    Item *p;
    Item *next;
    for(p = listptr; p!= NULL; p = next){
        next = p->rest;
        free(p);
    }
}

int main(){
    Item* listptr;
    int i;
    listptr = new_item(0);
    for (i=1; i < 6; i++){
        listptr=insert_front(listptr, i);
    }
    for (i=0; i < 6; i++){
        printf("i %d Item%d\n", i, get(listptr, i));
    }
    listptr = remove_item(listptr, 3);
    for (i=0; i <= 5; i++){
        printf("i %d Item %d\n", i, get(listptr, i));
    }
    printf("Index for 3 %d\n", get_index(listptr, 3));
    free_all(listptr);
}