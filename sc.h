#ifndef _SC_H_
#define _SC_H_

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* lexeme struct provides the datatype for each interpreted piece of input */
typedef struct _lexeme
{
	int type;
	int func;
	double val;

} lexeme;

/* It is faster to compare numbers than character arrays, so use numbers to make it easy
   and fast to know what each piece lexed input is */
/* lexeme type - be it number or op or unknown (NONE) */
enum type {NONE, NUM, OP};
/* What functional operator - plus minus etc */
enum func {UNKNOWN, PLUS, MINUS, DIV, MULT, POW, MOD};

/* lex() is in lex.c, lexes each whitespace-delimited input string */
int lex(char *str, lexeme ***outer_lexeme_list, int list_pos, int *outer_next_num_neg);
/* parse() is in parse.c, parses the array of lexemes from lex() */
double parse(lexeme **lexeme_list, int list_pos);

#endif /* _SC_H_ */
