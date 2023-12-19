//
// Created by 壮壮 ZHAO on 2023/12/2.
//
/************************************************************************/
/* File Name : lc4.c 													*/
/* Purpose   : This file contains the main() for this project			*/
/*             main() will call the loader and disassembler functions	*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <string.h>
#include "lc4_memory.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"

/* program to mimic pennsim loader and disassemble object files */

int main (int argc, char** argv) {


    /**
     * main() holds the linked list &
     * only calls functions in other files
     */

    /* step 1: create head pointer to linked list: memory 	*/
    row_of_memory* memory = NULL ;

    /* step 2: determine filename, then open it		*/
    /*   TODO: extract filename from argv, pass it to open_file() */

    char* filename = NULL;
    // remember open it
//    if(argc < 2){
//        fprintf(stderr, "error1: usage: ./lc4 <object_file.obj>");
//    }
    //get file name
    //filename = argv[1];

    //filename = "test_data.obj";
    //filename = "multiple_code_sections.obj";
    //filename = "add.obj";
    //filename = "labels.obj";
    //filename = "test.obj";
    //filename = "test_neg.obj";
    filename = "test_logic.obj";
    //filename = "test_cmp.obj";

    FILE *my_file;
    //call open file
    my_file =  open_file(filename);


    /* step 3: call function: parse_file() in lc4_loader.c 	*/
    /*   TODO: call function & check for errors		*/
    parse_file (my_file, &memory);


    /* step 4: call function: reverse_assemble() in lc4_disassembler.c */
    /*   TODO: call function & check for errors		*/

    reverse_assemble(memory);

    /* step 5: call function: print_list() in lc4_memory.c 	*/
    /*   TODO: call function 				*/
    printf("\ntest file name is: %s \n", filename);
    print_list(memory);

    //write .asm file
    char asmname[100];
    //get name
    strcpy(asmname, filename);
    int lenght = strlen(asmname);
    //chang file type
    strcpy(&asmname[lenght -3], "asm");

    write_asm_file(memory, asmname);


    /* step 6: call function: delete_list() in lc4_memory.c */
    delete_list(&memory);

    /*   TODO: call function & check for errors		*/

    /* only return 0 if everything works properly */
    return 0 ;
}
