/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:41:18 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/21 04:41:55 by akunimot         ###   ########.fr       */
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
typedef enum
{
    delimiter,
    pipe_c,
    command,
    argument,
    redirection,
}       NodeType;

typedef struct  s_node
{
    NodeType        type;
    struct s_node   *left;
    struct s_node   *right;
    char            *text;
}               t_node;

typedef enum e_token_type
{
	TK_WORD,
	TK_OP,
	TK_EOF,
	//	TK_RESERVED
}						t_token_type;

typedef struct s_token	t_token;

typedef struct s_token
{
	t_token_type		type;
	char				*word;
	t_token				*next;

}						t_token;

t_node    *parser(t_token *token);

#endif // MINISHELL_H