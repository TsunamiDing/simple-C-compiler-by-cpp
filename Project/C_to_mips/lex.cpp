#include "scc.h"
#include <iostream>
using namespace std;

TkWord* tk_hashtable[MAXKEY];
DynArray tktable;		// word_table
string tk_string, source_string;
char ch;
int token;

int line_num;


/***********************************************************
 * Function: insert to tk_list
 **********************************************************/
TkWord* tkword_direct_insert(TkWord* tp)
{
	int keyno;
	tp->sym_identifier = NULL;
	tp->sym_struct = NULL;
	dynarray_add(&tktable, tp);
	keyno = elf_hash(tp->spelling);
	tp->next = tk_hashtable[keyno];
	tk_hashtable[keyno] = tp;
	return tp;
}

/***********************************************************
 *	Func: find tkword
 **********************************************************/
TkWord* tkword_find(char* p, int keyno)
{
	TkWord* tp = NULL, * tp1;
	for (tp1 = tk_hashtable[keyno]; tp1; tp1 = tp1->next)
	{
		if (!strcmp(p, tp1->spelling))
		{
			token = tp1->tkcode;
			tp = tp1;
		}
	}
	return tp;
}


/***********************************************************
 *	Func: if there's no matched token, then insert tk_word
 **********************************************************/
TkWord* tkword_insert(char* p)
{
	TkWord* tp;
	int keyno;
	char* s;
	char* end;
	int length;

	keyno = elf_hash(p);
	tp = tkword_find(p, keyno);
	if (tp == NULL)// if there's no
	{
		length = strlen(p);
		tp = (TkWord*)mallocz(sizeof(TkWord) + length + 1);
		tp->next = tk_hashtable[keyno];
		tk_hashtable[keyno] = tp;
		dynarray_add(&tktable, tp);
		tp->tkcode = tktable.count - 1;
		s = (char*)tp + sizeof(TkWord);
		tp->spelling = (char*)s;
		for (end = p + length; p < end;)
		{
			*s++ = *p++;
		}
		*s = (char)'\0';

		tp->sym_identifier = NULL;
		tp->sym_struct = NULL;
	}
	return tp;
}

/***********************************************************
 * Func: is _ or letter
 **********************************************************/
int is_nodigit(char c)
{
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

/***********************************************************
 * Func: is digit
 **********************************************************/
int is_digit(char c)
{
	return c >= '0' && c <= '9';
}

/***********************************************************
 * FUNC: get one char
 **********************************************************/
void getch()
{
	ch = getc(fin); //get ch except at the end of the file(return eof)	
}

/***********************************************************
 *Func: parse identifier
 **********************************************************/
TkWord* parse_identifier()
{
	tk_string = "";
	tk_string += ch;

	getch();
	while (is_nodigit(ch) || is_digit(ch))
	{
		tk_string += ch;
		getch();
	}
	tk_string.erase(tk_string.find_last_not_of(" ") + 1);
	/*tk_string += '\0';*/

	char* tk_temp = (char*)tk_string.data();
	return tkword_insert(tk_temp);
}

/***********************************************************
 * Func: parse constant
 **********************************************************/
void parse_num()
{
	tk_string = "";
    source_string = "";

	do {
		tk_string += ch;
		source_string += ch;
		getch();
	} while (is_digit(ch));
	if (ch == '.') //To get decimals
	{
		do {
			tk_string += ch;
			source_string += ch;
			getch();
		} while (is_digit(ch));
	}
	/*tk_string += '\0';
	source_string += '\0';*/

	tkvalue = atoi((char*)tk_string.data());
}

/***********************************************************
 * FUNC: parse string 
	sep: ' or "
 **********************************************************/
void parse_string(char sep)
{
	char c;
	tk_string, source_string = "";
	source_string += sep;
	getch();
	for (;;)
	{
		if (ch == sep)
			break;
		else if (ch == '\\')
		{
			source_string += ch;
			getch();
			switch (ch)
			{
			case '0':
				c = '\0';
				break;
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 't':
				c = '\t';
				break;
			case 'n':
				c = '\n';
				break;
			case 'v':
				c = '\v';
				break;
			case 'f':
				c = '\f';
				break;
			case 'r':
				c = '\r';
				break;
			case '\"':
				c = '\"';
				break;
			case '\'':
				c = '\'';
				break;
			case '\\':
				c = '\\';
				break;
			default:
				c = ch;
				if (c >= '!' && c <= '~')
					warning("illegal escape character: \'\\%c\'", c);
				else
					warning("illegal escape character: \'\\0x%x\'", c);
				break;
			}
			tk_string += c;
			source_string += ch;
			getch();
		}
		else
		{
			tk_string += ch;
			source_string += ch;
			getch();
		}

	}
	tk_string += '\0';
	source_string += sep;
	source_string += '\0';
	getch();
}

/***********************************************************
 * Function:	Color the tokens
 **********************************************************/
void color_token(int lex_state)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	char* p;
	switch (lex_state)
	{
	case LEX_NORMAL:
	{
		if (token >= TK_IDENT)
			SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
		else if (token >= KW_CHAR)
			SetConsoleTextAttribute(h, FOREGROUND_BLUE);
		else if (token >= TK_CINT)
			SetConsoleTextAttribute(h, FOREGROUND_GREEN);
		else
			SetConsoleTextAttribute(h, FOREGROUND_RED);
		p = get_tkstr(token);
		printf("%s", p);
		break;
	}
	case LEX_SEP:
		printf("%c", ch);
		break;
	}
}


/***********************************************************
 * Function:	initialization
 **********************************************************/
void init_lex()
{
	TkWord* tp;
	static TkWord keywords[] = {
	{TK_PLUS,		NULL,	  "+",	NULL,	NULL},
	{TK_MINUS,		NULL,	  "-",	NULL,	NULL},
	{TK_STAR,		NULL,	  "*",	NULL,	NULL},
	{TK_DIVIDE,		NULL,	  "/",	NULL,	NULL},
	{TK_MOD,		NULL,	  "%",	NULL,	NULL},
	{TK_EQ,			NULL,	  "==",	NULL,	NULL},
	{TK_NEQ,		NULL,	  "!=",	NULL,	NULL},
	{TK_LT,			NULL,	  "<",	NULL,	NULL},
	{TK_LEQ,		NULL,	  "<=",	NULL,	NULL},
	{TK_GT,			NULL,	  ">",	NULL,	NULL},
	{TK_GEQ,		NULL,	  ">=",	NULL,	NULL},
	{TK_ASSIGN,		NULL,	  "=",	NULL,	NULL},
	{TK_POINTSTO,	NULL,	  "->",	NULL,	NULL},
	{TK_DOT,		NULL,	  ".",	NULL,	NULL},
	{TK_AND,		NULL,	  "&",	NULL,	NULL},
	{TK_OPENPA,		NULL,	  "(",	NULL,	NULL},
	{TK_CLOSEPA,	NULL,	  ")",	NULL,	NULL},
	{TK_OPENBR,		NULL,	  "[",	NULL,	NULL},
	{TK_CLOSEBR,	NULL,	  "]",	NULL,	NULL},
	{TK_BEGIN,		NULL,	  "{",	NULL,	NULL},
	{TK_END,		NULL,	  "}",	NULL,	NULL},
	{TK_SEMICOLON,	NULL,	  ";",	NULL,	NULL},
	{TK_COMMA,		NULL,	  ",",	NULL,	NULL},
	{TK_PRINTI,	NULL,	"printi",	NULL,	NULL},
	{TK_EOF,		NULL,	 "End_Of_File",	NULL,	NULL},

	{TK_CINT,		NULL,	 	"constant int",	NULL,	NULL},
	{TK_CCHAR,		NULL,		"constant char",	NULL,	NULL},
	{TK_CSTR,		NULL,		"constant string",	NULL,	NULL},

	{KW_CHAR,		NULL,		"char",	NULL,	NULL},
	{KW_SHORT,		NULL,		"short",	NULL,	NULL},
	{KW_INT,		NULL,		"int",	NULL,	NULL},
	{KW_VOID,		NULL,		"void",	NULL,	NULL},
	{KW_STRUCT,		NULL,		"struct",	NULL,	NULL},

	{KW_IF,			NULL,		"if"	,	NULL,	NULL},
	{KW_ELSE,		NULL,		"else",	NULL,	NULL},
	{KW_FOR,		NULL,		"for",	NULL,	NULL},
	{KW_CONTINUE,	NULL,		"continue",	NULL,	NULL},
	{KW_BREAK,		NULL,		"break",	NULL,	NULL},
	{KW_RETURN,		NULL,		"return",	NULL,	NULL},
	{KW_SIZEOF,		NULL,		"sizeof",	NULL,	NULL},
	{KW_ALIGN,		NULL,		"__align",	NULL,	NULL},
	{KW_CDECL,		NULL,		"__cdecl",	NULL,	NULL},
	{KW_STDCALL,	NULL,		"__stdcall",	NULL,	NULL},
	{0,				NULL,	NULL,	NULL,		NULL}
	};

	dynarray_init(&tktable, 8);
	for (tp = &keywords[0]; tp->spelling != NULL; tp++)
		tkword_direct_insert(tp);

}

/***********************************************************
 *  Function:	skip space and enters
 **********************************************************/
void skip_white_space()
{
	while (ch == ' ' || ch == '\t' || ch == '\r')
	{
		if (ch == '\r')
		{
			getch();
			if (ch != '\n')
				return;
			line_num++;
		}
		cout << ch;
		getch();
	}
}

/***********************************************************
 *  FUNC: skip space tab and enter
 **********************************************************/
void preprocess()
{
	while (1)
	{
		if (ch == ' ' || ch == '\t' || ch == '\r')
			skip_white_space();
		else if (ch == '/')
		{
			//向前多读一个字节看是否是注释开始符，猜错了把多读的字符再放回去
			getch();
			if (ch == '*')
			{
				parse_comment();
			}
			else
			{
				ungetc(ch, fin); //把一个字符退回到输入流中
				ch = '/';
				break;
			}
		}
		else
			break;
	}
}

/***********************************************************
 *  Function: skip comments
 **********************************************************/
void parse_comment()
{
	getch();
	do
	{
		do
		{
			if (ch == '\n' || ch == '*' || ch == CH_EOF)
				break;
			else
				getch();
		} while (1);
		if (ch == '\n')
		{
			line_num++;
			getch();
		}
		else if (ch == '*')
		{
			getch();
			if (ch == '/')
			{
				getch();
				return;
			}
		}
		else
		{
			error("No matched comment symbol");
			return;
		}
	} while (1);
}

/***********************************************************
 *  FUNC: get token
 **********************************************************/
void get_token()
{
	preprocess();
	switch (ch)
	{
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case '_':
	{
		TkWord* tp;
		parse_identifier();
		tp = tkword_insert((char*)tk_string.data());
		token = tp->tkcode;
		break;
	}
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
	case '8': case '9':
		parse_num();
		token = TK_CINT;
		break;
	case '+':
		getch();
		token = TK_PLUS;
		break;
	case '-':
		getch();
		if (ch == '>')
		{
			token = TK_POINTSTO;
			getch();
		}
		else
			token = TK_MINUS;
		break;
	case '/':
		token = TK_DIVIDE;
		getch();
		break;
	case '%':
		token = TK_MOD;
		getch();
		break;
	case '=':
		getch();
		if (ch == '=')
		{
			token = TK_EQ;
			getch();
		}
		else
			token = TK_ASSIGN;
		break;
	case '!':
		getch();
		if (ch == '=')
		{
			token = TK_NEQ;
			getch();
		}
		else
			error("ILLEGAL OPERATION");
		break;
	case '<':
		getch();
		if (ch == '=')
		{
			token = TK_LEQ;
			getch();
		}
		else
			token = TK_LT;
		break;
	case '>':
		getch();
		if (ch == '=')
		{
			token = TK_GEQ;
			getch();
		}
		else
			token = TK_GT;
		break;
	case '.':

		token = TK_DOT;
		
		break;
	case '&':
		token = TK_AND;
		getch();
		break;
	case ';':
		token = TK_SEMICOLON;
		getch();
		break;
	case ']':
		token = TK_CLOSEBR;
		getch();
		break;
	case '}':
		token = TK_END;
		getch();
		break;
	case ')':
		token = TK_CLOSEPA;
		getch();
		break;
	case '[':
		token = TK_OPENBR;
		getch();
		break;
	case '{':
		token = TK_BEGIN;
		getch();
		break;
	case ',':
		token = TK_COMMA;
		getch();
		break;
	case '(':
		token = TK_OPENPA;
		getch();
		break;
	case '*':
		token = TK_STAR;
		getch();
		break;
	case '\'':
		parse_string(ch);
		token = TK_CCHAR;
		tkvalue = *(char*)tk_string.data();
		break;
	case '\"':
	{
		parse_string(ch);
		token = TK_CSTR;
		break;
	}
	case EOF:
    {
		token = TK_EOF;

		break;
    }
	default:
		error("Illigal word", ch); 
		getch();
		break;
	}
	syntax_indent();
}



/***********************************************************
 * Func: get token context
 **********************************************************/
char* get_tkstr(int v)
{
	if (v > tktable.count)
		return NULL;
	else if (v >= TK_CINT && v <= TK_CSTR)
		return (char*)source_string.data();
	else
		return ((TkWord*)tktable.data[v])->spelling;
}


