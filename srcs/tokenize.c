/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 02:39:38 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/28 14:05:59 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h> /* for fprintf() */

static int	is_metachar(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')' || c == '<'
		|| c == '>' || c == ' ')
		return (1);
	return (0);
}

/*
** skip_quote: Advance index until matching quote is found.
** In double quotes, '\' escape is supported.
** Returns 0 on success, or -1 if no matching quote is found.
*/
static int	skip_quote(char *input, int *index, char quote)
{
	++(*index);
	while (input[*index] && input[*index] != quote)
	{
		if (quote == '"' && input[*index] == '\\')
			++(*index);
		++(*index);
	}
	if (input[*index] != quote)
	{
		fprintf(stderr, "Error: Unclosed quote detected\n");
		return (-1);
	}
	++(*index);
	return (0);
}

/*
** split_token: Extract one token (operator or word) from input.
** Handles redirection operators (">", ">>", "<", "<<") and escape
** characters (using '\' to treat next char as literal).
** Returns 0 on success, or -1 on error (e.g. unclosed quote).
*/
static int	split_token(char *input, int *index, int *word_start,
		char **tmp_token_word)
{
	while (input[*index] == ' ')
		++(*index);
	if (input[*index] == '\0')
		return (0);
	if (is_metachar(input[*index]))
	{
		/* Handle redirection operators: >> or << */
		if ((input[*index] == '>' || input[*index] == '<') && input[*index + 1]
			&& input[*index + 1] == input[*index])
		{
			*tmp_token_word = ft_substr(input, *index, 2);
			*index += 2;
		}
		else
		{
			*tmp_token_word = ft_substr(input, *index, 1);
			++(*index);
		}
		return (0);
	}
	*word_start = *index;
	while (input[*index] && !is_metachar(input[*index]))
	{
		if (input[*index] == '\\')
		{
			++(*index);
			if (input[*index])
				++(*index);
		}
		else if (input[*index] == '"' || input[*index] == '\'')
		{
			if (skip_quote(input, index, input[*index]) == -1)
				return (-1);
		}
		else
			++(*index);
	}
	*tmp_token_word = ft_substr(input, *word_start, *index - *word_start);
	return (0);
}

/*
** make_data: Returns an array of operator strings.
*/
char	**make_data(void)
{
	char	**op_list;

	op_list = (char **)malloc(sizeof(char *) * 13);
	if (!op_list)
		return (NULL);
	op_list[0] = "||";
	op_list[1] = "&";
	op_list[2] = "&&";
	op_list[3] = ";";
	op_list[4] = ";;";
	op_list[5] = "(";
	op_list[6] = ")";
	op_list[7] = "|";
	op_list[8] = "|&";
	op_list[9] = "<";
	op_list[10] = ">";
	op_list[11] = "<<";
	op_list[12] = ">>";
	return (op_list);
}

/*
** assign_type: Set token type based on word.
*/
void	assign_type(t_token **tmp_token)
{
	char	**op_list;
	size_t	i;

	op_list = make_data();
	if (!op_list)
		return ;
	i = 0;
	while (i < 13)
	{
		if (!ft_strncmp((*tmp_token)->word, op_list[i], ft_strlen(op_list[i])
				+ 1))
		{
			if (i >= 9)
				(*tmp_token)->type = TK_REDIR;
			else
				(*tmp_token)->type = TK_OP;
			break ;
		}
		++i;
	}
	if (i == 13)
		(*tmp_token)->type = TK_WORD;
	if (((*tmp_token)->word)[0] == ';')
		(*tmp_token)->type = TK_SEMICOLON;
	free(op_list);
}

/*
** tokenize: Splits the input line into a linked list of tokens.
** If an error is detected (e.g. unclosed quote), frees allocated tokens
** and returns NULL.
*/
t_token	*tokenize(char *line)
{
	t_token	*token;
	t_token	*tmp_token;
	int		index;
	int		word_first;
	int		ret;

	token = NULL;
	index = 0;
	word_first = 0;
	while (line[index] != '\0')
	{
		tmp_token = (t_token *)malloc(sizeof(t_token));
		if (!tmp_token)
			return (token);
		tmp_token->word = NULL;
		tmp_token->next = NULL;
		ret = split_token(line, &index, &word_first, &(tmp_token->word));
		if (ret == -1)
		{
			free(tmp_token->word);
			free(tmp_token);
			free_tokens(token);
			return (NULL);
		}
		if (!tmp_token->word || tmp_token->word[0] == '\0')
		{
			free(tmp_token->word);
			free(tmp_token);
			continue ;
		}
		assign_type(&tmp_token);
		ft_lstadd_back_token(&token, tmp_token);
	}
	return (token);
}

/*
** free_tokens: Frees the token list.
*/
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
