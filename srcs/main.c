/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: somukaid <somukaid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:50:37 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/18 12:16:59 by somukaid         ###   ########.fr       */
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

// t_token型のft_lstlast
t_token	*ft_lstlast_token(t_token *token)
{
	if (token == NULL)
		return (NULL);
	while (token->next != NULL)
		token = token->next;
	return (token);
}

// t_token型のft_lstadd_back
void	ft_lstadd_back_token(t_token **token, t_token *new)
{
	if (token == NULL || new == NULL)
		return ;
	if (*token == NULL)
		*token = new;
	else
		ft_lstlast_token(*token)->next = new;
}

// スペースの区切りごとにtmp_token->wordを作成
// | & ; ( ) < > space tab のスペースにしか対応できてない
// ls>output.txt とか echo hello|cat とかは今は無理
// ls>output.txt を分割したらどうなる？ ls > output.txt になるのか？ ls output.txtになっちゃうと引数なのかリダイレクトなのかわからんしな
// 要相談
void    split_token(char *input, int *index, int *word_first, char **tmp_token_word)
{
        if (input[*index] && '\0' != input[*index])
        {
                while (' ' == input[*index] && '\0' != input[*index])
                        ++(*index);
        }
		if (input[*index] == '\0')
			return ;
        *word_first = *index;
        while (' ' != input[*index] && '\0' != input[*index])
        {
                if ('"' == input[*index])
                {
                        ++(*index);
                        while ('"' != input[*index])
                                ++(*index);
                }
                if ('\'' == input[*index])
                {
                        ++(*index);
                        while ('\'' != input[*index])
                                ++(*index);
                }
                ++(*index);
        }
        *tmp_token_word = ft_substr(input, *word_first, *index - *word_first);
}

// トークンのタイプを調べてtmp_token->typeに代入
void	assign_type(t_token **tmp_token)
{
	char	op_list[9][3] = {"||", "&", "&&", ";",  ";;", "(",  ")", "|", "|&"};
	char	reserved_list[21][9] = {"!", "case", "do", "done", "elif", "else", "esac", "fi", "for", "function", "if", "in", "select", "then", "until", "while", "{", "}", "time", "[[", "]]"};
	size_t	i;
	size_t	j;

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
	j = 0;
	while (j < 21)
	{
		if (!ft_strncmp((*tmp_token)->word, reserved_list[j], ft_strlen(reserved_list[j])))
		{
			(*tmp_token)->type = TK_RESERVED;
			break ;
		}
		++j;
	}
	if (i == 9 && j == 21)
		(*tmp_token)->type = TK_WORD;
	// TK_EOFってどんなトークンにつくんや…？
}

// tokenizeテスト１
t_token	*tokenize(char *line)
{
	t_token  *token;
    t_token  *tmp_token;
    int             index;
    int             word_first;
	char    *tmp_str;

    token = NULL;
    index = 0;
    word_first = 0;
    tmp_str = NULL;
    while ('\0' != line[index])
    {
		tmp_token = (t_token *)malloc(sizeof(t_token));
		tmp_token->word = NULL;
		tmp_token->next = NULL;
        split_token(line, &index, &word_first, &(tmp_token->word)); // トークンごとにtmp_token->wordに代入
        if (NULL == tmp_token->word)
		{
			free(tmp_token);
            return (token);
		}
		assign_type(&tmp_token);
        ft_lstadd_back_token(&token, tmp_token); //tokenにり
    }
    return (token);
}

/*
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