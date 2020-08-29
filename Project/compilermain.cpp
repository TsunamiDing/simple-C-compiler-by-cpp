#include <stdlib.h>
#include "Simulator/simulator.h"
#include <cstring>
#include <fstream>
#include "Assembler/labelspliter.h"
#include "Assembler/LabelTable.h"
#include "Assembler/assembler.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "C_to_mips/scc.h"
#include "mainwindow.h"
#include <WINDOWS.H>




#include <QApplication>


using namespace std;



void compilermain(std::string strpath)
{
    ifstream ifs;
    ofstream ofs;
    string inputfile;
    string machinecodefile;

    cout<<strpath<<endl;

    QString destPath="./test.c";
    QString sourcepath=QString::fromStdString(strpath);
    QStringList filters;
    QDir* createFile = new QDir;
   if(createFile->exists(destPath)){
      createFile->remove(destPath);
   }
   if(!QFile::copy(sourcepath, destPath)){
      return;
   }


    inputfile="./Test.asm";
    machinecodefile="./Test.txt";

    scc_main();

    LabelTable table = LabelSplit(ifs,inputfile);

    Generate_MCode(ofs, machinecodefile, table);

    execute(inputfile, machinecodefile);
}
