#include "sc.h"

/* parse() uses a stack for the operators and the numbers to parse the incoming lexemes */
/* push() pushses a lexeme on the stack */
void push(lexeme *val, lexeme ***stack, int *num_stack);
/* pop() returns a lexeme from the stack and removes it from the stack */
lexeme *pop(lexeme ***stack, int *num_stack);
/* peek() returns a lexeme but doesn't remove it from the stack */
lexeme *peek(lexeme ***stack, int *num_stack);

/* parse takes in a (read-only) list of lexemes from main() and parses up to the list_pos */
double parse(lexeme **lexeme_list, int list_pos)
{
	int i = 0;
	lexeme **num_stack = NULL; /* num_stack is a stack of number lexemes */
	int num_stack_size = 0; /* size of the number stack */
	lexeme **op_stack = NULL; /* op_stack is the operator stack */
	int op_stack_size = 0; /* size of operator stack */

	/* Parse each lexeme (no backtracking because it's a simple algorithm */
	for(i = 0; i < list_pos; i++)
	{
		/* Start off with number type lexemes */
		if(lexeme_list[i]->type == NUM)
		{
			/* Peek at the top of the operator stack - if it is a "high priority" operator
               such as *, /, % or ^ we'll do the operation right away - order of operations */
			lexeme *check_lex = peek(&op_stack, &op_stack_size);
			
			/* Place the number on the stack */
			push(lexeme_list[i], &num_stack, &num_stack_size);
			/* If the operator found is a "high priority" operator */
			if(check_lex != NULL && check_lex->func != PLUS && check_lex->func != MINUS)
			{
				int op = 0;

				/* Remove the operator from the stack and compute the result with the new number */
				check_lex = pop(&op_stack, &op_stack_size);
				/* Find out the operator itself */
				op = check_lex->func;

				/* if the operator is the correct operator (redundant check) */
				if(op == DIV || op == MULT || op == POW || op == MOD)
				{
					/* Pop both the left and right side numbers of the operation */
					lexeme *divisor = pop(&num_stack, &num_stack_size);
					lexeme *dividend = pop(&num_stack, &num_stack_size);
					/* Check these numbers actually exist */
					if(divisor != NULL && dividend != NULL)
					{
						double result = 0;
						/* perform the operation */
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
						/* Free and delete the operator and the left side number */
						free(check_lex);
						free(dividend);
						/* Store the result in the right side number and place it back on the stack */
						divisor->val = result;
						push(divisor, &num_stack, &num_stack_size);
					}
					/* We are missing a number so the input lexeme list must be semantically incorrect */
					else
					{
						/* Bail */
						return NAN;
					}
				}
			}
		}
		/* If we have a number in the lexeme list, just push it on the operator stack */
		else if(lexeme_list[i]->type == OP)
		{
			push(lexeme_list[i], &op_stack, &op_stack_size);
		}
	}
	/* Once all the numbers and operators have been pushed and all the "high priority" operators
       have been evaluted, simply sum the result. Once we are out of operators, we know that
	   we have summed everything up if the input is semantically correct */
	while(op_stack_size > 0)
	{
		double result = 0;
		/* pop the right and left of the operation */
		lexeme *divisor = pop(&num_stack, &num_stack_size);
		lexeme *dividend = pop(&num_stack, &num_stack_size);
		/* pop the operator */
		lexeme *operator = pop(&op_stack, &op_stack_size);		
		int op = operator->func;

		/* Check we have enough numbers to support our operator */
		if(divisor == NULL || dividend == NULL)
		{
			/* If not bail */
			return NAN;
		}

		/* Add or subtract as needed */
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
		/* Free and delete the left side and the operator */
		free(operator);
		free(dividend);
		divisor->val = result;
		/* Place the right side back on the stack to be summed in the next loop */
		push(divisor, &num_stack, &num_stack_size);
	}
	/* Once all the operations have been completed, the result will be a single number
       at the top of the number stack */
	return pop(&num_stack, &num_stack_size)->val;
}

/* push on to one of the stacks and modify the given stack size */
void push(lexeme *val, lexeme ***stack, int *num_stack)
{
	/* ensure you have the correct brackets - otherwise you may get the next variable
	   instead of incrementing this one! */
	*num_stack = (*num_stack)+1;
	/* Realloc the stack to the new size */
	*stack = realloc(*stack, sizeof(lexeme**) * (size_t) (*num_stack));
	/* Place the new value at the top of the stack - num_stack is the size of the stack
       so the last position of the stack is size-1 */
	(*stack)[(*num_stack)-1] = val;
}

/* pop and peek are exactly the same except pop resizes the stack */
lexeme *pop(lexeme ***stack, int *num_stack)
{
	lexeme *pop_lexeme = NULL;
	/* If the stack has zero size obviously there is no lexeme to return */
	if((*num_stack) == 0)
	{
		return NULL;
	}
	/* Get the lexeme off the top of the stack */
	pop_lexeme = (*stack)[(*num_stack)-1];
	/* Shrink the stack */
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
