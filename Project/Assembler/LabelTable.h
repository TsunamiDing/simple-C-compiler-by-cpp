/*
 * File: LabelTable.h.
 * Declared some structures identified by myself, and the names of all of the
 * instructions and registers, and all of the opcodes and fucntion codes.
 */

#ifndef LABELTABLE_H
#define LABELTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

/*
 * The structure of every single label, including the name and its address,
 * which is the number of lines.
 */
struct Label{
    string name;
    int address = -1;
};

/*
 * The structure of every single instructure, including:
 * 1) int namenum, which is the position of the instructure's name in the InstructionSet.
 * 2) int rs, rt, rd, the number of corresponding registers.
 * 3) int immediate number.
 * 4) int target, which is the address number of the target label.
 */
struct Instruction{
    unsigned int namenum = 0;
    int rs = 0;
    int rt = 0;
    int rd = 0;
    int sa = 0;
    int immediate = 0;
    int target = 0;
};

/*
 * The structure of all of the information gotten from the input file, including
 * all of the labels and all of teh instructions.
 */
struct LabelTable{
    vector<Label> Labels;
    vector<Instruction> Instructions;
    int pc;
};

//The names of all of the instructions.
static vector<string> InstructionSet = {
    "add", "addu", "and", "nor", "or", "slt", "sltu", "sub", "subu", "xor",
    "sll", "srl",
    "div", "divu", "mult", "multu",
    "jr", "mthi", "mtlo",
    "mfhi", "mflo",
    "addi", "addiu", "andi", "slti", "sltiu", "xori", "ori",
    "beq", "bne",
    "bgez",
    "bgtz", "blez", "bltz",
    "lb", "lbu", "lw", "sb", "sw",
    "lui",
    "j", "jal",
    "syscall"
};

//The name of all of the registers.
static vector<string> registerList={
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0",   "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0",   "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8",   "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

//All of the opcodes. Ordered by the first alpha of corresponding instruction.
static vector<string> opclist = {
    "001000","001001","001100","001010","001011","001110",
    "001101","000100","000101","000001","000111","000110",
    "000001","100000","100100","100011","101000","101011",
    "001111","000010","000011","10000","10100","10000",
    "10000","00000","10000","00100","10000","10000"
};

//All of the function codes. Ordered by the first alpha of corresponding instructions.
static vector<string> funlist = {
    "100000","100001","100100","100111","100101","101010",
    "101011","100010","100011","100110","000000","000010",
    "011010","011011","011000","011001","001000","010001",
    "010011","010000","010010","000000","100000","100100",
    "000011","000000","000110","000000","000010","000001"};

/*
 * The followings are for more easily to observate the orders and positions of
 * elements in above vectors when writing code.
 */

//InstructionSet[0] = "add";
//InstructionSet[1] = "addu";
//InstructionSet[2] = "and";
//InstructionSet[3] = "nor";
//InstructionSet[4] = "or";
//InstructionSet[5] = "slt";
//InstructionSet[6] = "sltu";
//InstructionSet[7] = "sub";
//InstructionSet[8] = "subu";
//InstructionSet[9] = "xor";

//InstructionSet[10] = "sll";
//InstructionSet[11] = "srl";

//InstructionSet[12] = "div";
//InstructionSet[13] = "divu";
//InstructionSet[14] = "mult";
//InstructionSet[15] = "multu";

//InstructionSet[16] = "jr";
//InstructionSet[17] = "mthi";
//InstructionSet[18] = "mtlo";

//InstructionSet[19] = "mfhi";
//InstructionSet[20] = "mflo";

//InstructionSet[21] = "addi";
//InstructionSet[22] = "addiu";
//InstructionSet[23] = "andi";
//InstructionSet[24] = "slti";
//InstructionSet[25] = "sltiu";
//InstructionSet[26] = "xori";
//InstructionSet[27] = "ori";

//InstructionSet[28] = "beq";
//InstructionSet[29] = "bne";

//InstructionSet[30] = "bgez";

//InstructionSet[31] = "bgtz";
//InstructionSet[32] = "blez";
//InstructionSet[33] = "bltz";

//InstructionSet[34] = "lb";
//InstructionSet[35] = "lbu";
//InstructionSet[36] = "lw";
//InstructionSet[37] = "sb";
//InstructionSet[38] = "sw";

//InstructionSet[39] = "lui";

//InstructionSet[40] = "j";
//InstructionSet[41] = "jal";

//InstructionSet[42] = "syscall";

//registerList[0] = "$zero";
//registerList[1] = "$at";
//registerList[2] = "$v0";
//registerList[3] = "$v1";
//registerList[4] = "$a0";
//registerList[5] = "$a1";
//registerList[6] = "$a2";
//registerList[7] = "$a3";
//registerList[8] = "$t0";
//registerList[9] = "$t1";
//registerList[10] = "$t2";
//registerList[11] = "$t3";
//registerList[12] = "$t4";
//registerList[13] = "$t5";
//registerList[14] = "$t6";
//registerList[15] = "$t7";
//registerList[16] = "$s0";
//registerList[17] = "$s1";
//registerList[18] = "$s2";
//registerList[19] = "$s3";
//registerList[20] = "$s4";
//registerList[21] = "$s5";
//registerList[22] = "$s6";
//registerList[23] = "$s7";
//registerList[24] = "$t8";
//registerList[25] = "$t9";
//registerList[26] = "$k0";
//registerList[27] = "$k1";
//registerList[28] = "$gp";
//registerList[29] = "$sp";
//registerList[30] = "$fp";
//registerList[31] = "$ra";

#endif // LABELTABLE_H
