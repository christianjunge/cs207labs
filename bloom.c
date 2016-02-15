#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bloom.h"


// Multiplicative hash function, Bernstein's function from http://www.strchr.com/hash_functions
index_t hash1(bloom_filter_t* B, key_t k){
    index_t hash = 5381;
    int i;
    for(i = 0; i < k; i++)
        hash = 33 * hash + B->table[i];
    return hash % B->size;
}

// Multiplicative hash function with different constants.  
index_t hash2(bloom_filter_t* B, key_t k){
    index_t hash = 971;
    int i;
    for (i = 0; i< k; i++){
        hash = 17 * hash + (B->table[i]);
    }
    return hash % B->size;
}

int main(){

    //int table[6];
    int test[] = {0, 1, 2, 3, 13, 97};
    //for (int i = 0; i<6; i++){
    //    printf("%i", bob[i]);
    //}

    index_t table[100];
    bloom_filter_t bloom1;
    bloom1.size = 100;
    bloom1.count = 0;
    bloom1.table = table;

    for (int i = 0; i<6; i++){
        printf("%llu", hash2(&bloom1, test[i]));
        printf(" ");
    }

    //printf("%llu", hash1(&bloom1, 1));
    ///bloom1->table = table;
    //bloom1->size = 100;
    //index_t t = 1000;

    //printf("%llu", bloom1->table[0]);



    //printf("%llu", t);
    //printf("%i", 2);

    //printf("Ran");
}