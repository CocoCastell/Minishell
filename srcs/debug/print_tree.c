/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:54:33 by sluterea          #+#    #+#             */
/*   Updated: 2025/03/27 12:54:35 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_tree(t_node *node, int depth)
{
	int	i;

	if (node == NULL)
		return ;
	i = 0;
	while (i < depth)
	{
		printf("      ");
		i++;
	}
	if (node->type == CMD_NODE)
		printf("CMD_NODE: %s\n", node->command.command);
	else if (node->type == PIPE_NODE)
		printf("PIPE_NODE\n");
	else if (node->type == AND_NODE)
		printf("AND_NODE\n");
	else if (node->type == OR_NODE)
		printf("OR_NODE\n");
	else if (node->type == BRACKET_NODE)
		printf("BRACKET_NODE\n");
	else
		printf("UNKNOWN NODE\n");
	print_tree(node->left, depth + 1);
	print_tree(node->right, depth + 1);
}
