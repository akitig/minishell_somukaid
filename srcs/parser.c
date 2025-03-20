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

/*
typedef enum
{
    delimiter,
    pipe,
    command,
    argument,
    redirection,
}       NodeType;
*/

/*
typedef struct  s_node
{
    NodeType        type;
    struct s_node   *left;
    struct s_node   *right;
    char            *text;
}               t_node;
*/

t_node    *parser(t_token *token)
{
    t_node  *node0;
    t_node  *node1;
    t_node  *node2;
    t_node  *node3;
    t_node  *node4;
    t_node  *node5;
    t_node  *node6;
    t_node  *node7;
    
    (void)token;
    node0 = (t_node *)malloc(sizeof(t_node));
    
    node0->type = delimiter;
    node1 = (t_node *)malloc(sizeof(t_node));
    node0->left = node1;
    node0->right = NULL;
    node0->text = ft_strdup(";");

    node1->type = pipe_c;
    node2 = (t_node *)malloc(sizeof(t_node));
    node3 = (t_node *)malloc(sizeof(t_node));
    node1->left = node2;
    node1->right = node3;
    node1->text = ft_strdup("|");
    
    node2->type = delimiter;
    node4 = (t_node *)malloc(sizeof(t_node));
    node5 = (t_node *)malloc(sizeof(t_node));
    node2->left = node4;
    node2->right = node5;
    node2->text = ft_strdup(";");

    node3->type = delimiter;
    node6 = (t_node *)malloc(sizeof(t_node));
    node7 = (t_node *)malloc(sizeof(t_node));
    node3->left = node6;
    node3->right = node7;
    node3->text = ft_strdup(";");

    node4->type = command;
    node4->left = NULL;
    node4->right = NULL;
    node4->text = ft_strdup("echo");

    node5->type = argument;
    node5->left = NULL;
    node5->right = NULL;
    node5->text = ft_strdup("hello world");

    node6->type = command;
    node6->left = NULL;
    node6->right = NULL;
    node6->text = ft_strdup("cat");

    node7->type = argument;
    node7->left = NULL;
    node7->right = NULL;
    node7->text = ft_strdup("input.txt");

    printf("----- node -----\n");

	printf("%s\n", node0->text);
	printf("%d\n", node0->type);
    printf("%s\n", node1->text);
	printf("%d\n", node1->type);
    printf("%s\n", node2->text);
	printf("%d\n", node2->type);
    printf("%s\n", node3->text);
	printf("%d\n", node3->type);
    printf("%s\n", node4->text);
	printf("%d\n", node4->type);
    printf("%s\n", node5->text);
	printf("%d\n", node5->type);
    printf("%s\n", node6->text);
	printf("%d\n", node6->type);
    printf("%s\n", node7->text);
	printf("%d\n", node7->type);

    printf("----- node -----\n");
    
    return (node0);
}

/*
int main(void)
{
    t_parser    *data;
    t_token     *token;

    data = parser(token);
    return (0);
}
*/