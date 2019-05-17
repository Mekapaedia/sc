#include "sc.h"

void push(lexeme *val, lexeme ***stack, int *num_stack);
lexeme *pop(lexeme ***stack, int *num_stack);
lexeme *peek(lexeme ***stack, int *num_stack);

double parse(lexeme **lexeme_list, int list_pos)
{
	int i = 0;
	lexeme **num_stack = NULL;
	int num_stack_size = 0;
	lexeme **op_stack = NULL;
	int op_stack_size = 0;

	for(i = 0; i < list_pos; i++)
	{
		if(lexeme_list[i]->type == NUM)
		{
			lexeme *check_lex = peek(&op_stack, &op_stack_size);
			
			push(lexeme_list[i], &num_stack, &num_stack_size);
			if(check_lex != NULL && check_lex->func != PLUS && check_lex->func != MINUS)
			{
				int op = 0;

				check_lex = pop(&op_stack, &op_stack_size);
				op = check_lex->func;

				if(op == DIV || op == MULT || op == POW || op == MOD)
				{
					lexeme *divisor = pop(&num_stack, &num_stack_size);
					lexeme *dividend = pop(&num_stack, &num_stack_size);
					if(divisor != NULL && dividend != NULL)
					{
						double result = 0;
						switch(op)
						{
							case DIV:
								result = (dividend->val) / (divisor->val);
								break;
							case MULT:
								result = (dividend->val) * (divisor->val);
								break;
							case POW:
								result = pow(dividend->val, divisor->val);
								break;
							case MOD:
								result = fmod((dividend->val), (divisor->val));
								break;
							default:
								break;
						}
						free(check_lex);
						free(dividend);
						divisor->val = result;
						push(divisor, &num_stack, &num_stack_size);
					}
				}
			}
		}
		else if(lexeme_list[i]->type == OP)
		{
			push(lexeme_list[i], &op_stack, &op_stack_size);
		}
	}
	while(op_stack_size > 0)
	{
		double result = 0;
		lexeme *divisor = pop(&num_stack, &num_stack_size);
		lexeme *dividend = pop(&num_stack, &num_stack_size);
		lexeme *operator = pop(&op_stack, &op_stack_size);		
		int op = operator->func;

		switch(op)
		{
			case PLUS:
				result = (dividend->val) + (divisor->val);
				break;
			case MINUS:
				result = (dividend->val) - (divisor->val);
				break;
			default:
				break;
		}
		free(operator);
		free(dividend);
		divisor->val = result;
		push(divisor, &num_stack, &num_stack_size);
	}
	return pop(&num_stack, &num_stack_size)->val;
}

void push(lexeme *val, lexeme ***stack, int *num_stack)
{
	*num_stack = (*num_stack)+1;
	*stack = realloc(*stack, sizeof(lexeme**) * (size_t) (*num_stack));
	(*stack)[(*num_stack)-1] = val;
}

lexeme *pop(lexeme ***stack, int *num_stack)
{
	lexeme *pop_lexeme = NULL;
	if((*num_stack) == 0)
	{
		return NULL;
	}
	pop_lexeme = (*stack)[(*num_stack)-1];
	*num_stack = (*num_stack)-1;
	*stack = realloc(*stack, sizeof(lexeme**) * (size_t) *num_stack);
	return pop_lexeme;
}

lexeme *peek(lexeme ***stack, int *num_stack)
{
	lexeme *peek_lexeme = NULL;
	if((*num_stack) == 0)
	{
		return NULL;
	}
	peek_lexeme = (*stack)[(*num_stack)-1];
	return peek_lexeme;
}
