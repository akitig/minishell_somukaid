/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: somukaid <somukaid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 22:41:21 by somukaid          #+#    #+#             */
/*   Updated: 2025/04/05 22:41:55 by somukaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*extract_prefix(const char *str, int end)
/*
 * Extracts the substring from the beginning of the string up to the given index.
 *
 * @param str The original string.
 * @param end The index to end extraction (not inclusive).
 * @return The extracted prefix string, or NULL if end is 0.
 */
{
	if (end == 0)
		return (NULL);
	return (ft_substr(str, 0, end));
}

static char	*extract_key(const char *str, int *i)
/*
 * Extracts an alphanumeric environment variable key from the string.
 *
 * @param str The original string.
 * @param i Pointer to the current index; updated after key extraction.
 * @return The extracted key string.
 */
{
	int	start;

	start = ++(*i);
	while (ft_isalnum(str[*i]))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static char	*extract_suffix(const char *str, int start)
/*
 * Extracts the remaining substring starting from a given index.
 *
 * @param str The original string.
 * @param start The index to start extraction.
 * @return The extracted suffix string, or NULL if the rest is empty.
 */
{
	if (str[start] == '\0')
		return (NULL);
	return (ft_substr(str, start, ft_strlen(str) - start));
}

static char	*get_env_value(const char *key)
/*
 * Gets the value of an environment variable by key.
 *
 * @param key The environment variable key.
 * @return A duplicated string of the variable's value, or NULL if not found.
 */
{
	const char	*value;

	if (!key)
		return (NULL);
	value = getenv(key);
	if (value)
		return (ft_strdup(value));
	else
		return (NULL);
}

static char	*join_first_and_middle(char *first, char *middle)
{
	char	*tmp;

	tmp = NULL;
	if (first && middle)
	{
		tmp = ft_strjoin(first, middle);
		free(first);
		free(middle);
	}
	else if (first)
	{
		tmp = ft_strdup(first);
		free(first);
	}
	else if (middle)
	{
		tmp = ft_strdup(middle);
		free(middle);
	}
	return (tmp);
}

static char	*join_temp_and_last(char *tmp, char *last)
{
	char	*mix;

	mix = NULL;
	if (tmp && last)
	{
		mix = ft_strjoin(tmp, last);
		free(tmp);
		free(last);
	}
	else if (!tmp && last)
	{
		mix = ft_strdup(last);
		free(last);
	}
	else
		mix = tmp;
	return (mix);
}

char	*three_strjoin(char *first, char *middle, char *last)
{
	char	*tmp;
	char	*result;

	if (!first && !middle && !last)
		return (NULL);
	tmp = join_first_and_middle(first, middle);
	result = join_temp_and_last(tmp, last);
	return (result);
}

int	name2value_main(char **str, int i)
/*
 * Main routine to extract and replace a variable name with its value.
 *
 * @param str Pointer to the string containing a variable reference.
 * @param i Index where the '$' character is located.
 * @return Always returns 0.
 */
{
	char	*first;
	char	*key;
	char	*env;
	char	*last;
	char	*joined;

	first = extract_prefix(*str, i);
	key = extract_key(*str, &i);
	env = get_env_value(key);
	last = extract_suffix(*str, i);
	free(key);
	joined = three_strjoin(first, env, last);
	if (!joined)
		joined = ft_strdup("");
	free(*str);
	*str = joined;
	return (1);
}

int	question2return_value(char **str, int i)
/*
 * Handles the special variable `$?` by replacing it with the last exit code.
 *
 * @param str Pointer to the string.
 * @param i Index where the '$' character is located.
 * @return 3 on success, 1 on failure.
 */
{
	int	ret;

	ret = name2value_main(str, i);
	if (ret)
		return (1);
	else
		return (3);
}

int	name2value(char **str, int i)
/*
 * Checks for a valid environment variable after '$' and replaces it.
 *
 * @param str Pointer to the string.
 * @param i Current index to check.
 * @return 2 if replaced, 3 if `$?`, or 0 if not applicable.
 */
{
	int	ret;

	if ((*str)[i] == '$')
	{
		if (ft_isalnum((*str)[i + 1]))
		{
			ret = name2value_main(str, i);
			if (ret)
				return (1);
			else
				return (3);
		}
		if ((*str)[i + 1] == '?')
			return (question2return_value(str, i));
	}
	return (0);
}

static void	skip_single_quote(const char *str, int *i)
/*
 * Skips over characters inside a single quote pair.
 *
 * @param str The original string.
 * @param i Pointer to the current index; updated after skipping.
 */
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
}

static void	skip_double_quote_and_expand(char **str, int *i)
/*
 * Skips characters inside double quotes and performs variable expansion.
 *
 * @param str Pointer to the string.
 * @param i Pointer to the current index; updated after processing.
 */
{
	(*i)++;
	while ((*str)[*i] && (*str)[*i] != '"')
	{
		if (name2value(str, *i) == 2)
			continue ;
		(*i)++;
	}
}

int	dollar2env(t_node *node)
/*
 * Recursively processes a syntax tree node to replace all `$variables`.
 *
 * @param node Pointer to the root node.
 * @return Always returns 0.
 */
{
	char	*str;
	int		i;
	int		ret;

	if (!node)
		return (0);
	str = (char *)node->value;
	i = 0;
	while (str[i])
	{
		ret = name2value(&str, i);
		if (ret == 3)
			break ;
		if (ret == 2)
			continue ;
		if (str[i] == '\'')
			skip_single_quote(str, &i);
		else if (str[i] == '"')
			skip_double_quote_and_expand(&str, &i);
		i++;
	}
	node->value = (void *)str;
	dollar2env(node->left);
	dollar2env(node->right);
	return (0);
}

static void	remove_quote_at(char **str, char quote)
/*
 * Removes the first occurrence of a specific quote character from a string.
 *
 * @param str Pointer to the string.
 * @param quote The quote character to remove (' or ").
 */
{
	char	*pos;
	size_t	idx;
	char	*front;
	char	*back;
	char	*joined;

	pos = ft_strchr(*str, quote);
	idx = pos - *str;
	front = ft_substr(*str, 0, idx);
	back = ft_substr(*str, idx + 1, ft_strlen(*str) - idx - 1);
	joined = ft_strjoin(front, back);
	free(front);
	free(back);
	free(*str);
	*str = joined;
}

static void	remove_quotes_helper(char **str)
/*
 * Removes paired single or double quotes from the string.
 *
 * @param str Pointer to the string.
 */
{
	size_t	i;
	int		single;
	int		dbl;

	if (!str || !*str)
		return ;
	i = 0;
	single = 0;
	dbl = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '\'' && dbl == 0)
		{
			remove_quote_at(str, '\'');
			single = !single;
			i--;
		}
		else if ((*str)[i] == '"' && single == 0)
		{
			remove_quote_at(str, '"');
			dbl = !dbl;
			i--;
		}
		i++;
	}
}

void	remove_quotes(t_node **node)
/*
 * Recursively removes quotes from all nodes in the syntax tree.
 *
 * @param node Pointer to the root node.
 */
{
	if (!node || !*node)
		return ;
	remove_quotes(&(*node)->left);
	remove_quotes(&(*node)->right);
	remove_quotes_helper(&(*node)->value);
}

void	expansion(t_node **node)
/*
 * Performs full expansion of variables and quote removal on a node.
 *
 * @param node Pointer to the root node.
 */
{
	if (!node || !*node)
		return ;
	dollar2env(*node);
	remove_quotes(node);
}
