/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 23:38:35 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/27 00:35:46 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// 与えられたノードリストの末尾を返す（左奥）
t_node	*ft_lstlast_node(t_node *node)
{
	while (node && node->left)
		node = node->left;
	return (node);
}

// ノードリストの末尾に新しいノードを追加する
void	ft_lstadd_back_node(t_node **node, t_node *new)
{
	t_node	*last;

	if (!node || !new)
		return ;
	if (*node == NULL)
		*node = new;
	else
	{
		last = ft_lstlast_node(*node);
		last->left = new;
	}
}

// 新しいノードを追加する
t_node	*ft_lstnew_node(char *text)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->value = text;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = 0;
	return (new_node);
}

// ノードリストの先頭に新しいノードを追加する
void	ft_lstadd_front_node(t_node **node, t_node *new)
{
	if (!node || !new)
		return ;
	new->left = *node;
	*node = new;
}