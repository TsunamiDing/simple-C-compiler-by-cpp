/*
 * File: assembler.c
 * According to the type of the instruction and the information stored in the labtable,
 * print out them into the output file.
 */

#include "assembler.h"
#include "LabelTable.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include<math.h>

/*
 * Function: binaryForm
 * Usage: binaryForm(integ, digit);
 * -----------------------------------------
 * Transform the decimal integer into given digits' binary form, and output it as string.
 */
string binaryForm(int integ, int digit){      //# change the inte you input to binary form with the number of digit digits and return it as strings
    int k = 0;
    int n = 1;
    string strd = "";
    while (k < digit){
        strd = strd + "0";
        k++;
    }
    while (integ >= 1){
        if (integ % 2 == 1){
            strd.replace(digit-n,1,"1",0,1);
        }
        n++;
        integ = int(integ/2);
    }
    return strd;
}

/*
 * Function: twosComplement
 * Usage: twosComplement(ofs, FileToWrite, integ, digit);
 * -----------------------------------------
 * Transform the decimal integer into given digits' 2's complement form, and output it into a file.
 */
string twosComplement(int n, int digit){
    string c = "";
    if(n >= 0){
        c += "0";
    }
    else if (n < 0){
        c += "1";
        n += int(pow(2,digit-1));
    }
    int top=0,s[109];
    do{
        s[++top] = n % 2;
        n /= 2;
    }
    while(n>0);
    for(int i = 0 ; i < digit-1-top ; i++){
        c += "0";
    }
    for(int i=top ; i>=1 ; i--){
        c += to_string(s[i]);
    }
    if(c.length() > digit){
        c = c.substr(1,c.length()-1);
    }
    return c;
}

/*
 * Function: Generate_MCode
 * Usage: Generate_MCode(ofs, FileToWrite, labtable);
 * -----------------------------------------
 * Use the given labtable, get all of imformation in it and output the binary machine code into a file.
 */
void Generate_MCode(ofstream & ofs, string FileToWrite, LabelTable labtable){
    ofs.open(FileToWrite);             // open and read the file to get all labels
    int pc = 0;
    unsigned int name = 0;
    while (pc <= labtable.pc){
        name = labtable.Instructions[pc].namenum;

        //R-type Instructions
        /*
         * Caculation instructions and logic instructions, including "add",
         * "addu", "and", "nor", "or", "slt", "sltu", "sub", "subu", "xor",
         * "sll", "srl", "div", "divu", "mult", "multu", "jr", "mthi", "mtlo",
         * "mfhi", "mflo".
         */
        if (name < 21){
            ofs<<"000000";
            ofs<<twosComplement(labtable.Instructions[pc].rs,5);
            ofs<<twosComplement(labtable.Instructions[pc].rt,5);
            ofs<<twosComplement(labtable.Instructions[pc].rd,5);
            ofs<<twosComplement(labtable.Instructions[pc].sa,5);
            ofs<<funlist[name];
        }

        //I-type Instructions
        /*
         *  Branch instructions "bgez".
         */
        else if (name == 30){
            ofs<<opclist[name-21];
            ofs<<twosComplement(labtable.Instructions[pc].rs,5);
            ofs<<"00001";
            ofs<<twosComplement(labtable.Instructions[pc].immediate-pc-1,16);
        }

        /*
         *  Branch instructions "bgtz".
         */
        else if (name == 31){
            ofs<<opclist[name-21];
            ofs<<twosComplement(labtable.Instructions[pc].rs,5);
            ofs<<"00000";
            ofs<<twosComplement(labtable.Instructions[pc].immediate-pc-1,16);
        }

        /*
         * Branch instructions, including  "beq", "bne", "blez", "bltz".
         */
        else if ((name > 27)&& (name <= 33)){
            ofs<<opclist[name-21];
            ofs<<twosComplement(labtable.Instructions[pc].rs,5);
            ofs<<twosComplement(labtable.Instructions[pc].rt,5);
            ofs<<twosComplement(labtable.Instructions[pc].immediate-pc-1,16);
        }

        /*
         * Other I-type instructions, including  "addi", "addiu", "andi", "slti",
         * "sltiu", "xori", "ori", "lb", "lbu", "lw", "sb", "sw", "lui".
         */
        else if ((name < 40)&&(name != 30)&&(name != 31)){
            ofs<<opclist[name-21];
            ofs<<twosComplement(labtable.Instructions[pc].rs,5);
            ofs<<twosComplement(labtable.Instructions[pc].rt,5);
            ofs<<twosComplement(labtable.Instructions[pc].immediate,16);
        }

        //J-type Instructions
        else if (name < 42) {
            ofs<<opclist[name-21];
            ofs<<twosComplement(labtable.Instructions[pc].target,26);
        }

        //syscall Instruction
        else if (name == 42){
            ofs<<"00000000000000000000000000001100";
        }

        else {
            ofs<<"00000000000000000000000000000000";
        }

        pc++;
        if (pc <= labtable.pc){
            ofs<<("\n");
        }
    }
    ofs.close();
}

