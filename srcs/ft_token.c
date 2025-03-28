/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 23:14:31 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/28 13:23:13 by akunimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// t_token型のft_lstlast
t_token	*ft_lstlast_token(t_token *token)
{
	if (token == NULL)
		return (NULL);
	while (token->next != NULL)
		token = token->next;
	return (token);
}

/*
** ft_lstadd_back_token: Adds a token at the end of the list.
*/
void	ft_lstadd_back_token(t_token **lst, t_token *new)
{
	t_token *tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}