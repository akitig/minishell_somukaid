/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akunimot <akitig24@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 23:14:31 by akunimot          #+#    #+#             */
/*   Updated: 2025/03/26 23:27:36 by akunimot         ###   ########.fr       */
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
