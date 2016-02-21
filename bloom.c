#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bloom.h"

int num_hashes = 3;

// Multiplicative hash function, Bernstein's function from http://www.strchr.com/hash_functions
// index_t hash1(bloom_filter_t* B, key_t k){
//     index_t hash = 5381;
//     int i;
//     for(i = 0; i < k; i++)
//         hash = 33 * hash + B->table[i];
//     return hash;// % B->size;
// }

// Multiplicative hash function with different constants.  
// index_t hash2(bloom_filter_t* B, key_t k){
//     index_t hash = 5383;
//     int i;
//     for (i = 0; i< k; i++){
//         hash = 41 * hash + B->table[i];
//     }
//     return hash;// % B->size;
// }

// from http://burtleburtle.net/bob/hash/integer.html
index_t hash1(bloom_filter_t* B, key_t k)
{
    k = (k+0x479ab41d) + (k<<8);
    k = (k^0xe4aa10ce) ^ (k>>5);
    k = (k+0x9942f0a6) - (k<<14);
    k = (k^0x5aedd67d) ^ (k>>3);
    k = (k+0x17bea992) + (k<<7);
    return k;
}

index_t hash2(bloom_filter_t* B, key_t k) {
    k = ((k >> 32) ^ k) * 0x45d9f3b;
    k = ((k >> 32) ^ k) * 0x45d9f3b;
    k = ((k >> 32) ^ k);
    return k;
}

index_t get_bit(bloom_filter_t *B, index_t i){
    // Pick the index of the integer in the bloom filter's 
    //   table that corresponds to the i'th position.
    int int_index = i >> 6;
    // Pull out that integer, which stores the bit values
    //    of the bloom filter.
    int integer = B->table[int_index];
    // Pick the index of the bit within the integer.
    int bit_index = i%64;
    
    // Jesus Christ this caused a motherfucker of a bug!
    // Shifting the int "1" past 32 places caused some kind of 
    //     silent overflow that fucked everything up.  It caused
    //     bits to be inexplicably set, but somehow didn't change
    //     the underlying table values.  Took me hours to find 
    //     this.  
    index_t one = 1;

    // Value of the bit at that position within the integer.
    index_t bit = (integer & (one<<bit_index)) >> bit_index;
    return bit;
}

void set_bit(bloom_filter_t *B, index_t i){
    int int_index = i >> 6;
    int integer = B->table[int_index];
    int bit_index = i%64;
    index_t one = 1;

    // Use "or" to set the bit whether it is already set or not.
    B->table[int_index] = integer | (one<<bit_index);
}

void bloom_init(bloom_filter_t *B, index_t size_in_bits){
    B->count = 0;
    B->size = size_in_bits;
    // Make a table that holds a sufficient number of integers
    //    to contain all the bits.
    int array_size = (size_in_bits >> 6) + 1;
    index_t *tab = malloc(array_size * sizeof(index_t));
    // Make it empty
    for(int i = 0; i<array_size; i++){
        tab[i]=0;
    }
    B->table = tab; 
}

void bloom_destroy(bloom_filter_t *B){
    // Empty out the bloom filter
    B->count = 0;
    int array_size = (B->size >> 6) + 1;
    index_t *tab = malloc(array_size * sizeof(index_t));
    // Make it empty
    for(int i = 0; i<array_size; i++){
        tab[i]=0;
    }
    B->table = tab; 
}

int bloom_check(bloom_filter_t *B, key_t k){
    int hashed = 0;
    index_t hash;
    for(int i = 0; i< num_hashes; i++){
        hash = (hash1(B, k) + i * hash2(B, k)) % B->size;
        hashed += get_bit(B, hash);
    }
    return hashed;
}


void bloom_add(bloom_filter_t *B, key_t k){
    index_t hash;
    for(int i = 0; i < num_hashes; i++){
        hash = (hash1(B, k) + i * hash2(B, k)) % B->size;
        set_bit(B, hash);
    }
    B->count += num_hashes;
}

// I added this one to count how many bits are set.
index_t numset(bloom_filter_t *B){
    index_t count = 0;
    for(int i = 0; i < B->size; i++){
        count += get_bit(B, i);
    }
    return count;
}

int * rand_generator(){
    static int arr[100];
    for(int i = 0; i<100; i++){
        arr[i] = rand()%1000000;
    }
    return arr;
}

void counter(int *arr1, int *arr2){
    int count = 0;
    bloom_filter_t bloom;
    bloom_init(&bloom, 1000);
    int collisions = 0;

    for(int i=0; i<100; i++){
        bloom_add(&bloom, arr1[i]);
    }
    printf("Number of bits set for first array: ");
    printf("%llu", numset(&bloom));
    printf("\n");

    printf("Number of collisions in second array: ");
    for(int i = 0; i<100; i++){
        
        if (bloom_check(&bloom, arr2[i])==3){
            collisions +=1;
        }
    }
    printf("%i", collisions);
    printf("\n");

}

int main(){

    // bloom_filter_t bloom1;

    for(int i = 1; i<=5; i++){
        num_hashes = i;
        bloom_filter_t bloom1;
        bloom_init(&bloom1, 1000);
        int *a = rand_generator();
        int *b = rand_generator();

        counter(a,b);

    }

    // bloom_init(&bloom1, 1000);

    



    // for(int i =0; i<70; i++){
    //     bloom_add(&bloom1, i);
    // }
    // printf("%llu", bloom1.count);
    // printf("\n");
    // printf("%llu", numset(&bloom1));
    // printf("\n");

    // for(int i = 0; i<1000; i++){
    //     printf("%llu", get_bit(&bloom1, i));
    //     printf(" ");
    // }

    // set_bit(&bloom1, 0);

    

    // printf("%llu", bloom1.table[0]);
    // printf("\n");
    // printf("%llu", numset(&bloom1));
    // printf("\n");
    // set_bit(&bloom1, 3);




    // printf("%llu", bloom1.table[0]);
    // printf("\n");
    // printf("%llu", numset(&bloom1));

    // printf("%llu", numset(&bloom1));
    // printf("%llu", bloom1.count);
    // printf("\n");

    // for(int i =0; i<3; i++){
    //     printf("check");
    //     printf("%i", bloom_check(&bloom1, i));
    //     bloom_add(&bloom1, i);
    //     printf("check after");
    //     printf("%i", bloom_check(&bloom1, i));

    //     printf("\n");
    //     printf("count");
    //     printf("%llu", bloom1.count);
    //     printf("\n");
    //     printf("numset");
    //     printf("%llu", numset(&bloom1));
    //     printf("\n");
    //     printf("\n");
    // }

    // printf("%llu", bloom1.count);
    // printf("\n");

    // printf("%llu", numset(&bloom1));




 
    // printf("%llu", bloom1.table[0]);
    // printf("\n");
    // printf("%llu", bloom1.table[1]);
    // printf("\n");
    // set_bit(&bloom1, 2);
    // printf("%llu", get_bit(&bloom1,2));
    // printf("\n");
    // printf("%llu", bloom1.table[0]);
    // printf("\n");
    // printf("Expecting 4 above");

    // printf("\n");
    // set_bit(&bloom1,3);
    // printf("\n");
    // printf("%llu", bloom1.table[0]);
    // printf("\n");
    // printf("Expecting 12 above");
    
    // printf("\n");
    // printf("%llu", bloom1.table[1]);
    // printf("\n");
    // set_bit(&bloom1, 64);
    // printf("%llu", bloom1.table[1]);
    // printf("\n");
    // printf("Expecting 1 above");
    // set_bit(&bloom1, 66);
    // printf("\n");
    // printf("%llu", bloom1.table[1]);
    // printf("\n");
    // printf("Expecting 5 above");



    // // index_t table[100];
    // bloom_filter_t bloom1;
    // bloom1.size = 100;
    // bloom1.count = 0;
    // bloom1.table = table;



    //int table[6];
    //for (int i = 0; i<6; i++){
    //    printf("%i", bob[i]);
    //}

    // bloom1.table[0]=7;
    // printf("%llu", bloom1.table[0]);
    // printf("\n");
    // printf("%llu", get_bit(&bloom1, 3));
    // printf("\n");
    // set_bit(&bloom1, 3);
    // printf("%llu", get_bit(&bloom1, 3));
    // printf("\n");
    // printf("%llu", bloom1.table[0]);


    // printf("working");





    /////////////// 
    // For getting hashes of sample values
    // int test[] = {0, 1, 2, 3, 13, 97};
    // for (int i = 0; i<6; i++){
    //    printf("%llu", hash2(&bloom1, test[i]));
    //    printf(" ");
    // }
    ///////////////
}