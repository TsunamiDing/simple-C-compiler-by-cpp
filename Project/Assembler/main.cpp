#include "labelspliter.h"
#include "LabelTable.h"
#include "assembler.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

int main()
{
    ifstream ifs;
    ofstream ofs;
    string inputfile;
    string outputfile;

    cout<<"input file:";
    cin>>inputfile;
    cout<<"output file:";
    cin>>outputfile;

    LabelTable table = LabelSplit(ifs,inputfile);
    phase2(ofs, outputfile, table);

    return 0;
}
