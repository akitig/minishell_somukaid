/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 02:34:07 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/28 13:59:20 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_node(t_node *node)
{
	if (!node)
		return ;
	print_node(node->left);
	if (node->value)
		printf("node: %s, type: %d\n", node->value, node->type);
	print_node(node->right);
}

static t_node	*new_ast_node(char *value, int type)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/*
** Joins successive TK_WORD tokens into one command string.
** Updates *tok to the first token after the command.
*/
static char	*join_command_words(t_token **tok)
{
	int		total_len;
	t_token	*current;
	int		index;
	char	*cmd;

	total_len = 0;
	current = *tok;
	while (current && current->type == TK_WORD)
	{
		if (current->word)
			total_len += ft_strlen(current->word);
		total_len++; /* for space or terminator */
		current = current->next;
	}
	if (total_len == 0)
		return (NULL);
	cmd = malloc(sizeof(char) * total_len);
	if (!cmd)
		return (NULL);
	index = 0;
	current = *tok;
	while (current && current->type == TK_WORD)
	{
		if (current->word)
		{
			ft_strcpy(cmd + index, current->word);
			index += ft_strlen(current->word);
		}
		if (current->next && current->next->type == TK_WORD)
			cmd[index++] = ' ';
		current = current->next;
	}
	cmd[index] = '\0';
	*tok = current;
	return (cmd);
}

/*
** Parses a command from successive tokens.
** After parsing the command name and arguments, if a redirection token
** (TK_REDIR) is found, a redirection AST node (ND_REDIR) is created,
** with left child holding the current command and right child holding
** the redirection target (ND_FILE node).
*/
static t_node	*parse_command(t_token **tok)
{
	char	*cmd_str;
	t_node	*node;
	t_node	*redir_node;
	t_node	*file_node;
	char	*redir_op;

	if (!tok || !*tok)
		return (NULL);
	if ((*tok)->type != TK_WORD)
		return (NULL);
	cmd_str = join_command_words(tok);
	if (!cmd_str)
		return (NULL);
	node = new_ast_node(cmd_str, ND_COMMAND);
	while (*tok && (*tok)->type == TK_REDIR)
	{
		/* Save redirection operator token value */
		redir_op = (*tok)->word;
		*tok = (*tok)->next;
		if (!*tok || (*tok)->type != TK_WORD)
			break ;
		/* Create file node for the redirection target */
		file_node = new_ast_node((*tok)->word, ND_FILE);
		*tok = (*tok)->next;
		redir_node = new_ast_node(redir_op, ND_REDIR);
		redir_node->left = node;
		redir_node->right = file_node;
		node = redir_node;
	}
	return (node);
}

/*
** Parses a pipeline: command ('|' command)* .
** After a pipe operator, call parse_command() to get the next command.
*/
static t_node	*parse_pipe(t_token **tok)
{
	t_node	*left;
	t_node	*node;
	t_node	*right;

	left = parse_command(tok);
	if (!left)
		return (NULL);
	while (*tok && (*tok)->type == TK_OP)
	{
		*tok = (*tok)->next;
		right = parse_command(tok);
		if (!right)
			break ;
		node = new_ast_node("|", ND_PIPE);
		if (!node)
			return (left);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

/*
** Parses a sequence: pipeline (';' pipeline)* .
** If a semicolon is found without a following pipeline, break.
*/
static t_node	*parse_sequence(t_token **tok)
{
	t_node	*left;
	t_node	*node;
	t_node	*right;

	left = parse_pipe(tok);
	if (!left)
		return (NULL);
	while (*tok && (*tok)->type == TK_SEMICOLON)
	{
		*tok = (*tok)->next;
		right = parse_pipe(tok);
		if (!right)
			break ;
		node = new_ast_node(";", ND_SEQ);
		if (!node)
			return (left);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

/*
** parser: The parser entry point.
** Uses a local copy of the token pointer to avoid modifying the original.
*/
t_node	*parser(t_token *token)
{
	t_node	*ast;
	t_token	*tmp;

	tmp = token;
	ast = parse_sequence(&tmp);
	return (ast);
}
