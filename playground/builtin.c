/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: somukaid <somukaid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:19:21 by somukaid          #+#    #+#             */
/*   Updated: 2025/04/22 13:37:40 by somukaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

int	ft_exit(int num)
{
	printf("exit\n");
	return (num);
}

void	pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return ;
	printf("%s\n", pwd);
	free(pwd);
}

void	echo(char *echo_str)
{
	char	*str;

	str = echo_str + 5;
	if (echo_str == NULL)
		return ;
	if (!ft_strncmp(str, "-n", 2))
		printf("%s", str + 3);
	else
		printf("%s\n", str);
	return ;
}

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

void	env(t_list *env)
{
	t_list	*tmp;

	if (env == NULL)
		return ;
	tmp = env;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	return ;
}

// delete and add environ
void	export(t_list *env, char *key)
{
	t_list	*tmp;
	char	*key_name;
	size_t	len;

	if (env == NULL || key == NULL)
		return ;
	tmp = env;
	len = ft_strchr(key, '=') - key;
	key_name = ft_substr(key, 0, len);
	if (key_name == NULL)
		return ;
	while (tmp)
	{
		if (!ft_strncmp(key_name, (char *)tmp->content, len))
		{
			free(tmp->content);
			tmp->content = (void *)ft_strdup(key);
			free(key_name);
			return ;
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		if (ft_strchr(key, '='))
		{
			tmp = ft_lstnew(ft_strdup(key));
			ft_lstadd_back(&env, tmp);
		}
	}
	free(key_name);
	return ;
}

void	unset(t_list **env, char *key)
{
	t_list	*tmp;

	if (env == NULL || *env == NULL || key == NULL)
		return ;
	if (!ft_strncmp((char *)(*env)->content, key, ft_strlen(key)) && \
		(size_t)(ft_strchr((char *)(*env)->content, '=') - (char *)(*env)->content) == ft_strlen(key))
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->content);
		free(tmp);
		unset(env, key);
	}
	else
		unset(&(*env)->next, key);
}

int	main(int ac, char **av, char **envp)
{
	int		fork_ret;
	int		wait_id;
	t_list	*env_list;

	(void)ac;
	(void)av;
	env_list = env_init(envp);
	unset(&env_list, "SESSION_MANAGER");
	fork_ret = fork();
	if (!fork_ret)
	{
		env(env_list);
		ft_lstclear(&env_list, free);
		exit(0);
	}
	else
		wait(&wait_id);
	ft_lstclear(&env_list, free);
	return (ft_exit(5));
}
