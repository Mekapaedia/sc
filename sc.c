#include "sc.h"

int main(int argc, char **argv)
{
	int i = 0;
	lexeme **lexeme_list = NULL;
	int list_pos = 0;
	double result = 0;

	for(i = 1; i < argc; i++)
	{
		list_pos = lex(argv[i], &lexeme_list, list_pos);
	}
		
	result = parse(lexeme_list, list_pos);

	printf("%g\n", result);

	return 0;
}
