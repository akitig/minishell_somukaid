#include <readline/readline.h>
#include <stdio.h>

// char *readline (prompt)

int	main(void)
{
	char *prompt = "minishell >";
	char *ret = readline(prompt);
	printf("ret:%s\n", ret);
	return (0);
}