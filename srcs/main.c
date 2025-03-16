/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:50:37 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/17 00:17:24 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

typedef enum e_token_type
{
	TK_WORD,
	TK_OP,
	TK_EOF,
	TK_RESERVED
}						t_token_type;

typedef struct s_token	t_token;

typedef struct s_token
{
	t_token_type		type;
	char				*word;
	t_token				*next;

}						t_token;

t_token	*tokenize(char *line)
{
	t_token	*token1;
	t_token	*token2;

	(void)line;
	token1 = malloc(sizeof(t_token));
	token2 = malloc(sizeof(t_token));
	if (!token1 || !token2)
		return (NULL);
	token1->type = TK_WORD;
	token1->word = ft_strdup("hello");
	token1->next = token2;
	token2->type = TK_WORD;
	token2->word = ft_strdup("world");
	token2->next = NULL;
	return (token1);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->word);
		free(tmp);
	}
}

void	minishell(char *line)
{
	t_token	*token;
	t_token	*head;

	token = tokenize(line);
	head = token;
	// tokenを全て出力
	while (token)
	{
		printf("%s\n", token->word);
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