/*
 * File: assembler.h
 * According to the type of the instruction and the information stored in the labtable,
 * print out them into the output file.
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "LabelTable.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

/*
 * Function: binaryForm
 * Usage: binaryForm(integ, digit);
 * -----------------------------------------
 * Transform the decimal integer into given digits' binary form, and output it as string.
 */
string binaryForm(int integ = 0, int digit = 0);

/*
 * Function: twosComplement
 * Usage: twosComplement(ofs, FileToWrite, integ, digit);
 * -----------------------------------------
 * Transform the decimal integer into given digits' 2's complement form, and output it into a file.
 */
void twosComplement(ofstream & ofs, string FileToWrite, int integ = 0, int digit = 0);

/*
 * Function: Generate_MCode
 * Usage: Generate_MCode(ofs, FileToWrite, labtable);
 * -----------------------------------------
 * Use the given labtable, output the binary machine code into a file.
 */
void Generate_MCode(ofstream & ofs, string FileToWrite, LabelTable labtable);

#endif // ASSEMBLER_H
