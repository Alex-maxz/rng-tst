// here we go again : This file contains the 'main' function. Program execution begins and ends there.
//

#include <ctime>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <math.h>
//#include <memoryapi.h>
#include <windows.h>
//#include <tchar.h>
#include <stdio.h>
#include <stdlib.h> 

int* rng(int num);

int* toaddr(int32_t* arr, int size);

int* neworder(int32_t* arr, int size);

int checkassembly(LPBYTE startpointer, int size);

int movethrough(int* arr);

int assembly_write_exec(int32_t* arr, int size);

int movethroughalt(int* arr, int size);

using namespace std::chrono;
using namespace std;

int main(int argc, char* argv[]) {

    int arr_size_init = 1024; //1024*1024;


   
    int pow2 = 0;
    int i = 0;
    int exec_time = 0;
    int old_exec_time = 1;
    for (pow2 = 2; pow2 < 11; pow2++) {
        for (i = 0; i < 10; i++) {

            int32_t* arrpoint;
            

            int arr_size = arr_size_init * (int)(pow(2, pow2));

            //arr_size = 1024*1024;

            arrpoint = rng(arr_size);

            //printf("el maxxo");

            arrpoint = neworder(arrpoint, arr_size);

            movethrough(arrpoint);

            exec_time += assembly_write_exec(arrpoint, arr_size);

            delete[]arrpoint;
        }
        printf("10 iterations done, for size %i, execution time is %i microseconds on average\n", (int)(arr_size_init*pow(2, pow2)), exec_time/10);
        printf("Difference is %f times\n", exec_time / (float)old_exec_time);
        old_exec_time = exec_time;
    }
    
  

}

int movethroughalt(int* arr, int size) {

    // moves through adress numbers
    // checks for adress generation correctness

    int i = 0;
    int movecount = 0;
    while (i < size) {

        i += 1 + arr[i];
        movecount++;
    }

    return movecount;

}

int checkassebmly(LPBYTE startpointer, int size) {

    //very simple assembly emulator
    //returns completed instructions count

    LPBYTE localpointer = startpointer;
    int instruction_counter = 0;

    int current_instruction = 90; // noop, 0xE9 = 233 = jmp, 0xc3 = 195 = ret
    int32_t current_adress = 0;

    while ((int)*localpointer != 195) {
        
        current_instruction = (int)*localpointer;
        current_adress += (int)localpointer[4];
        current_adress = current_adress << 8;
        current_adress += (int)(localpointer[3]);
        current_adress = current_adress << 8;
        current_adress += (int)(localpointer[2]);
        current_adress = current_adress << 8;
        current_adress += (int)(localpointer[1]);
        if (current_instruction == 0xe9) {
            //printf("%i. Jmp %i\n", instruction_counter, current_adress/5);
            localpointer += 5;
            localpointer += current_adress;
            current_adress = 0;
            //printf("New adress is %llu\n", (long long int)localpointer);
        }
        else if (current_instruction != 195){
            throw std::invalid_argument("Written instruction was wrong");
        }

        instruction_counter++;

    }

    return instruction_counter;

}

int assembly_write_exec(int32_t* arr, int size) {

    //main function
    //trtansforms absolute cell numbers to relative jumps 
    //generates assembly code in memory and calls for it's execution
    //returns assemly exectuion time

    int completion_time = 0;
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

       //printf("%i. %i\n", ix, arr[ix]);

    }

    //printf("\n");


    LPVOID lpvBase;
    SYSTEM_INFO sSysInfo;
    
    GetSystemInfo(&sSysInfo);

    //_tprintf(TEXT("This computer has page size %d.\n"), sSysInfo.dwPageSize);

    
    int dwPageSize = sSysInfo.dwPageSize;  // system page size, 4096
    
    long long int memsize = size; 
    memsize = memsize * 5 + 1;

    //printf("Size is %i\n", size);

    lpvBase = VirtualAlloc(
        NULL,                 // System selects address
        (SIZE_T)memsize,               // Allocate 5*num + 1 bytes for instructions
        MEM_COMMIT | MEM_RESERVE,          // Magic
        PAGE_EXECUTE_READWRITE); // Mark as readable, writeable and executable

    if (lpvBase == NULL) {
        throw std::invalid_argument("Was unable to allocate memory");
    }
    else {
        //std::cout << "LpvBase is " << lpvBase << "\n";

        LPBYTE localaddr = (LPBYTE)lpvBase;

            

        int ii = 0;
        printf("Addr is %llu\n", (long long int)localaddr);
        int newaddr;
        //int nextaddr = addr;

        for (ii = 0; ii < size; ii++) {
            newaddr = arr[ii] * 5;                        // 5 bytes per command
            *localaddr = 0xE9;
            localaddr++;
            *localaddr = (newaddr >> 0); // lower 8 bits
            localaddr++;
            *localaddr = (newaddr >> 8);
            localaddr++;
            *localaddr = (newaddr >> 16);
            localaddr++;
            *localaddr = (newaddr >> 24); // upper 8 bits 
            localaddr++;


        }
        // = addr + size * ii;
        //printf("Localaddr is %p\n", localaddr);
        if (localaddr) {
            *localaddr = 0xc3;
        }

        int instruction_count = checkassebmly((LPBYTE)lpvBase, size);
        printf("Instruction_count is %i\n", instruction_count);
        //printf("\n Addr = %i\n", addr);
        //printf("\n Localaddr = %i\n", localaddr);



        void (*CallAsFunction) (void);

        //printf("\n%02x\n", &memspace[0]);


        CallAsFunction = (void(*)()) lpvBase;

        auto start = high_resolution_clock::now();

        CallAsFunction();

        auto end = high_resolution_clock::now();

        auto diff = duration_cast<microseconds>(end - start);

        completion_time = (int)diff.count();
        //std::cout << "Execution time is " << diff.count() << " microseconds\n";
    }

    bool freesuccess =  VirtualFree(
        lpvBase,
        0, //size*5+1,
        MEM_RELEASE
    );


    if (!freesuccess) {
        throw std::invalid_argument("Was unable to free memory");
    }


    //printf("I've done it\n");

    return completion_time;


}


int movethrough(int* arr) {

    //moves through array of relative jumps
    //checks absolute adress to jmp conversion

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

    //generates random adress table from rng output
    //generated adress table does not go through all memory cells

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

    //generates adress table that includes all memory cells

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

        arr[i] = tmparr2[i];

    }
    delete[]tmparr;
    delete[]tmparr2;

    //printf("\n");

    return arr;

}




int* rng(int num) {

    // generates array of random integers using LFSR


    int32_t* arrpoint;
    arrpoint = new int32_t[num];
    long long int seed = (long long int)std::time(0);  //potential issue: time resolution is 1 second
    int gencount = 0;
    long long int oldseed = seed;
    do {

        seed = ((((seed >> 31) ^ (seed >> 30) ^ (seed >> 29) ^ (seed >> 27) ^ (seed >> 25) ^ seed) & 0x00000001) << 31) | (seed >> 1);

        arrpoint[gencount] = (int32_t)seed;

        gencount += 1;

    } while (seed != oldseed && gencount < num); // will generate until loops over or generated all needed numbers

    return arrpoint;
}




