#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

// char *readline (prompt)

int	main(void)
{
	char *prompt = "minishell >";
	while (1)
	{

	//	char *prompt = "minishell >";
		char *ret = readline(prompt);
		printf("ret:%s\n", ret);
		if (ret == NULL)
		{
			printf("minishell is end.\n");
			break ;
		}
		add_history(ret);
		//free(ret);
	}
	return (0);
}