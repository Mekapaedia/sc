#ifndef _SC_H_
#define _SC_H_

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct _lexeme
{
	int type;
	int func;
	double val;

} lexeme;

enum type {NONE, NUM, OP, FUNC, OPEN_PARA, CLOSE_PARA};
enum func {UNKNOWN, PLUS, MINUS, DIV, MULT, POW, MOD};

int lex(char *str, lexeme ***outer_lexeme_list, int list_pos);
double parse(lexeme **lexeme_list, int list_pos);

#endif /* _SC_H_ */
