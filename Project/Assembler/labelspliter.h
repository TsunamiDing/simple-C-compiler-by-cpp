/*
 * File: labelspliter.h
 * Split the input assembly language file into labels and instructions, and store
 * all of them into the labeltable.
 */

#ifndef LABELSPLIT_H
#define LABELSPLIT_H

#include "LabelTable.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

/*
 * Function: SplitString
 * Usage: binaryForm(s, v, c);
 * -----------------------------------------
 * Split the string s into several part, by seperator c. The gotten strings are
 * stored in the vector v.
 */
void SplitString(const string& s, vector<string>& v, const string& c);

/*
 * Function: GetLabel
 * Usage: GetLabel(instruction);
 * -----------------------------------------
 * Get the label in a single line, which is before the instruction.
 */
vector<string> GetLabel(string instruction);

/*
 * Function: FindLabel
 * Usage: FindLabel(table, label);
 * -----------------------------------------
 * Find the label in the label table, and return it's adress.
 */
int FindLabel(LabelTable table, string label);

/*
 * Function: DeleteSpace
 * Usage: DeleteSpace(s);
 * -----------------------------------------
 * Delete all of the spaces and tabs at the beginning and end of string s.
 */
void DeleteSpace(string &s);

/*
 * Function: StringReplace
 * Usage: StringReplace(ling, oldstr, newstr);
 * -----------------------------------------
 * Replace all of the string oldstr in the string line into string newstr.
 */
string StringReplace(string line, string oldstr, string newstr);

/*
 * Function: findPosInVector
 * Usage: findPosInVector(input, str);
 * -----------------------------------------
 * Find the position of the string str in the input vector, and return it.
 */
int findPosInVector(vector<string> input , string str);

/*
 * Function: LabelSplit
 * Usage: LabelSplit(ifs, FileToOpen);
 * -----------------------------------------
 * 1) Open the test file and read through it.
 * 2) Get all of the labels in the .text part of assembly language file, and
 *    store them into the labtable.
 * 3) Get all of the instrucions in teh .text part of assembly language file,
 *    including the instruction name, the related registers and immediate numbers,
 *    and sotre them in to the labtable.
 */
LabelTable LabelSplit(ifstream & ifs, string FileToOpen);


#endif // LABELSPLIT_H
