
#include "scc.h"

/***********************************************************
 * FUNC: handle exception 
 * stage: not being used
 * fmt:	format
 * ap:	variable list
 **********************************************************/
void handle_exception(int stage,int level,char *fmt,va_list ap)
{
	char buf[1024];
	vsprintf(buf,fmt,ap);
	if(stage == STAGE_COMPILE)
	{
		if(level==LEVEL_WARNING)
			printf("%s(第%d行) : 编译警告: %s!\n",filename,line_num,buf);
		else
		{
			printf("%s(第%d行) : 编译错误: %s!\n",filename,line_num,buf);
			exit(-1);
		}
	}
	else
	{
		printf("链接错误:%s!\n",buf);
		exit(-1);
	}
}

/***********************************************************
 * Func: warning
 **********************************************************/
void warning(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    handle_exception(STAGE_COMPILE,LEVEL_WARNING, fmt, ap);
    va_end(ap);
}

/***********************************************************
 * Func: error
 **********************************************************/
void error(char *fmt, ...)
{
	va_list ap;
    va_start(ap, fmt);
    handle_exception(STAGE_COMPILE,LEVEL_ERROR, fmt, ap);
    va_end(ap);
}

/***********************************************************
 * Func: expect
 **********************************************************/
void expect(char *msg)
{
    error("缺少%s", msg);
}

/***********************************************************
 * Func: if the token is v, then get next one.
		else, generate an error
 **********************************************************/
void skip(int v)
{
    if (token != v)
       error("缺少'%s'", get_tkstr(v));
    get_token();
}

/***********************************************************
 * Func: link error to where it came from
 **********************************************************/
void link_error(char *fmt, ...)
{
	va_list ap;
    va_start(ap, fmt);
    handle_exception(STAGE_LINK,LEVEL_ERROR, fmt, ap);
    va_end(ap);
}
