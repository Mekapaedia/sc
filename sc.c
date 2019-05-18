#include "sc.h"

/* This program is free software. It comes without any warranty, to
     * the extent permitted by applicable law. You can redistribute it
     * and/or modify it under the terms of the Do What The Fuck You Want
     * To Public License, Version 2, as published by Sam Hocevar. See
     * http://www.wtfpl.net/ for more details. */

/* Function that prints help information to the console */
void help(char *progname);

/* Simple calculator program - lexer produces list of lexemes, then the result is parse */
int main(int argc, char **argv)
{
	int i = 0;
	lexeme **lexeme_list = NULL;
	int list_pos = 0;
	double result = 0;
	int next_num_neg = 0;
	const char delim[2] = " ";
	char *token_buf = NULL;

	/* If there are no arguments, just give the help information */
	if(argc == 1)
	{
		help(argv[0]);
		return 0;
	}

	/* Use the bash commandline to remove the whitespace and then build up the 
	   lexeme list one by one */
	for(i = 1; i < argc; i++)
	{
		char *token = NULL;
		/* Ensure we can use the arguments list again for error printing - copy the
		   arguments into a new buffer */
		token_buf = calloc(1, strlen(argv[i])+1);
		strcpy(token_buf, argv[i]);
		/* tokenise to ensure all whitespace is removed */
		token = strtok(token_buf, delim);
		/* NULL indicates we have processed all the tokens */
		while(token != NULL)
		{
			/* lex() returns the new position of the list */
			list_pos = lex(token, &lexeme_list, list_pos, &next_num_neg);
			/* strtok will NULL returns the next token */
			token = strtok(NULL, delim);
		}
		if(list_pos == -1)
		{
			int j = 0;
			/* If -1 is returned, an unknown character was found in lexing, so quit */
			printf("Unknown character error occured\n");
			/* Return the command line as this can help */
			puts("Command line was:");
			for(j = 1; j < argc; j++)
			{
				printf("%i: %s\n", j, argv[j]);
			}
			return 1;
		}
		/* Free the token buf to save memory */
		if(token_buf != NULL)
		{
			free(token_buf);
		}
	}
		
	/* parse returns the numeric value of the parsed lexeme list */
	result = parse(lexeme_list, list_pos);

	/* if parse() returns NaN, there is an incorrect number or order of numbers and operators */
	if(isnan(result))
	{
		int j = 0;

		printf("Semantic error occured - check there is the correct amount of numbers and operators\n");
		puts("Command line was:");
		for(j = 1; j < argc; j++)
		{
			printf("%i: %s\n", j, argv[j]);
		}
		return 1;
	}

	/* Print the result. %g is useful because it results reasonably formatted numbers instead of %f */
	printf("%g\n", result);

	return 0;
}

/* It's a help function - just using the name of the program (argv[0]) to be ~fancy~ */
void help(char *progname)
{
	printf("%s: Simple calculator program v1.0 by Ryan Harvey (Mekapaedia)\n", progname);
	printf("Example useage: %s 2+2/2\n", progname);
	printf("Current operators: + - * / ^ %%\n");
	printf("Licensed under WTFPL so do whatcha like\n");
}
