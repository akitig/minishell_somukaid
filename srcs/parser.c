/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 02:34:07 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/21 04:52:44 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_node	*ft_lstlast_node(t_node *node)
{
	if (node == NULL)
		return (NULL);
	while (node->left != NULL)
		node = node->left;
	return (node);
}

void	ft_lstadd_back_node(t_node **node, t_node *new)
{
	if (node == NULL || new == NULL)
		return ;
	if (*node == NULL)
		*node = new;
	else
		ft_lstlast_node(*node)->left = new;
}

t_node	*ft_lstnew_node(char *text)
{
	t_node	*ans;

	ans = (t_node *)malloc(sizeof(t_node));
	if (ans == NULL)
		return (NULL);
	ans->text = text;
	ans->left = NULL;
    ans->right = NULL;
    ans->type = 0;
	return (ans);
}

void	ft_lstadd_front_node(t_node **node, t_node *new)
{
	if (node == NULL || new == NULL)
		return ;
	new->left = *node;
	*node = new;
}

void    print_node(t_node *node)
{
    if (node == NULL)
        return ;
    print_node(node->left);
    if (node->text != NULL)
        printf("%s\n", node->text);
    print_node(node->right);
}

t_node    *parser(t_token *token)
{
    t_token *tmp_token;
    t_node *root_node;
    t_node *top_node;
    t_node *past_top_node;
    t_node *node;
    int     right_flag;
    int     first_flag;
    
    tmp_token = token;
    root_node = NULL;
    top_node = NULL;
    past_top_node = NULL;
    root_node = top_node;
    right_flag = 0;
    first_flag = 0;
    while (tmp_token != NULL)
    {
        if (right_flag == 1)
        {
            node = ft_lstnew_node(tmp_token->word);
            node->type = command;
            if (first_flag == 0)
            {  
                root_node = top_node;
                first_flag = 1;
            }
            past_top_node = top_node;
            top_node->right = node;
            top_node = node;
            right_flag = 0;
        }
        else if (tmp_token->type == TK_WORD)
        {
            node = ft_lstnew_node(tmp_token->word);
            node->type = command;
            ft_lstadd_back_node(&top_node, node);            
        }
        else if (tmp_token->type == TK_OP)
        {
            node = ft_lstnew_node(tmp_token->word);
            node->type = pipe_c;
            ft_lstadd_front_node(&top_node, node);
            right_flag = 1;
            if (past_top_node != NULL)
                past_top_node->right = node;
        }
        tmp_token = tmp_token->next;
    }
    printf("----- node -----\n");
    if (root_node != NULL)
        print_node(root_node);
    else
        print_node(top_node);
    printf("----- node -----\n");
    if (root_node != NULL)
        return (root_node);
    else
        return (top_node);
}