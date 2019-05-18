#include "sc.h"

/* create_lex() uses the buffer and the lexeme type to create the lexeme entry */
lexeme *create_lex(char *buf, int type);
/* resize_list() is just a utility function to resize the list to whatever required size */
lexeme **resize_list(lexeme **list, int size);

/* lex() takes in the whitespace-cleaned string from main(), a pointer to the lexeme list
   and the position in the list and returns lexed answers into that list */
int lex(char *str, lexeme ***outer_lexeme_list, int list_pos, int *outer_next_num_neg)
{
	int i = 0;
	int num_started = 0;
	char buf[64]; /* 64 bytes is longer than any possible double number */
	int buf_pos = 0;
	int next_num_neg = 0; /* This is a cheat to not have to parse negative numbers */
	lexeme **lexeme_list = NULL; /* This prevents you from having to use (*outer_lexeme_list) */
	
	/* If the list pointer given is non-NULL, continue to grow that. Otherwise make a new one */
	if(*outer_lexeme_list != NULL) 
	{
		lexeme_list = *outer_lexeme_list;
	}
	
	/* Since next_num_neg needs to persist over multiple whitespace delimited strings, 
       set it here if it is non-NULL */
	if(outer_next_num_neg != NULL)
	{
		next_num_neg = *outer_next_num_neg;
	}

	/* Make sure the buffer is filled with zeros - or '\0' characters. 
       Makes it easy to see where the number ends */
	memset(buf, 0, sizeof(buf));
	while(str[i] != '\0') /* The string from main() will be null terminated */
	{
		/* Make a new lexeme pointer to be filled */
		lexeme *new_lex = NULL;
		switch(str[i])
		{
			case '+':
				/* For an operator, create_lex() uses the character at buf[0] to create the lexeme entry */
				buf[0] = '+';
				/* create the new lexeme entry */
				new_lex = create_lex(buf, OP);
				/* expand the list */
				lexeme_list = resize_list(lexeme_list, list_pos);
				/* place the new lexeme in the list */
				lexeme_list[list_pos] = new_lex;
				/* increase the known list size */
				list_pos = list_pos+1;
				break;

			case '-':
				/* The lexing of "-" uses a trick to mean we don't need to parse negative numbers */
				buf[0] = '-';
				/* If it's the first in the list, this must mean the next number is negative */
				if(list_pos == 0)
				{
					next_num_neg = 1;
				}
				/* Otherwise, if it _follows_ an operator (e.g.: 2+-1) it must be a negative number */
				else if(list_pos > 0 && lexeme_list[list_pos-1]->type == OP)
				{
					/* Or possibly, a negated negative number, e.g.: 2----2 = 2+2 */
					if(next_num_neg == 0)
					{
						next_num_neg = 1;
					}
					else
					{
						next_num_neg = 0;
					}
				}
				/* If not, just treat it as a normal operator */
				else
				{
					new_lex = create_lex(buf, OP);
					lexeme_list = resize_list(lexeme_list, list_pos);
					lexeme_list[list_pos] = new_lex;
					list_pos = list_pos+1;
				}
				break;

			case '*':
				buf[0] = '*';
				new_lex = create_lex(buf, OP);
				lexeme_list = resize_list(lexeme_list, list_pos);
				lexeme_list[list_pos] = new_lex;
				list_pos = list_pos+1;
				break;

			case '/':
				buf[0] = '/';
				new_lex = create_lex(buf, OP);
				lexeme_list = resize_list(lexeme_list, list_pos);
				lexeme_list[list_pos] = new_lex;
				list_pos = list_pos+1;
				break;

			case '^':
				buf[0] = '^';
				new_lex = create_lex(buf, OP);
				lexeme_list = resize_list(lexeme_list, list_pos);
				lexeme_list[list_pos] = new_lex;
				list_pos = list_pos+1;
				break;

			case '%':
				buf[0] = '%';
				new_lex = create_lex(buf, OP);
				lexeme_list = resize_list(lexeme_list, list_pos);
				lexeme_list[list_pos] = new_lex;
				list_pos = list_pos+1;

				break;

			default:
				/* If it isn't a recognised operator, it should be a number (or a decimal point) */
				if(isdigit(str[i]) || str[i] == '.')
				{
					/* The number will continue throughout the string, so set a flag that we've started
					   to lex it */
					if(num_started == 0)
					{
						buf_pos = 0;
						num_started = 1;
					}
					/* Build up the buffer of characters to get converted to a double */
					buf[buf_pos] = str[i];
					buf_pos = buf_pos + 1;
					/* Readahead one - if the next character is a not a number or decimal, the 
					   number is probably complete. */
					if(!isdigit(str[i+1]) && str[i+1] != '.')
					{
						/* Create the new lexeme */
						new_lex = create_lex(buf, NUM);
						/* Negate if we detect it is a negative number */
						if(next_num_neg)
						{
							new_lex->val = -(new_lex->val);
							next_num_neg = 0;
						}
						/* Add it to a list */
						lexeme_list = resize_list(lexeme_list, list_pos);
						lexeme_list[list_pos] = new_lex;
						list_pos = list_pos+1;
						buf_pos = 0;
						memset(buf, 0, sizeof(buf));
						num_started = 0;
					}
				}
				else
				{
					/* If we have an unknown character in lexing, note it and quit */
					printf("Unknown character: %c\n", str[i]);
					return -1;
				}
				break;
		}
		i++;
	}
	/* Return the list, status of negation and the list position */
	*outer_lexeme_list = lexeme_list;
	*outer_next_num_neg = next_num_neg;
	return list_pos;
}


lexeme *create_lex(char *buf, int type)
{
	/* Create the new lexeme will all zeros to ensure consistent information */
	lexeme *new = calloc(1, sizeof(lexeme));
	/* If it's an operator, check the first character of the buffer classify the operator */
	if(type == OP)
	{
		new->type = OP;
		switch(buf[0])
		{
			case '+':
				new->func = PLUS;
				break;
			case '-':
				new->func = MINUS;
				break;
			case '/':
				new->func = DIV;
				break;
			case '*':
				new->func = MULT;
				break;
			case '^':
				new->func = POW;
				break;
			case '%':
				new->func = MOD;
				break;
			default:
				new->func = UNKNOWN;
				break;
		}
			
	}
	/* If it's a number convert the null-terminated buffer into a double */
	else if(type == NUM)
	{
		new->type = NUM;
		new->val = strtod(buf, NULL);
	}
	return new;
}

/* resize the list using the current list position */
lexeme **resize_list(lexeme **list, int size)
{
	/* simple realloc */
	list = realloc(list, (size_t) (size+1) * sizeof(lexeme**));
	list[size] = NULL;
	return list;
}
