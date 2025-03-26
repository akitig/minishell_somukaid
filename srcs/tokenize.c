/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 02:39:38 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/26 23:26:37 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_metachar(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')' || c == '<'
		|| c == '>' || c == ' ')
		return (1);
	return (0);
}
/*
** skip_quote: Advance the index until the matching quote is found.
*/
static void	skip_quote(char *input, int *index, char quote)
{
	++(*index);
	while (input[*index] && input[*index] != quote)
		++(*index);
	if (input[*index] == quote)
		++(*index);
}

/*
** split_token: Extract a token (operator or word) from the input string.
*/
static void	split_token(char *input, int *index, int *word_start,
		char **tmp_token_word)
{
	while (input[*index] == ' ')
		++(*index);
	if (input[*index] == '\0')
		return ;
	if (is_metachar(input[*index]))
	{
		if ((input[*index + 1] == '>' || input[*index + 1] == '<')
			&& input[*index + 1] != '\0')
		{
			*tmp_token_word = ft_substr(input, *index, 2);
			++(*index);
		}
		else
			*tmp_token_word = ft_substr(input, *index, 1);
		++(*index);
		return ;
	}
	*word_start = *index;
	while (input[*index] && !is_metachar(input[*index]))
	{
		if (input[*index] == '"' || input[*index] == '\'')
			skip_quote(input, index, input[*index]);
		else
			++(*index);
	}
	*tmp_token_word = ft_substr(input, *word_start, *index - *word_start);
}

// operator一覧
char	**make_data(void)
{
	char	**op_list;

	op_list = (char **)malloc(sizeof(char *) * 9);
	op_list[0] = "||";
	op_list[1] = "&";
	op_list[2] = "&&";
	op_list[3] = ";";
	op_list[4] = ";;";
	op_list[5] = "(";
	op_list[6] = ")";
	op_list[7] = "|";
	op_list[8] = "|&";
	return (op_list);
}

// トークンのタイプを調べてtmp_token->typeに代入
void	assign_type(t_token **tmp_token)
{
	char	**op_list;
	size_t	i;

	op_list = make_data();
	if (!op_list)
		return ;
	i = 0;
	while (i < 9)
	{
		if (!ft_strncmp((*tmp_token)->word, op_list[i], ft_strlen(op_list[i])))
		{
			(*tmp_token)->type = TK_OP;
			break ;
		}
		++i;
	}
	if (i == 9)
		(*tmp_token)->type = TK_WORD;
	if (((*tmp_token)->word)[0] == ';')
		(*tmp_token)->type = TK_EOF; //今のところ、明示的に;があるときだけ
}

// tokenizeテスト１
t_token	*tokenize(char *line)
{
	t_token	*token;
	t_token	*tmp_token;
	int		index;
	int		word_first;

	token = NULL;
	index = 0;
	word_first = 0;
	while ('\0' != line[index])
	{
		tmp_token = (t_token *)malloc(sizeof(t_token));
		if (!tmp_token)
			return (token);
		tmp_token->word = NULL;
		tmp_token->next = NULL;
		split_token(line, &index, &word_first, &(tmp_token->word));
		// トークンごとにtmp_token->wordに代入
		if (tmp_token->word == NULL)
		{
			free(tmp_token);
			return (token);
		}
		assign_type(&tmp_token);
		ft_lstadd_back_token(&token, tmp_token); // tokenにり
	}
	return (token);
}
void	free_tokens(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->word);
		free(tmp);
	}
}