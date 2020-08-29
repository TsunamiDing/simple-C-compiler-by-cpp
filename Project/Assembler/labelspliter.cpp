/*
 * File: labelspliter.c
 * Split the input assembly language file into labels and instructions, and store
 * all of them into the labeltable.
 */

#include "labelspliter.h"
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
void SplitString(const string& s, vector<string>& v, const string& c){
    v.clear();
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(pos2 != string::npos){
        v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length()){
        v.push_back(s.substr(pos1));
    }
}

/*
 * Function: GetLabel
 * Usage: GetLabel(instruction);
 * -----------------------------------------
 * Get the label in a single line, which is before the instruction.
 */
vector<string> GetLabel(string instruction){
    vector<string> labels;
    SplitString(instruction, labels, ":");
    return labels;
}

/*
 * Function: FindLabel
 * Usage: FindLabel(table, label);
 * -----------------------------------------
 * Find the label in the label table, and return it's adress.
 */
int FindLabel(LabelTable table, string label){
    for (Label l : table.Labels){
        if (l.name == label){
            return l.address;
        }
    }
    return 0;
}

/*
 * Function: DeleteSpace
 * Usage: DeleteSpace(s);
 * -----------------------------------------
 * Delete all of the spaces and tabs at the beginning and end of string s.
 */
void DeleteSpace(string &s){
    if( !s.empty() ){
        s.erase(0,s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ")+1);
        s.erase(0,s.find_first_not_of("\t"));
        s.erase(s.find_last_not_of("\t")+1);
    }
}

/*
 * Function: StringReplace
 * Usage: StringReplace(ling, oldstr, newstr);
 * -----------------------------------------
 * Replace all of the string oldstr in the string line into string newstr.
 */
string StringReplace(string line, string oldstr, string newstr){
    string::size_type pos = line.find(oldstr);
    while (pos != string::npos){
        line.replace(pos, oldstr.length(), newstr);
        pos = line.find(oldstr);
    }
    return line;
}

/*
 * Function: findPosInVector
 * Usage: findPosInVector(input, str);
 * -----------------------------------------
 * Find the position of the string str in the input vector, and return it.
 */
int findPosInVector(vector<string> input , string str){
    vector<string>::iterator iter = find(input.begin(),input.end(),str);//返回的是一个迭代器指针
    if(iter == input.end())
    {
        return -1;
    } else{
        return distance(input.begin(),iter);
    }
}


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
LabelTable LabelSplit(ifstream & ifs, string FileToOpen){

    //store all the important information
    //Store all the instructions without name and label
    //Store all the Registers used in each instruction
    //Count and store the number of instructoins in the file
    LabelTable Table;
    vector<string> InstructionBodys;
    vector<string> Registers;
    int pc = 0;
    string line;

    //Open and read the file
    cout<<"!!!!!!!!!!!!"<<endl;
    ifs.open(FileToOpen);


    int text_part = 0;

    //Scan the file and find all of the labels and store them.
    int i=0;
    while(getline(ifs,line)){
        cout<<line<<endl;
        string instructionbody;
        Label label;
        i++;
        if (line == ".text"){
            text_part = 1;
            continue;
        }

        if (text_part == 1){

            pc++;

            if (line.length() <= 3){
                pc--;
                continue;
            }
            else if (line.find(":") != string::npos){

                vector<string> Instruction;
                Instruction = GetLabel(line);
                DeleteSpace(Instruction[0]);

                DeleteSpace(Instruction[1]);

                label.name = Instruction[0];
                label.address = pc-1;
                cout<<label.name<<endl;
                Table.Labels.push_back(label);

                instructionbody = Instruction[1];
                InstructionBodys.push_back(instructionbody);

            }
            else{

                    instructionbody = line;
                    InstructionBodys.push_back(instructionbody);

                }

        }

    }


    Table.pc = pc-1;
//    cout<<"Labels are"<<Table.Labels[0].name<<","<<Table.Labels[1].name<<","<<Table.Labels[2].name<<endl;

    //Scan the file and find all of the instructions and store their name and related registers.
    for (string instructionbody : InstructionBodys){
        vector<string> SplitInsBody;
        Instruction instruction;
        unsigned int space = 100;
        string name;

        if (instructionbody.find("#") != string::npos){
            instructionbody = instructionbody.substr(0,instructionbody.find('#'));
        }

        DeleteSpace(instructionbody);
        DeleteSpace(instructionbody);

        if (instructionbody == ""){
            continue;
        }

        if (instructionbody == "syscall"){
            name = "syscall";
        }

        if (instructionbody.find(" ") != string::npos){
            space = instructionbody.find(" ");
        }

        name = instructionbody.substr(0,space);
        DeleteSpace(name);
        SplitInsBody.push_back(name);
        cout<<"name is "<<name<<endl;

        string RegistersStr;
        if (instructionbody != "syscall"){
        RegistersStr = instructionbody.substr(space,instructionbody.length()-space);
        RegistersStr = StringReplace(RegistersStr, " ", "");
        RegistersStr = StringReplace(RegistersStr, "\t", "");
        RegistersStr = StringReplace(RegistersStr, "\n", "");
        SplitInsBody.push_back(RegistersStr);
//        cout<<"registers are "<<RegistersStr<<endl;
        }

        int NameNum = findPosInVector(InstructionSet , name);
        instruction.namenum = NameNum;

        if (NameNum <= 9){
            SplitString(RegistersStr, Registers, ",");
            cout<<"R0:"<<Registers[0]<<endl;
            cout<<"R1:"<<Registers[1]<<endl;
            cout<<"R2:"<<Registers[2]<<endl;
            instruction.rd = findPosInVector(registerList , Registers[0]);
            instruction.rs = findPosInVector(registerList , Registers[1]);
            instruction.rt = findPosInVector(registerList , Registers[2]);
        }
        else if (NameNum <= 11){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rd = findPosInVector(registerList , Registers[0]);
            instruction.rt = findPosInVector(registerList , Registers[1]);
            instruction.sa = stoi(Registers[2]);
        }
        else if (NameNum <= 15){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rs = findPosInVector(registerList , Registers[0]);
            instruction.rt = findPosInVector(registerList , Registers[1]);
        }
        else if (NameNum <= 18){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rs = findPosInVector(registerList , Registers[0]);
        }
        else if (NameNum <= 20){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rd = findPosInVector(registerList , Registers[0]);
        }
        else if (NameNum <= 27){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rt = findPosInVector(registerList , Registers[0]);
            instruction.rs = findPosInVector(registerList , Registers[1]);
            instruction.immediate = stoi(Registers[2]);
        }
        else if (NameNum <= 29){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rs = findPosInVector(registerList , Registers[0]);
            instruction.rt = findPosInVector(registerList , Registers[1]);
            instruction.immediate = FindLabel(Table,Registers[2]);
        }
        else if (NameNum == 30){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rs = findPosInVector(registerList , Registers[0]);
            instruction.rt = 1;
            instruction.immediate = FindLabel(Table,Registers[1]);
        }
        else if (NameNum <= 33){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rs = findPosInVector(registerList , Registers[0]);
            instruction.immediate = FindLabel(Table,Registers[1]);
        }
        else if (NameNum <= 38){
            SplitString(SplitInsBody[1], Registers, ",");
            vector<string> buffer;
            vector<string> buffer1;
            SplitString(Registers[1], buffer, "(");
            SplitString(buffer[1], buffer1, ")");

            instruction.rt = findPosInVector(registerList , Registers[0]);
            instruction.immediate = stoi(buffer [0]);
            instruction.rs = findPosInVector(registerList,buffer1[0]);
        }
        else if (NameNum <= 39){
            SplitString(SplitInsBody[1], Registers, ",");
            instruction.rt = findPosInVector(registerList , Registers[0]);
            instruction.immediate = stoi(Registers[1]);
        }
        else if (NameNum <= 41){
            instruction.target = FindLabel(Table,SplitInsBody[1]);
        }
        else if (NameNum == 42){
        }
        Table.Instructions.push_back(instruction);
        cout<<instruction.rd<<endl;
        cout<<instruction.rs<<endl;
        cout<<instruction.rt<<endl;
        cout<<instruction.sa<<endl;
        cout<<instruction.immediate<<endl;
        cout<<instruction.target<<endl;
    }
    return Table;
}
