/************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"

/* declarations of functions that must defined in lc4_loader.c */
// open file
FILE* open_file(char* file_name){
    FILE *my_file;
    my_file = fopen(file_name,"rb");

    if(my_file ==NULL){
        printf("error: open_file() failed \n");
        return NULL;
    } else{
        return my_file;
    }


}

//swap file contain
unsigned short endianness(unsigned short value){

    return (value<< 8)|(value >>8);
}

//read file and parse file contain

int parse_file (FILE* my_obj_file, row_of_memory** memory){

    while (!feof(my_obj_file)) {
        unsigned short type, address, n;
        //find type
        fread(&type, sizeof(unsigned short), 1, my_obj_file);
        //find address
        fread(&address, sizeof(unsigned short), 1, my_obj_file);
        // find instr count n
        fread(&n, sizeof(unsigned short), 1, my_obj_file);

        //swap header
        type = endianness(type);
        address = endianness(address);
        n = endianness(n);

        //if CODE if DATA if SYMBOL
        if (type == 0xcade) {
            for (int i = 0; i < n; ++i) {
                unsigned short intr;
                fread(&intr, sizeof(unsigned short), 1, my_obj_file);
                intr = endianness(intr);
                add_to_list(memory, address, intr);

                //printf("all code contain %X\n", intr);

                address++;
            }
        }
        else if (type == 0xDADA) {
            for(int i = 0; i < n; ++i){
                unsigned short data;
                fread(&data, sizeof(unsigned short), 1, my_obj_file);
                data = endianness(data);
                add_to_list(memory,address,data);

                //printf("all code contain %X\n", data);

                address++;
            }

        }
        else if (type == 0xC3B7) {
            //
            char *label = (char *) malloc(n + 1);
            fread(label, sizeof(char), n, my_obj_file);
            label[n] = '\0';

            row_of_memory* node = search_address( *memory, address);
            if (node == NULL) {
                add_to_list(memory,address,0);
                node = search_address( *memory, address);
            }
            node->label = label;
        }

    }

    return 0;
}
