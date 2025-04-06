/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:56:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 18:51:10 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	make_node(t_node **node, enum e_node_type type)
{
	*node = malloc(sizeof(t_node));
	if (*node == NULL)
		return (-1);
	(*node)->type = type;
	(*node)->error = 0;
	(*node)->left = NULL;
	(*node)->right = NULL;
	return (0);
}

void	init_cmd_data(t_cmd_node *cmd)
{
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->redir_in = NULL;
	cmd->redir_out = NULL;
	cmd->heredoc = NULL;
	cmd->append = NULL;
}

int	is_part_of_cmd(t_token *token)
{
	if (token == NULL)
		return (-1);
	if (token->type == O_BRACKET)
		return (1);
	if (token->type == FLAG)
		return (2);
	if (token->type == COMMAND || token->type == ARGUMENT)
		return (2);
	if (token->type == WILDCARD)
		return (2);
	if (token->type == REDIR_IN || token->type == REDIR_OUT)
		return (3);
	if (token->type == APPEND || token->type == HEREDOC)
		return (3);
	return (0);
}
