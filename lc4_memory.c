/************************************************************************/
/* File Name : lc4_memory.c		 										*/
/* Purpose   : This file implements the linked_list helper functions	*/
/* 			   to manage the LC4 memory									*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"


/*
 * adds a new node to linked list pointed to by head - MUST keep list in order by address
 */
int add_to_list (row_of_memory** head, short unsigned int address, short unsigned int contents) {

    row_of_memory *target, *last = NULL, *p = *head;

    while (p && p->address < address) {
        last = p;
        p = p->next;
    }

    if (p == NULL || p->address > address) {
        target = (row_of_memory *) calloc(1, sizeof(row_of_memory));
        if (last) {
            last->next = target;
        } else {
            *head = target;
        }
        target->next = p;
    } else {
        target = p;
    }

    target->address = address;
    target->contents = contents;

    return 0;
}


/*
 * search linked list by address field, returns node if found
 */
row_of_memory* search_address (row_of_memory* head,
                               short unsigned int address )

{
    while( (head != NULL) && (head->address != address)) {
        head = head->next;
    }
    return head;

}

/*
 * search linked list by opcode field, returns node if found
 */
row_of_memory* search_opcode  (row_of_memory* head,
                               short unsigned int opcode  )
{
    row_of_memory *p = head;
    while (p && ((p->contents >> 12) != opcode || p->assembly)) {
        p = p->next;
    }
    return p;
    // done
}

//problem3
void print_list (row_of_memory* head )
{
    //print first line
    printf("%-23s %-10s %-15s %-10s\n","<label>","<address>","<contents>", "<assembly>");
    // check linklist
    while (head != NULL){
        printf("%-20s\t %04X\t\t%04X\t\t%s\n",
               head->label ? head->label : " ",
               head->address,
               head->contents,
               head->assembly ? head->assembly : " ");
        //print next node
        head = head->next;
    }
}

/*
 * delete entire linked list
 */

//write .asm file
int write_asm_file(row_of_memory* head, char* name){

    FILE * thefile = fopen(name,"w");
    if(thefile == NULL){
        printf("writen .asm failed");
        return -1;
    }

    row_of_memory *temp;
    temp = head;

    while (temp != NULL && temp->assembly != NULL) {
        fputs(temp->assembly, thefile);
        fputc('\n', thefile);
        temp = temp->next;
    }
    fclose(thefile);
    return 0;
}


int delete_list    (row_of_memory** head )
{
    row_of_memory *p = *head;
    *head = NULL;
    while (p) {
        row_of_memory *next = p->next;
        free(p->assembly);
        free(p->label);
        free(p);
        p = next;
    }
    return 0 ;
}
