
#include "scc.h"
#include <string>
using namespace std;
int syntax_state;  
int syntax_level;  
int reg_counter = 0;
G_Node node_list[256];
G_Node* root_node= &node_list[0];

G_Node* current_node=root_node;

int not_print = 1;


/***********************************************************
 * Func: translate a code block
 **********************************************************/
void translation_unit()
{
	
	while(token != TK_EOF)
	{
		
		external_declaration(SC_GLOBAL);
		text_content += translate_block(root_node);
		clear_tree();

	}
	data_content += translate_data();
	
}

/***********************************************************
 * <external_declaration>::=
 *  <type_specifier> (<TK_SEMICOLON> 
 *      |<declarator><funcbody>
 *	    |<declarator>[<TK_ASSIGN><initializer>]
 *	     {<TK_COMMA> <declarator>[<TK_ASSIGN><initializer>]}
 *		 <TK_SEMICOLON>
 **********************************************************/
void external_declaration(int l)
{	

	if (!type_specifier())
	{
		expect("Type specifier");

	}	
	/*cout << "!!" << token << "!!";*/
	
    while (1)
	{								
		build_tree(root_node, current_node, 0, DIRECT_DECLARATOR, NULL, "");
		G_Node* temp_n = current_node;
		current_node = current_node->mid;
		direct_declarator();
		current_node = temp_n;
		if (token == TK_SEMICOLON) current_node->right = NULL;

		if (token == TK_BEGIN) 
		{
			if (l == SC_LOCAL)
				error("Do not support nested function");  		
			build_tree(root_node, current_node, 1, COMPOUND_STATEMENT, NULL, "");
			current_node = current_node->right;
			compound_statement();
			current_node = current_node->father;
			break;
		}
		else
		{
			
			if(token == TK_ASSIGN)	
			{
				get_token();
				build_tree(root_node, current_node, 1, EXPRESSION, NULL, "",reg_counter);
				reg_counter = (reg_counter + 1) %6;
				current_node = current_node->right;
			    expression();
				current_node = current_node->father;
			}	
			

			syntax_state = SNTX_LF_HT;
			skip(TK_SEMICOLON);
			break;		
		}
	}	


}

/***********************************************************
 * <variable_declaration>::=
 *  <type_specifier><declarator> |
 * <type_specifier><declarator>[<TK_ASSIGN><expression>]
 **********************************************************/

void variable_declaration()
{
	if (!type_specifier())
	{
		expect("Type specifier");
	}


	if (token >= TK_IDENT)
	{
		current_node->left->text_list[0] = tk_string;
		get_token();
	}
	else
	{
		expect("标识符");
	}
		

	if (token == TK_ASSIGN)
	{
		get_token();
		build_tree(root_node, current_node, 1, EQUALITY_EXPRESSION, NULL, "", reg_counter);
		reg_counter = (reg_counter + 1) % 6;
		current_node = current_node->right;
		equality_expression();
		current_node = current_node->father;
	}


	syntax_state = SNTX_LF_HT;
	skip(TK_SEMICOLON);
}




/***********************************************************
 *	<type_specifier>::= <KW_INT> 
 *		| <KW_CHAR> 
 *		| <KW_SHORT> 
 *		| <KW_VOID >
 *		| <struct_specifier>
 **********************************************************/
int type_specifier()
{		
	int type_found = 0;
	int space = 0;
    switch(token) 
	{
    case KW_CHAR:
		build_tree(root_node, current_node, -1, TYPE_SPECIFIER, GM_CHAR, tk_string);
		type_found = 1;
		space = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;
	case KW_SHORT:
		build_tree(root_node, current_node, -1, TYPE_SPECIFIER, GM_SHORT, tk_string);
		space = 2;
		type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;
	case KW_VOID:
		build_tree(root_node, current_node, -1, TYPE_SPECIFIER, GM_VOID, tk_string);
		type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;
	case KW_INT:
		build_tree(root_node, current_node, -1, TYPE_SPECIFIER, GM_INT, tk_string);
		syntax_state = SNTX_SP;
		type_found = 1;
		space = 4;
		get_token();
		break;
	default:		
		break;
	}
	return type_found*10+space;
}




/***********************************************************
 * <direct_declarator>::=  <IDENTIFIER><parameter_name_list>
 **********************************************************/
int direct_declarator()
{
	
	if (token >= TK_IDENT) 
	{		 
		
		build_tree(root_node, current_node, -1, IDENTIFIER, NULL, tk_string);
		get_token();
		
	} 
	else
	{
		expect("标识符");
	}
	if (token == TK_OPENPA)
	{	
		build_tree(root_node, current_node, 1, PARAMETER_TYPE_LIST, NULL, "");
		current_node = current_node->right;
		parameter_type_list();
		current_node = current_node->father;
		return 0;
	}
	return 1;
}
/***********************************************************
 * <parameter_type_list>::=<parameter_list>
 *			|<parameter_list><TK_COMMA><TK_ELLIPSIS>
 *
 *  <parameter_list>::=<parameter_declaration>
 *			{<TK_COMMA ><parameter_declaration>}
 **********************************************************/
void parameter_type_list()
{

	get_token();
	build_tree(root_node, current_node, -1, TYPE_SPECIFIER, NULL, "");
	build_tree(root_node, current_node, 1, DIRECT_DECLARATOR, NULL, "");
	int para_counter = 0;
	while (token != TK_CLOSEPA)
	{
		if (!type_specifier())
		{
			error("Invalid Specifier");
		}
		current_node->left->text_list[para_counter] = current_node->left->text;
		if (token >= TK_IDENT)
		{
			current_node->right->text_list[para_counter] = tk_string;
			get_token();
		}
		else
		{
			expect("Identifier");
		}
		para_counter++;
		if (token == TK_CLOSEPA)
			break;
		
		skip(TK_COMMA);
		
	}
	current_node->reg_num = para_counter;
	syntax_state = SNTX_DELAY;
	skip(TK_CLOSEPA);
	if (token == TK_BEGIN)			
		syntax_state = SNTX_LF_HT;
	else							
		syntax_state = SNTX_NUL;
	syntax_indent();
}



int is_type_specifier(int v)
{
	if (v == KW_CHAR || v == KW_SHORT || v == KW_INT || v == KW_VOID ) return 1;
	else return 0;
}

/***********************************************************
 * <statement >::=<compound_statement> 
 *		| <if_statement> 
 *		| <return_statement> 
 *		| <break_statement> 
 *		| <continue_statement> 
 *		| <for_statement> 
 *		| <expression_statement>
 **********************************************************/
void statement()
{	

	switch(token)
	{
	case KW_IF:
		build_tree(root_node, current_node, 0, IF_STATEMENT, NULL, "");
		current_node = current_node->mid;
		if_statement();
		break;
	case KW_RETURN:
		build_tree(root_node, current_node, 0, RETURN_STATEMENT, NULL, "");
		current_node = current_node->mid;
		return_statement();
		break;
	default:
		build_tree(root_node, current_node, 0, EXPRESSION_STATEMENT, NULL, "");
		current_node = current_node->mid;
		
		expression_statement();
		break;
	}	
}

/***********************************************************
 * <compound_statement>::=<TK_BEGIN>{<declaration>}{<statement>}<TK_END>
 **********************************************************/
void compound_statement()
{ 	
	syntax_state = SNTX_LF_HT;
	syntax_level++;						// how many indents(tabs)
	//cout << 1;
	G_Node* temp_ptr = current_node;
	get_token();
	/*while(is_type_specifier(token))
	{
		external_declaration(SC_LOCAL);
	}*/
	while (token != TK_END) 
	{		
		
		if (is_type_specifier(token)) {
			build_tree(root_node, current_node, 0, EXTERNAL_DECLARATOR, NULL, "");
			current_node = current_node->mid;
			variable_declaration();
		}
		else {
			statement();
		}
	}		
	
	syntax_state = SNTX_LF_HT;
	get_token();	
}

/***********************************************************
 * <if_statement>::=<KW_IF><TK_OPENPA><expression>
 *	<TK_CLOSEPA><statement>[<KW_ELSE><statement>]
 **********************************************************/
void if_statement()
{
	syntax_state = SNTX_SP;
	get_token();
	skip(TK_OPENPA);

	build_tree(root_node, current_node, -1, EQUALITY_EXPRESSION, NULL, "", reg_counter);
	reg_counter = (reg_counter + 1) %6;
	current_node = current_node->left;
	equality_expression();
	current_node = current_node->father;

	syntax_state = SNTX_LF_HT;		
	skip(TK_CLOSEPA);

	if(token == TK_BEGIN){
		build_tree(root_node, current_node, 1, COMPOUND_STATEMENT, NULL, "");
		G_Node* temp_node = current_node;
		current_node = current_node->right;
		compound_statement();
		current_node = temp_node;
	}
	else{
		build_tree(root_node, current_node, 1, EXPRESSION, NULL, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->right;
		expression();	
		current_node = current_node->father;
	}
	
}

/***********************************************************
 * <return_statement>::=<KW_RETURN><TK_SEMICOLON>
 *			|<KW_RETURN><expression><TK_SEMICOLON>
 **********************************************************/
void return_statement()
{
	syntax_state = SNTX_DELAY;
	get_token();
	if(token == TK_SEMICOLON)	// 适用于 return;
		syntax_state = SNTX_NUL;
	else						// 适用于 return <expression>;
		syntax_state = SNTX_SP;
	syntax_indent();
	
	if (token != TK_SEMICOLON) 
	{
		build_tree(root_node, current_node, -1, EQUALITY_EXPRESSION, NULL, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->left;
		equality_expression();
		current_node = current_node->father;
	}
	syntax_state = SNTX_LF_HT;
	skip(TK_SEMICOLON);		
}

/***********************************************************
 * <expression_statement>::= <TK_SEMICOLON>|<expression> <TK_SEMICOLON> 
 **********************************************************/
void expression_statement()
{	
	not_print = 1;
	if (token == TK_PRINTI) {
		not_print = 0;
		build_tree(root_node, current_node, -1, GM_PRINTI, NULL, "", reg_counter);
		reg_counter = (reg_counter + 1) % 6;
		current_node = current_node->left;
		print_integer();
		current_node = current_node->father;
	}
	else if (token != TK_SEMICOLON) 
	{	
		
		build_tree(root_node, current_node, -1, EXPRESSION, NULL, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->left;
		expression();
		current_node = current_node->father;
		
	}
	syntax_state = SNTX_LF_HT;

	skip(TK_SEMICOLON);
	
}

void print_integer() {
	get_token();
	skip(TK_OPENPA);
	if (token >= TK_IDENT) {
		build_tree(root_node, current_node, -1, IDENTIFIER, NULL, "");
		current_node->left->text = tk_string;
	}
	get_token();
	skip(TK_CLOSEPA);
}

/***********************************************************
 * <assignment_expression>::= <equality_expression>
 *		|<unary_expression><TK_ASSIGN> <equality_expression> 
 **********************************************************/
void expression()
{		
	
	build_tree(root_node, current_node, -1, EQUALITY_EXPRESSION, NULL, "", reg_counter);
	reg_counter = (reg_counter + 1) % 6;
	current_node = current_node->left;
	primary_expression();
	current_node = current_node->father;
	if (token == TK_OPENPA && not_print==1) {
		skip(TK_OPENPA);
		skip(TK_CLOSEPA);
		current_node->left->grammer_type = FUNCTION_CALL;
	}
    if (token == TK_ASSIGN) 
	{
        get_token();
		build_tree(root_node, current_node, 1, EQUALITY_EXPRESSION, NULL, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->right;
		relational_expression();
		current_node = current_node->father;
    }
	
}

/***********************************************************
 * < equality_expression >::=<relational_expression>
 *		{<TK_EQ> <relational_expression>
 *		|<TK_NEQ><relational_expression>}
 **********************************************************/
void equality_expression()
{

    int t;
	//cout << 55;
	build_tree(root_node, current_node, -1, RELATIONAL_EXPRESSION, NULL, "", reg_counter);
	reg_counter = (reg_counter + 1) %6;
	current_node = current_node->left;
	relational_expression();
	current_node = current_node->father;
    
	
    while (token == TK_EQ || token == TK_NEQ) 
	{
        t = token;
        get_token();

		build_tree(root_node, current_node, 1, RELATIONAL_EXPRESSION, t, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->right;
		relational_expression();
		current_node = current_node->father;
    }
}

/***********************************************************
 * <relational_expression>::=<additive_expression>{
 *		<TK_LT><additive_expression> 
 *		|<TK_GT><additive_expression> 
 *		|<TK_LEQ><additive_expression> 
 *		|<TK_GEQ><additive_expression>}
 **********************************************************/
void relational_expression()
{	
	//cout << 66;
	build_tree(root_node, current_node, -1, ADDITIVE_EXPRESSION, NULL, "",reg_counter);
	reg_counter = (reg_counter + 1) %6;
	current_node = current_node->left;
	additive_expression();
	current_node = current_node->father;
    
	
    while ((token == TK_LT || token == TK_LEQ) ||
           token == TK_GT || token == TK_GEQ) 
	{	
		int t = token;
        get_token();
        
		build_tree(root_node, current_node, 1, ADDITIVE_EXPRESSION, t, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->right;
		additive_expression();
		current_node = current_node->father;
    }
}

/***********************************************************
 * <additive_expression>::=< multiplicative_expression> 
 *		{<TK_PLUS> <multiplicative_expression>
 *		<TK_MINUS>< multiplicative_expression>}
 **********************************************************/
void additive_expression()
{	
	//cout << 77;
	build_tree(root_node, current_node, -1, MULTIPLICATIE_EXPRESSION, NULL, "",reg_counter);
	reg_counter = (reg_counter + 1) %6;
	current_node = current_node->left;
	multiplicative_expression();
	current_node = current_node->father;
    
	
    while (token == TK_PLUS || token == TK_MINUS)
	{
		int t = token;
		get_token();

		build_tree(root_node, current_node, 1, MULTIPLICATIE_EXPRESSION, t, "",reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->right;
		multiplicative_expression();
		current_node = current_node->father;
    }
}

/***********************************************************
 * <multiplicative_expression>::=<unary_expression>
 *		{<TK_STAR>  < unary_expression >
 *		|<TK_DIVIDE>< unary_expression > 
 *		|<TK_MOD>  < unary_expression >}
 **********************************************************/
void multiplicative_expression()
{
    int t;
	//cout << 88;
	build_tree(root_node, current_node, -1, UNARY_EXPRESSION, NULL, "", reg_counter);
	reg_counter= (reg_counter+1)%8;
	current_node = current_node->left;
	unary_expression();
	current_node = current_node->father;
    
	
    while (token == TK_STAR || token == TK_DIVIDE ) 
	{
        t = token;
        get_token();
        
		build_tree(root_node, current_node, 1, UNARY_EXPRESSION,t, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->right;
		unary_expression();
		current_node = current_node->father;
    }
}

/***********************************************************
 * <unary_expression>::= <postfix_expression> 
 *			|<TK_AND><unary_expression> 
 *			|<TK_STAR><unary_expression> 
 *			|<TK_PLUS><unary_expression> 
 *			|<TK_MINUS><unary_expression> 
 *			|<KW_SIZEOF><TK_OPENPA><type_specifier><TK_ CLOSEPA> 
 **********************************************************/
void unary_expression()
{
	//cout << 99;
    switch(token) 
	{
	case TK_MINUS:
        get_token();

		build_tree(root_node, current_node, -1, UNARY_EXPRESSION, TK_MINUS, "", reg_counter);
		reg_counter = (reg_counter + 1) %6;
		current_node = current_node->left;
		unary_expression();
		current_node = current_node->father;
        
        break;
   
	default:
		build_tree(root_node, current_node, -1, PRIMARY_EXPRESSION, NULL, "");
		current_node = current_node->left;
		primary_expression();
		current_node = current_node->father;
		
		break;
	}

}




/***********************************************************
 * <primary_expression>::=<IDENTIFIER>
 *		|<TK_CINT>
 *		|<TK_CSTR>
 *		|<TK_CCHAR>
 *		|<TK_OPENPA><expression><TK_CLOSEPA>
 **********************************************************/
void primary_expression()
{
	int t;
	
	switch(token) 
	{
    case TK_CINT: 
		
		current_node->text =tk_string;
		
		current_node->my_token = TK_CINT;
		
		get_token();
		break;
    case TK_CCHAR:
		current_node->text = tk_string;
		current_node->my_token = TK_CCHAR;
		get_token();
        break;
	
	default:
		 t = token;	
		 current_node->text = tk_string;
		 get_token();
		 if (t < TK_IDENT) {
			 current_node->text = "";
			 expect("Identifier or Constant");
		 }
		 break;
	}
}



void print_tab(int n)
{
	int i = 0;
	for(; i < n; i++)
	    printf("\t");
}


void syntax_indent()
{
	switch(syntax_state)
	{
		case SNTX_NUL:
			color_token(LEX_NORMAL);
			break;	
		case SNTX_SP:
			printf(" ");
			color_token(LEX_NORMAL);
			break;	
		case SNTX_LF_HT:
			{	
				if(token == TK_END)		
					syntax_level--;
				printf("\n");
				print_tab(syntax_level);				
			}
			color_token(LEX_NORMAL);
			break;		
		case SNTX_DELAY:
			break;
	}	
	syntax_state = SNTX_NUL;
}


