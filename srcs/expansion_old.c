/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_old.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: somukaid <somukaid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 22:41:21 by somukaid          #+#    #+#             */
/*   Updated: 2025/03/28 22:41:26 by somukaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*three_strjoin(char *first_str, char *env_str, char *second_str)
{
	char	*mix_str;
	char	*tmp_str;

	mix_str = NULL;
	tmp_str = NULL;
	if (NULL == first_str && NULL == env_str && NULL == second_str)
		return (NULL);
	if (NULL != first_str && NULL != env_str)
	{
		tmp_str = ft_strjoin(first_str, env_str);
		if (NULL == tmp_str)
			return (NULL);
		free(first_str);
		free(env_str);
	}
	else if (NULL == first_str && NULL == env_str)
	{
		tmp_str = NULL;
	}
	else if (NULL == first_str)
	{
		tmp_str = ft_strdup(env_str);
		if (NULL == tmp_str)
			return (NULL);
		free(env_str);
	}
	else if (NULL == env_str)
	{
		tmp_str = ft_strdup(first_str);
		if (NULL == tmp_str)
			return (NULL);
		free(first_str);
	}
	if (NULL != tmp_str && NULL != second_str)
	{
		mix_str = ft_strjoin(tmp_str, second_str);
		if (NULL == mix_str)
			return (NULL);
		free(tmp_str);
		free(second_str);
	}
	else if (NULL == tmp_str)
	{
		mix_str = ft_strdup(second_str);
		if (NULL == mix_str)
			return (NULL);
		free(second_str);
	}
	else if (NULL == second_str)
		mix_str = tmp_str;
	return (mix_str);
}

int	name2value_main(char **str, int i)
{
	char	*first_str;
	char	*env_words_and_mix_str;	
	char	*env_str;
	char	*second_str;
	int		start;

	if (0 != i)
	{
		first_str = ft_substr(*str, 0, i);
		if (NULL == first_str)
			return (1);
	}
	else
		first_str = NULL;
	++i;
	start = i;
	while (ft_isalnum((*str)[i]))
		++i;
	if (i != start)
	{
		env_words_and_mix_str = ft_substr(*str, start, i - start);
		if (NULL == env_words_and_mix_str)
			return (1);
		if (NULL != getenv(env_words_and_mix_str))
		{
			env_str = ft_strdup(getenv(env_words_and_mix_str));
			if (NULL == env_str)
				return (1);
			free(env_words_and_mix_str);
		}
		else
			env_str = NULL;
	}
	else
	{
		env_words_and_mix_str = NULL;
		env_str = NULL;
	}
	if ('\0' != (*str)[i])
	{
		second_str = ft_substr(*str, i, (int)ft_strlen(*str) - i);
		if (NULL == second_str)
			return (1);
	}
	else
		second_str = NULL;
	env_words_and_mix_str = three_strjoin(first_str, env_str, second_str);
	if (NULL == env_words_and_mix_str)
		env_words_and_mix_str = ft_strdup("\0");
	free(*str);
	*str = env_words_and_mix_str;
	return (0);
}

int	question2return_value(char **str, int i)
{
	int	ret;

	ret = name2value_main(str, i);
	if (1 == ret)
		return (1);
	return (3);
}

int	name2value(char **str, int i)
{
	int	ret;

	if ('$' == (*str)[i] && ft_isalnum((*str)[i + 1]))
	{
		ret = name2value_main(str, i);
		if (1 == ret)
			return (1);
		return (2);
	}
	if ('$' == (*str)[i])
	{
		if ('?' == (*str)[i + 1])
		{
			ret = question2return_value(str, i);
			return (2);
		}
	}
	return (0);
}

int	dollar2env(t_node *node)
{
	char	*str;
	int		i;
	int		ret;

	if (node == NULL)
	{
		return (0);
	}
	str = (char *)node->value;
	i = 0;
	while (str[i] != '\0')
	{
		ret = name2value(&str, i);
		if (ret == 3)
			break ;
		if (ret == 2)
			continue ;
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		if (str[i] == '"')
		{
			i++;
			while (str[i] != '"')
			{
				ret = name2value(&str, i);
				if (ret == 2)
					continue ;
				i++;
			}
		}
		i++;
	}
	node->value = (void *)str;
	if (node->left)
		dollar2env(node->left);
	if (node->right)
		dollar2env(node->right);
	return (0);
}

void	remove_quotes_single(char **str)
{
	char	*tmp_str;
	char	*front_str;
	char	*back_str;
	size_t	i;

	if (*str == NULL)
		return ;
	tmp_str = ft_strchr(*str, '\'');
	i = tmp_str - *str;
	front_str = ft_substr(*str, 0, i);
	back_str = ft_substr(*str, i + 1, ft_strlen(*str) - i - 1);
	free(*str);
	*str = ft_strjoin(front_str, back_str);
	free(front_str);
	free(back_str);
	return ;
}

void	remove_quotes_double(char **str)
{
	char	*tmp_str;
	char	*front_str;
	char	*back_str;
	size_t	i;

	if (*str == NULL)
		return ;
	tmp_str = ft_strchr(*str, '\"');
	i = tmp_str - *str;
	front_str = ft_substr(*str, 0, i);
	back_str = ft_substr(*str, i + 1, ft_strlen(*str) - i - 1);
	free(*str);
	*str = ft_strjoin(front_str, back_str);
	free(front_str);
	free(back_str);
	return ;
}

void	remove_quotes_helper(char **str)
{
	size_t	i;
	int		single_flag;
	int		double_flag;

	if (*str == NULL)
		return ;
	i = 0;
	single_flag = 0;
	double_flag = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '\'' && double_flag == 0)
		{
			remove_quotes_single(str);
			if (single_flag == 0)
				single_flag = 1;
			else
				single_flag = 0;
			i--;
		}
		else if ((*str)[i] == '\"' && single_flag == 0)
		{
			remove_quotes_double(str);
			if (double_flag == 0)
				double_flag = 1;
			else
				double_flag = 0;
			i--;
		}
		i++;
	}
	return ;
}

void	remove_quotes(t_node **node)
{
	if (node == NULL || *node == NULL)
		return ;
	remove_quotes(&((*node)->left));
	if (&((*node)->right))
		remove_quotes(&((*node)->right));
	remove_quotes_helper(&((*node)->value));
	return ;
}

void	expansion(t_node **node)
{
	dollar2env(*node);
	print_node(*node);
	remove_quotes(node);
	return ;
}