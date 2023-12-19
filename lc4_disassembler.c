
/************************************************************************/
/* File Name : lc4_disassembler.c 										*/
/* Purpose   : This file implements the reverse assembler 				*/
/*             for LC4 assembly.  It will be called by main()			*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"

int reverse_assemble(row_of_memory *memory) {

    row_of_memory *p = memory;
    //check if 0001 operation
    while ((p = search_opcode(p, 1))) {
        int sub_opcode = (p->contents >> 3) & 0b111;
        int d = (p->contents >> 9) & 0b111;
        int s = (p->contents >> 6) & 0b111;
        int t = p->contents & 0b111;
        int imm = p->contents & 0b11111;

        const char *instr[] = {"ADD", "MUL", "SUB", "DIV"};
        char buffer[100];

        if (sub_opcode < 4) {
            sprintf(buffer, "%s R%d, R%d, R%d", instr[sub_opcode], d, s, t);
        } else {
            sprintf(buffer, "ADD R%d, R%d, #%d", d, s, imm < 16 ? imm : imm - 32);
        }

        p->assembly = strdup(buffer);
    }

    // data assembly
    p = memory;  //initial p
    int add = 0x4000;
    while ((p = search_address(p,add)) && p->assembly == NULL) {
        char data[100];
//        strcpy(data,".FILL x");
        int value;
        value = (p->contents)& 0xFFFF;
        sprintf(data, "%s x%d", ".FILL", value);
        p->assembly = strdup(data);
        add++;
    }

    // check brn 0000
//    p = memory;
//    while ((p = search_opcode(p, 0))) {
//        int sub_opcode = (p->contents >> 9) & 0b111;
//        //int imm = p->contents & 0b111111111;
//
//        const char *instr[] = {"NOP", "BRp","BRz","BRzp","BRn", "BRnz", "BRnp","BRnzp"};
//        char nzp[100];
//        if (sub_opcode > 1) {
//            sprintf(nzp, "%s", instr[sub_opcode]);
//        }
//        p->assembly = strdup(nzp);
//    }


    // check cmp group 0001
    p = memory; // initial p
    while ((p = search_opcode(p, 2)) && p->assembly == NULL) {
        int sub_opcode = (p->contents >> 7) & 0b011;
        int s = (p->contents >> 9) & 0b111;
        int t = p->contents & 0b111;
        int imm = p->contents & 0b1111111;

        const char *instr[] = {"CMP", "CMPU", "CMPI", "CMPIU"};
        char cmp[100];

        if (sub_opcode == 0) {
            sprintf(cmp, "%s R%d, R%d", instr[0], s, t);
        } else if(sub_opcode == 1){
            sprintf(cmp, "%s R%d, R%d", instr[1], s, t);
        }else if(sub_opcode == 2){
            sprintf(cmp, "%s R%d, #%d", instr[2], s, imm < 64 ? imm : imm - 128);
        }
        else {
            sprintf(cmp, "%s R%d, #%d", instr[3], s, imm > 0 ? imm : imm - 127);
        }
        p->assembly = strdup(cmp);
    }


    //check the logic opcode 0101
    p = memory;   //initial p
    while ((p = search_opcode(p, 5)) && p->assembly == NULL) {
        int sub_opcode = (p->contents >> 3) & 0b111;
        int d = (p->contents >> 9) & 0b111;
        int s = (p->contents >> 6) & 0b111;
        int t = p->contents & 0b111;
        int imm = p->contents & 0b11111;

        const char *instr[] = {"AND", "OR", "XOR"};
        char logic[100];

        if (sub_opcode == 0) {
            sprintf(logic, "%s R%d, R%d, R%d", instr[sub_opcode], d, s, t);
        } else if(sub_opcode == 1){
            sprintf(logic, "%s R%d, R%d", "NOT", d, s);
        }else if(sub_opcode == 2){
            sprintf(logic, "%s R%d, R%d, R%d", instr[sub_opcode -1 ], d, s, t);
        } else if(sub_opcode == 3){
            sprintf(logic, "%s R%d, R%d, R%d", instr[sub_opcode -1 ], d, s, t);
        }
        else{
            sprintf(logic, "AND R%d, R%d, #%d", d, s, imm < 16 ? imm : imm - 32);
        }
        p->assembly = strdup(logic);
    }
    //check ldr 0110
    p = memory;   //initial p
    while ((p = search_opcode(p, 6))) {
        int d = (p->contents >> 9) & 0b111;
        int s = (p->contents >> 6) & 0b111;
        int imm = p->contents & 0b111111;

        char ldr[100];
        //IMM6 number
        sprintf(ldr, "LDR R%d, R%d, #%d", d, s, imm < 32 ? imm : imm - 64);
        p->assembly = strdup(ldr);
    }

    //check str 0111
    p = memory;   //initial p
    while ((p = search_opcode(p, 7))) {
        int d = (p->contents >> 9) & 0b111;
        int s = (p->contents >> 6) & 0b111;
        int imm = p->contents & 0b111111;

        char str[100];
        //IMM6 number
        sprintf(str, "STR R%d, R%d, #%d", d, s, imm < 32 ? imm : imm - 64);
        p->assembly = strdup(str);
    }

    //check rti 1000
    p = memory;   //initial p
    while ((p = search_opcode(p, 8))) {
        char rti[100];
        sprintf(rti, "RTI");
        p->assembly = strdup(rti);
    }

    //check CONST 1001
    p = memory;   //initial p
    while ((p = search_opcode(p, 9))) {
        int d = (p->contents >> 9) & 0b111;
        int imm = p->contents & 0b111111111;
        char cons[100];
        //IMM6 number
        sprintf(cons, "CONST R%d, #%d", d, imm < 256 ? imm : imm - 512);
        p->assembly = strdup(cons);
    }

    //check HICONST 1101
    p = memory;   //initial p
    while ((p = search_opcode(p, 13))) {
        int d = (p->contents >> 9) & 0b111;
        int imm = p->contents & 0b11111111;
        char hicons[100];
        //IMM6 number
        sprintf(hicons, "HICONST R%d, x%d", d, imm < 256 ? imm : imm - 512);
        p->assembly = strdup(hicons);
    }

    //check offset 1010
    p = memory;
    while ((p = search_opcode(p, 10))) {
        int sub_opcode = (p->contents >> 4) & 0b111;
        int d = (p->contents >> 9) & 0b111;
        int s = (p->contents >> 6) & 0b111;
        int t = p->contents & 0b111;
        int imm = p->contents & 0b1111;

        const char *instr[] = {"SLL", "SRA", "SRL", "MOD"};
        char offset[100];

        if (sub_opcode == 0) {
            sprintf(offset, "%s R%d, R%d, #%d",instr[sub_opcode], d, s, imm > 0 ? imm : imm - 16);
        } else if (sub_opcode == 1){
            sprintf(offset, "%s R%d, R%d, #%d",instr[sub_opcode], d, s, imm > 0 ? imm : imm - 16);
        } else if(sub_opcode == 2){
            sprintf(offset, "%s R%d, R%d, #%d",instr[sub_opcode], d, s, imm > 0 ? imm : imm - 16);
        } else if(sub_opcode == 3){
            sprintf(offset, "%s R%d, R%d, R%d", instr[sub_opcode], d, s, t);
        }
        p->assembly = strdup(offset);
    }

    //check jmp 1100
    p = memory;   //initial p
    while ((p = search_opcode(p, 12))) {
        int sub_opcode = (p->contents >> 11) & 0b001;
        int s = (p->contents >> 6) & 0b111;
        int imm = p->contents & 0b11111111111;
        char jmp[100];

        if (sub_opcode == 0) {
            sprintf(jmp, "JMPR R%d", s);
        }
        p->assembly = strdup(jmp);
    }

    //check TRAP 1111
    p = memory;   //initial p
    while ((p = search_opcode(p, 15))) {
        int imm = p->contents & 0b11111111;
        char trap[100];
        //IMM6 number
        sprintf(trap, "TRAP x%d", imm < 256 ? imm : imm - 512);
        p->assembly = strdup(trap);
    }

    return 0;
}
