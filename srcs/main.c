/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:50:37 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/28 14:12:14 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	minishell(char *line)
{
	t_token	*token;
	t_token	*head;
	t_node	*node;

	/* Pass address of line so that tokenize() can reallocate it if needed */
	token = tokenize(&line);
	if (!token)
	{
		free(line);
		return ;
	}
	node = parser(token);
	head = token;
	/* Debug output: print all tokens */
	while (token)
	{
		if (token->word)
			printf("%s\n", token->word);
		else
			printf("(null)\n");
		printf("%d\n", token->type);
		token = token->next;
	}
	print_node(node);
	free_tokens(head);
	free(line);
}

int	main(int ac, char **av)
{
	char	*line;
	char	*prompt;

	prompt = "minishell > ";
	(void)ac;
	(void)av;
	if (isatty(fileno(stdin)))
	{
		while ((line = readline(prompt)))
		{
			if (*line)
				add_history(line);
			minishell(line);
		}
	}
	else
	{
		while ((line = get_next_line(0)) != NULL)
		{
			ft_putstr_fd(line, 1);
			free(line);
		}
	}
	return (0);
}
