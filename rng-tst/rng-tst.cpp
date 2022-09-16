// here we go again : This file contains the 'main' function. Program execution begins and ends there.
//

#include <ctime>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <math.h>
//#include <memoryapi.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h> 

int* rng(int num);

int* toaddr(int32_t* arr, int size);

int* neworder(int32_t* arr, int size);

int movethrough(int* arr);

int abs2rel(int32_t* arr, int size);

int movethroughalt(int* arr, int size);

using namespace std::chrono;
using namespace std;

int main(int argc, char* argv[]) {

    int arr_size_init = 1024; //1024*1024;

    int32_t* arrpoint;
    int pow2 = 0;
    int i = 0;
    int exec_time = 0;
    int old_exec_time = 1;
    for (pow2 = 2; pow2 < 11; pow2++) {
        for (i = 0; i < 10; i++) {

            int arr_size = arr_size_init * pow(2, pow2);
            arrpoint = rng(arr_size);

            //printf("el maxxo");

            arrpoint = neworder(arrpoint, arr_size);

            movethrough(arrpoint);

            exec_time += abs2rel(arrpoint, arr_size);
        }
        printf("10 iterations done, for size %i, execution time is %i microseconds on average\n", (int)(arr_size_init*pow(2, pow2)), exec_time/10);
        printf("Difference is %f times\n", exec_time / (float)old_exec_time);
        old_exec_time = exec_time;
    }
    
    /*
    for (i = 0; i < arr_size; i++) {

        printf(" %i", arrpoint[i]);
    }
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
   */

}

int movethroughalt(int* arr, int size) {
    int i = 0;
    int movecount = 0;
    while (i < size) {

        i += 1 + arr[i];
        movecount++;
    }

    return movecount;

}

int abs2rel(int32_t* arr, int size) {
    int count = 0;
    int ix = 0;
    bool isbigger = 0;
    //bool isless = 0;
    for (ix = 0; ix < size; ix++) {

    //    printf("%i. %i\n", ix, arr[ix]);

    }
    //printf("\n");

    for (count = 0; count < size; count++) {
        if (arr[count] == 0) {
            arr[count] = size;
        }

        // translation to relative jumps (as in jupm 2 blocks down)
       //takes "active memory cell" pointer movement into account

        if (arr[count] > count) { isbigger = 1; }
        else { isbigger = 0; }
        arr[count] = abs(arr[count] - count) + 1 - 2*(int)(arr[count] > count);
        if (!isbigger) { arr[count] *= -1; }
        
        

        
    }
    
    //printf("\nNew took %i moves to make it\n", movethroughalt(arr, size));

    for (ix = 0; ix < size; ix++) {

     //   printf("%i. %i\n", ix, arr[ix]);

    }

    //printf("\n");


    LPVOID lpvBase;
    LPTSTR lpPtr;
    BOOL bSuccess;
    DWORD i;
    SYSTEM_INFO sSysInfo;
    
    GetSystemInfo(&sSysInfo);

    //_tprintf(TEXT("This computer has page size %d.\n"), sSysInfo.dwPageSize);

    
    int dwPageSize = sSysInfo.dwPageSize;  // system page size, 4096

    //printf("Size is %i\n", size);

    lpvBase = VirtualAlloc(
        NULL,                 // System selects address
        size*5+1,               // Allocate 5*num + 1 bytes for instructions
        MEM_COMMIT | MEM_RESERVE,          // Magic
        PAGE_EXECUTE_READWRITE); // Mark as readable, writeable and executable

    if (lpvBase == NULL) {
        printf("This thing has failed\n");
    }

    lpPtr = (LPTSTR)lpvBase;

    //printf("Addr is %i\n", (int)lpPtr);

    int ii = 0;
    unsigned int addr = (int)lpPtr;   //lpPtr[1] starts at the next page, 4096 cells down
    unsigned int localaddr = addr;
    unsigned int newaddr;
    //int nextaddr = addr;

    for (ii = 0; ii < size; ii++) {
        localaddr = addr + ii * 5;
        newaddr = arr[ii]*5;                        // 5 bytes per command
        *(unsigned char*)localaddr = 0xE9;
        localaddr++;
        *(unsigned char*)localaddr = (newaddr >> 0); // lower 8 bits
        localaddr++;
        *(unsigned char*)localaddr = (newaddr >> 8); 
        localaddr++;
        *(unsigned char*)localaddr = (newaddr >> 16); 
        localaddr++;
        *(unsigned char*)localaddr = (newaddr >> 24); // upper 8 bits 


        /*

        *(unsigned char*)localaddr = 0xE9;
        localaddr++;
        *(unsigned char*)localaddr = (unsigned char)(newaddr >> 0); // lower 8 bits
        localaddr++;
        *(unsigned char*)localaddr = (unsigned char)(newaddr >> 8);
        localaddr++;
        *(unsigned char*)localaddr = (unsigned char)(newaddr >> 16);
        localaddr++;
        *(unsigned char*)localaddr = (unsigned char)(newaddr >> 24); // upper 8 bits 
        */
        /*
        nextaddr = addr + i * 5 + 1;
        *(unsigned char*)localaddr = 0xE9;
        localaddr = addr + i * 5 + 1; 
        *(unsigned char*)localaddr = 5 ;
        localaddr = addr + i * 5 + 2;
        *(unsigned char*)localaddr = 0;
        localaddr = addr + i * 5 + 3;
        *(unsigned char*)localaddr = 0;
        localaddr = addr + i * 5 + 4;
        *(unsigned char*)localaddr = 0;
        * 
        */
    }

    localaddr++; // = addr + size * ii;
    if (localaddr) {
        *(char*)localaddr = 0xc3;
    }
    //printf("\n Addr = %i\n", addr);
    //printf("\n Localaddr = %i\n", localaddr);



    void (*CallAsFunction) (void);

    //printf("\n%02x\n", &memspace[0]);
    

    CallAsFunction = (void(*)()) lpPtr;

    auto start = high_resolution_clock::now();

    CallAsFunction();

    auto end = high_resolution_clock::now();

    auto diff = duration_cast<microseconds>(end - start);

    //std::cout << "Execution time is " << diff.count() << " microseconds\n";

    //printf("\n");

    //void(*)() exec = memspace;

   // CallAsFunction();

    //printf("%i\n", &assemblycode[0]);

    //CallAsFunction = assemblycode;

    //CallAsFunction();

    bool freesuccess =  VirtualFree(
        lpvBase,
        size*5+1,
        MEM_DECOMMIT|MEM_RELEASE
    );


    //printf("I've done it\n");

    return (int)diff.count();


}


int movethrough(int* arr) {
    int moves = 0;
    int i = 0;
    while (arr[i] != 0) { // let's say when it hits 0, its eqivalent to the pointer pointing to "return" jump

        moves += 1;
        i = arr[i];
        //printf("%i\n", i);
    }
    //printf("took %i moves to move throuhg", moves);

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

    //printf("\n");
    
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




