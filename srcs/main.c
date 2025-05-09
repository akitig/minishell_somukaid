/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: somukaid <somukaid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:50:37 by akunimot          #+#    #+#             */
/*   Updated: 2025/04/23 16:31:38 by somukaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*env_init(char **envp)
{
	size_t	i;
	t_list	*tmp;
	t_list	*head;

	i = 0;
	head = NULL;
	if (envp == NULL || *envp == NULL)
		return (NULL);
	while (envp[i])
	{
		tmp = ft_lstnew(ft_strdup(envp[i]));
		ft_lstadd_back(&head, tmp);
		i++;
	}
	return (head);
}

void	minishell(char *line, t_list *envp_list)
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
	expansion(&node);
	printf("----- After expansion -----\n");
	print_node(node);
	printf("----- op_command -----\n");
	op_command(node, envp_list);
	free_tokens(head);
	free(line);
}

int	main(int ac, char **av, char **en)
{
	char	*line;
	char	*prompt;
	t_list	*envp_list;

	prompt = "minishell > ";
	(void)ac;
	(void)av;
	if (isatty(fileno(stdin)))
	{
		envp_list = env_init(en);
		while ((line = readline(prompt)))
		{
			if (*line)
				add_history(line);
			minishell(line, envp_list);
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
