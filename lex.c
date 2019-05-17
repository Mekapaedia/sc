
#include "sc.h"

lexeme *create_lex(char *buf, int type);
lexeme **resize_list(lexeme **list, int size);

int lex(char *str, lexeme ***outer_lexeme_list, int list_pos)
{
	int i = 0;
	int num_started = 0;
	char buf[64];
	int buf_pos = 0;
	int next_num_neg = 0;
	lexeme **lexeme_list = NULL;
	if(*outer_lexeme_list != NULL)
	{
		lexeme_list = *outer_lexeme_list;
	}

	memset(buf, 0, sizeof(buf));
	while(str[i] != '\0')
	{
		lexeme *new_lex = NULL;
		switch(str[i])
		{
			case '+':
				buf[0] = '+';
				new_lex = create_lex(buf, OP);
				lexeme_list = resize_list(lexeme_list, list_pos);
				lexeme_list[list_pos] = new_lex;
				list_pos = list_pos+1;
				break;

			case '-':
				buf[0] = '-';
				if(list_pos == 0)
				{
					next_num_neg = 1;
				}
				else if(list_pos > 0 && lexeme_list[list_pos-1]->type == OP)
				{
					if(next_num_neg == 0)
					{
						next_num_neg = 1;
					}
					else
					{
						next_num_neg = 0;
					}
				}
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
				if(isdigit(str[i]) || str[i] == '.')
				{
					if(num_started == 0)
					{
						buf_pos = 0;
						num_started = 1;
					}
					buf[buf_pos] = str[i];
					buf_pos = buf_pos + 1;
					if(!isdigit(str[i+1]) && str[i+1] != '.')
					{
						new_lex = create_lex(buf, NUM);
						if(next_num_neg)
						{
							new_lex->val = -(new_lex->val);
							next_num_neg = 0;
						}
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
					printf("unknown: %c\n", str[i]);
				}
				break;
		}
		i++;
	}
	*outer_lexeme_list = lexeme_list;
	return list_pos;
}

lexeme *create_lex(char *buf, int type)
{
	lexeme *new = calloc(1, sizeof(lexeme));
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
	else if(type == NUM)
	{
		new->type = NUM;
		new->val = strtod(buf, NULL);
	}
	return new;
}

lexeme **resize_list(lexeme **list, int size)
{
	list = realloc(list, (size_t) (size+1) * sizeof(lexeme**));
	list[size] = NULL;
	return list;
}
