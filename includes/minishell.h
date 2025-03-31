/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:41:18 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/28 14:10:57 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Custom Libraries
# include "../libft/libft.h"

// Standard Libraries
# include <readline/history.h>
# include <readline/readline.h>

// Prototypes
typedef enum e_node_type
{
	// ND_SEMICOLON,
	ND_PIPE,
	ND_COMMAND,
	ND_SEQ,
	ND_REDIR,
	ND_FILE,
}						t_node_type;

typedef struct s_node
{
	char				*value;
	int					type;
	struct s_node		*left;
	struct s_node		*right;

}						t_node;

typedef enum e_token_type
{
	TK_WORD,
	TK_OP,
	TK_SEMICOLON,
	TK_REDIR,
	//	TK_RESERVED
}						t_token_type;

typedef struct s_token	t_token;

typedef struct s_token
{
	t_token_type		type;
	char				*word;
	t_token				*next;

}						t_token;

// ------- tokenize --------
// tokenize.c
t_token					*tokenize(char **line);
void					free_tokens(t_token *token);

// ft_token.c
t_token					*ft_lstlast_token(t_token *token);
void					ft_lstadd_back_token(t_token **token, t_token *new);

// ------- parser -------
// parser.c
t_node					*parser(t_token *token);
void					print_node(t_node *node);

// ft_parser.c
t_node					*ft_lstlast_node(t_node *node);
void					ft_lstadd_back_node(t_node **node, t_node *new);
t_node					*ft_lstnew_node(char *text);
void					ft_lstadd_front_node(t_node **node, t_node *new);

// ------- expansion -------
// expansion.c
void    expansion(t_node **node);

#endif // MINISHELL_H