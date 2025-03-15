/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:50:37 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/15 14:18:05 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

size_t	ft_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (*str)
	{
		len++;
		str++;
	}
	return (len);
}

void	ft_putstr_fd(char *str, int fd)
{
	size_t	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	while (len > 100)
	{
		write(fd, str, 100);
		str += 100;
		len -= 100;
	}
	write(fd, str, len);
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
			ft_putstr_fd("You entered: ", 1);
			ft_putstr_fd(line, 1);
			ft_putstr_fd("\n", 1);
			free(line);
		}
	}
	// echo "hello" | ./minishell
	// else
	// {
	// 	while ((line =) != NULL)
	// 	{
	// 	}
	// }
	return (0);
}