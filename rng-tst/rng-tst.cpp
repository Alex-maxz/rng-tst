// here we go again : This file contains the 'main' function. Program execution begins and ends there.
//

#include <ctime>
#include <iostream>
#include <algorithm>
#include <math.h>

int* rng(int num);

int* toaddr(int32_t* arr, int size);

int* neworder(int32_t* arr, int size);

int movethrough(int* arr);

int main(int argc, char* argv[]) {

    int arr_size = 200;

    int32_t* arrpoint;

    arrpoint = rng(arr_size);

    /*
    for (i = 0; i < arr_size; i++) {

        printf(" %i", arrpoint[i]);
    }
    */
    arrpoint = neworder(arrpoint, arr_size);

    int m = movethrough(arrpoint);

    delete[]arrpoint;

   // printf("Took %i moves to make it!\n", m);

    int mulc = 2;

    for (mulc = 2; mulc < 14; mulc++) {

        arr_size = pow(3, mulc);

        printf("Generated %i numbers\n", arr_size);

        arrpoint = rng(arr_size);

        arrpoint = neworder(arrpoint, arr_size);

        int m = movethrough(arrpoint);

        printf("Took %i moves to make it!\n\n", m);



    }









}
int movethrough(int* arr) {
    int moves = 0;
    int i = 0;
    while (arr[i] != 0) { // let's say when it hits 0, its eqivalent to the pointer pointing to "return" jump

        moves += 1;
        i = arr[i];
        //printf("%i\n", i);
    }

    return moves;

}
int* toaddr(int32_t* arr, int size) {
    int i = 0;
    int32_t* tmparr;
    tmparr = new int32_t[size];
    int32_t* tmparr2;
    tmparr2 = new int32_t[size];
    /*
    for (i = 0; i < size; i++) {

        printf("%i\n", arr[i]);
    }
    */
    // array without duplicates
    for (i = 0; i < size; i++) {

        tmparr[i] = i; //probably could use some other method that would be faster
    }

    for (i = 0; i < size; i++) {

        arr[i] = abs(arr[i]) % size; //can be done by dividing by max and multiplying by size
                                    // this way there is no floats and cant hit the 32 bit int limit

    }
    int j = 0;
    for (i = 0; i < size; i++) {

        // target was to avoid duplicates and for the algorithm to be fast
        // thus the actual adress space is filled from the no-duplices array, but the feeding is controlled by the randomly generated sequence
        //seems fast


        if (tmparr[arr[i]] != size + 2 && tmparr[arr[i]] != i) {
            tmparr2[i] = tmparr[arr[i]];
            tmparr[arr[i]] = size + 2;
        }
        else {
            j = size - 1 - arr[i]; //source of slight bias here
            while (tmparr[j] == (int32_t)(size + 2)) {
                j++;
                if (j == size) { j = 0; };
            }
            tmparr2[i] = tmparr[j];
            tmparr[j] = (int32_t)(size + 2);
        }

    }
    delete[]tmparr;
    delete[]arr;
    
    for (i = 0; i < size; i++) {

         printf("%i. %i\n" ,i, tmparr2[i]);

    }

    printf("\n");
    
    return tmparr2;
    
}

int* neworder(int32_t* arr, int size) {

    int i = 0;
    int32_t* tmparr;
    tmparr = new int32_t[size];
    int32_t* tmparr2;
    tmparr2 = new int32_t[size];
    /*
    for (i = 0; i < size; i++) {

        printf("%i\n", arr[i]);
    }
    */
    // array without duplicates
    for (i = 0; i < size; i++) {

        tmparr[i] = i; //probably could use some other method that would be faster
    }

    for (i = 0; i < size; i++) {

        arr[i] = abs(arr[i]) % size; //can be done by dividing by max and multiplying by size
                                    // this way there is no floats and cant hit the 32 bit int limit

    }
    int caddr = 0;
    int j = 0;

    for (i = 0; i < size; i++) {

       // printf("%i. %i\n", i, tmparr[i]);

    }

    //printf("\n");

    for (i = 0; i < size; i++) {

       // printf("%i. %i\n", i, arr[i]);

    }

    //printf("\n");


    //creates path throuh all cells


    for (i = 0; i < size-1; i++) {

        if (tmparr[arr[i]] != 0 && tmparr[arr[i]] != size + 2 && tmparr[arr[i]] != caddr) {
           // printf("Addr @ %i = %i\n", caddr, tmparr[arr[i]]);
            tmparr2[caddr] = tmparr[arr[i]];
            caddr = tmparr[arr[i]];
            tmparr[arr[i]] = size + 2;
        }
        else {
            j = size - 1 - arr[i]; //source of slight bias here
            while (tmparr[j] == (int32_t)(size + 2) || tmparr[j] == caddr || tmparr[j] == 0) {
                j++;
                if (j == size) { j = 0; };
            }
          //  printf("Addr @ %i = %i\n", caddr, tmparr[j]);
            tmparr2[caddr] = tmparr[j];
            caddr = tmparr[j];
            tmparr[j] = (int32_t)(size + 2);
        }

        

        

        // target was to avoid duplicates and for the algorithm to be fast
        // thus the actual adress space is filled from the no-duplices array, but the feeding is controlled by the randomly generated sequence
        //seems fast
        
        

    }
    tmparr2[caddr] = 0;

    for (i = 0; i < size; i++) {

       // printf("%i. %i\n", i, tmparr2[i]);

    }

    //printf("\n");

    return tmparr2;

}

/*

int* rng(int num) {
    int32_t* arrpoint;
    arrpoint = new int32_t[num];
    std::time_t seed = 20; //std::time(0);  //potential issue: time resolution is 1 second
    int i;
    int j;
    for (i = 0; i < num; i++) {
        for (j = 0; j < 32; j++) {

            //didn't quite understand the Fibbonaci part and what exactly is it's pseudo-random output. Bit sequence seems more "bulletproof" to me

            seed = ((((seed >> 31) ^ (seed >> 30) ^ (seed >> 29) ^ (seed >> 27) ^ (seed >> 25) ^ seed) & 0x00000001) << 31) | (seed >> 1);

            // probably could write the whole number and be fine
            // Removed temp. result seems to be the same
            // actually potentially longer loop because time is probably 64bit
            //tmp += seed & 1;

        }
        arrpoint[i] = (int32_t)seed;
    }



    return arrpoint;
}
*/


int* rng(int num) {
    int32_t* arrpoint;
    arrpoint = new int32_t[num];
    std::time_t seed = std::time(0);  //potential issue: time resolution is 1 second
    int gencount = 0;
    int oldseed = seed;
    do {

        seed = ((((seed >> 31) ^ (seed >> 30) ^ (seed >> 29) ^ (seed >> 27) ^ (seed >> 25) ^ seed) & 0x00000001) << 31) | (seed >> 1);

        arrpoint[gencount] = (int32_t)seed;

        gencount += 1;

    } while (seed != oldseed && gencount < num); // will generate until loops over or generated all needed numbers

    return arrpoint;
}




