
#include<iostream>
#include <windows.h>
#include<fstream>

using namespace std;

int scc_main();



extern string tk_string, source_string;
extern string func_name_list[256];

/*******************************dynarray****************************/
typedef struct DynArray 
{
    int count;			
    int capacity;		
    void **data;		
} DynArray;

void dynarray_add(DynArray *parr, void *data);
void dynarray_init(DynArray *parr,int initsize);
void dynarray_free(DynArray *parr);                 
int dynarray_search(DynArray *parr, int key);
/*******************************dynarray****************************/


/*******************************for grammer tree****************************/
extern int tkvalue;
extern string var_list[256];
extern string var_type_list[256];
extern int var_counter ;
extern int reg_counter;
extern int primary_count_num;
extern int if_label;


typedef struct G_Node
{		
	int grammer_type;
	//int reg=-1;
	string text="";
	string text_list[64];
	int my_token=-1;
	int reg_num = -1;
	struct G_Node* father=NULL;
	struct G_Node* left = NULL;
	struct G_Node* mid = NULL;
	struct G_Node* right = NULL;
}G_Node;

string add_or_find_var(string org_name, string type);
extern G_Node* current_node;
extern G_Node* root_node;
G_Node* build_tree(G_Node* root, G_Node* current_node, int left_mid_right, int grammer_type, int my_token, string text);
G_Node* build_tree(G_Node* root, G_Node* current_node, int left_mid_right, int grammer_type, int my_token, string text,int reg_num);
void clear_tree();
/*******************************for grammer tree****************************/

/*******************************for translation****************************/
string translate_data(); //generate .data part for mips code
string translate_block(G_Node* current_node); //translate a code block/external declarator
string translate_var_declarator(G_Node* current_node);
string translate_direct_declarator(G_Node* current_node);
void translate_parameter_list(G_Node* current_node);
string translate_compound_statement(G_Node* current_node);
string translate_mid(G_Node* current_node);
string translate_if_statement(G_Node* current_node);
string translate_return_statement(G_Node* current_node);
string translate_expression_statement(G_Node* current_node);
string translate_expression(G_Node* current_node);
string translate_equality_expression(G_Node* current_node);
string translate_relational_expression(G_Node* current_node);
string translate_additive_expression(G_Node* current_node);
string translate_multiplicative_expression(G_Node* current_node);
string translate_unary_expression(G_Node* current_node);
string translate_print_integer(G_Node* current_node);
string get_text(string temp);
string get_reg_num(string temp);
enum grammer_type
{
	BLOCK,
	EXTERNAL_DECLARATOR,
	TYPE_SPECIFIER,
	DIRECT_DECLARATOR,
	GM_CHAR,
	GM_SHORT,
	GM_VOID,
	GM_INT,
	IDENTIFIER,
	STATEMENT,
	PARAMETER_TYPE_LIST,
	INITIALIZER,
	COMPOUND_STATEMENT,
	VARIABLE_DECLARATION,
	IF_STATEMENT,
	RETURN_STATEMENT,
	EXPRESSION_STATEMENT,
	EXPRESSION,
	EQUALITY_EXPRESSION,
	RELATIONAL_EXPRESSION,
	ADDITIVE_EXPRESSION,
	MULTIPLICATIE_EXPRESSION,
	UNARY_EXPRESSION,
	PRIMARY_EXPRESSION,
	GM_PRINTI,
	FUNCTION_CALL
};

extern int current_node_counter;
extern string text_content ;
extern string data_content;
/*******************************for translation****************************/

/********************************lex.h begin*****************************/
enum e_TokenCode
{  
	TK_PLUS,		
    TK_MINUS,		
    TK_STAR,		
    TK_DIVIDE,		
    TK_MOD,			
    TK_EQ,			
    TK_NEQ,			// != 
    TK_LT,			// < 
    TK_LEQ,			// <= 
    TK_GT,			// > 
    TK_GEQ,			// >= 
    TK_ASSIGN,		// = 
    TK_POINTSTO,	// -> 
    TK_DOT,			
	TK_AND,         
	TK_OPENPA,		// ( 
	TK_CLOSEPA,		// ) 
	TK_OPENBR,		// [ 
	TK_CLOSEBR,		// ] 
	TK_BEGIN,		// { 
	TK_END,			// } 
    TK_SEMICOLON,	
    TK_COMMA,		
	TK_PRINTI,		//print int
	TK_EOF,			// end file

    TK_CINT,		
    TK_CCHAR,		
    TK_CSTR,		

	KW_CHAR,		
	KW_SHORT,		
	KW_INT,			
    KW_VOID,		
    KW_STRUCT,		
	KW_IF,			
	KW_ELSE,		
	KW_FOR,			
	KW_CONTINUE,	
    KW_BREAK,		
    KW_RETURN,		
    KW_SIZEOF,		

    KW_ALIGN,		
    KW_CDECL,	
	KW_STDCALL,   
	
	TK_IDENT //identifier
};


enum e_LexState
{
	LEX_NORMAL,
	LEX_SEP
};

//Token word
typedef struct TkWord
{
    int  tkcode;					
    struct TkWord *next;			// next word in the same section in hashtable
    char *spelling;					// data
    struct Symbol *sym_struct;		
    struct Symbol *sym_identifier;	
} TkWord;

#define MAXKEY	1024				// capacity of hashtable

extern TkWord* tk_hashtable[MAXKEY];
extern DynArray tktable;			
#define CH_EOF   (-1)				

TkWord* tkword_direct_insert(TkWord* tp);
TkWord* tkword_insert(char * p);
int  is_nodigit(char c);
int  is_digit(char c);
void getch();
void parse_num();
void parse_string(char sep);
void parse_comment();
void get_token();
char *get_tkstr(int v);
void init_lex();
void color_token(int lex_state);
/*****************************lex.h*************************************/


/*****************************error.h***********************************/

enum e_ErrorLevel
{
	LEVEL_WARNING,
	LEVEL_ERROR,
};

enum e_WorkStage
{
	STAGE_COMPILE,
	STAGE_LINK,
};
void warning(char *fmt, ...);
void error(char *fmt, ...);
void expect(char *msg);
void skip(int c);
void link_error(char *fmt, ...);
/*****************************error.h end*************************************/


/*******************************grammar.h begin****************************/

enum e_SynTaxState
{
	SNTX_NUL,       // NULL
	SNTX_SP,		// space int a; int __stdcall MessageBoxA(); return 1;
	SNTX_LF_HT,		// \ n \ t
	SNTX_DELAY      // 
};

// storage type
enum e_StorageClass
{
	SC_GLOBAL =   0x00f0,		          
	SC_LOCAL  =   0x00f1,		
	SC_LLOCAL =   0x00f2,       
	  
};


enum e_TypeCode
{
	T_INT    =  0,			                   
	T_CHAR   =  1,			           
	T_SHORT  =  2,			                  
	T_VOID   =  3,			             
	T_PTR    =  4,			                      
	T_FUNC   =  5,			             
	T_STRUCT =  6,			
	
	T_BTYPE  =  0x000f,		   
	T_ARRAY  =  0x0010,		
};

#define ALIGN_SET 0x100  

extern int syntax_state;
extern int syntax_level;

void translation_unit();
void external_declaration(int l);
void variable_declaration();
void print_integer();
int type_specifier();
int direct_declarator();
void parameter_type_list();
void statement();
void compound_statement();
int is_type_specifier(int v);
void statement();
void expression_statement();
void if_statement();
void return_statement();

void expression();
void equality_expression();
void relational_expression();
void additive_expression();
void multiplicative_expression();
void unary_expression();
void primary_expression();
void syntax_indent();
/*******************************grammar.h end****************************/

/*******************************scc.h begin****************************/
extern FILE *fin;
extern char ch;
extern char *filename;
extern int token;

extern int line_num;
int elf_hash(char *name);
void *mallocz(int size);
/******************************scc.h end*************************/
