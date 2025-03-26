/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:50:37 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/26 23:16:26 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	minishell(char *line)
{
	t_token	*token;
	t_token	*head;
	t_node	*node;

	(void)node;
	token = tokenize(line);
	node = parser(token);
	head = token;
	// tokenを全て出力
	while (token)
	{
		printf("%s\n", token->word);
		printf("%d\n", token->type);
		token = token->next;
	}
	free_tokens(head);
}

int	main(int ac, char **av)
{
	char *line;
	char *prompt = "minishell > ";

	(void)ac;
	(void)av;

	// prompt に打たれた文字列をそのまま表示する

	// ./minishell
	if (isatty(fileno(stdin)))
	{
		while ((line = readline(prompt)))
		{
			if (*line)
				add_history(line);
			minishell(line);
			free(line);
		}
	}
	//	echo "hello" | ./minishell
	else
	{
		while ((line = get_next_line(0)) != NULL)
		{
			// TODO:ttyって使うのかなー　存在するってなんだろー
			// echo "/usr/bin/cat main.c" | /bin/bash

			ft_putstr_fd(line, 1);
			free(line);
		}
	}
	return (0);
}