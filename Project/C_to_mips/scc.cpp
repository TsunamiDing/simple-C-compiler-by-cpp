
#include<iostream>
#include "scc.h"
using namespace std;
FILE *fin = NULL;
char *filename;
char *outfile;

string text_content = ".text\n";
string data_content = ".data\n";
int current_node_counter = 0;
int tkvalue=0;


void *mallocz(int size)
{
    void *ptr;
	ptr = malloc(size);
	if (!ptr && size)
        error("cannot allocate memory");
    memset(ptr, 0, size);
    return ptr;
}

/*********************************************************** 
 * FUNC: get address in hash table
 **********************************************************/
int elf_hash(char *key)
{
    int h = 0, g;
    while (*key) 
	{
        h = (h << 4) + *key++;
        g = h & 0xf0000000;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h % MAXKEY;
}

/*********************************************************** 
 Func: get how many spaces are needed to align it
 **********************************************************/
int calc_align(int n, int align)
{                                                     
    return ((n + align - 1) & (~(align - 1)));        
}



void init ()
{
	line_num = 1;
    init_lex();

	syntax_state = SNTX_NUL;
	syntax_level = 0;
}



void clear()
{	
	int i;   
	for(i = TK_IDENT; i < tktable.count; i++)
	{	
        free(tktable.data[i]);
	}
    free(tktable.data);
}

/*********************************************************** 
 * Func: get extension name
 **********************************************************/
char *get_file_ext(char *fname)
{
	char *p;
	p = strrchr(fname,'.');
	return p+1;
}


int scc_main()
{  	
    ofstream OutFile("Test.asm");
//    fin = fopen("E:/CSC3002/project/final/build-Project-Desktop_Qt_5_14_1_MinGW_32_bit-Debug/test.c","rb");
    fin = fopen("./test.c","rb");
	if(!fin)
	{
		printf("Cannot open file\n"); 
		return 0;
	}
	
	init();
	getch();
	get_token();
    translation_unit();
	clear();

    printf("\n lexical and grammer analysis passed!\n");
	data_content = ".data\n";
	OutFile << data_content;
	OutFile << text_content;
    OutFile.close();
    OutFile.clear();
	fclose(fin);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
	return 1;
}
