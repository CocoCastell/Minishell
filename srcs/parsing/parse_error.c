/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:56:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/14 16:12:13 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_node	*cmd_err(int flag, t_node *cmd_node, char **arg_ln, t_token **tk)
{
	free_wrap(*arg_ln);
	cmd_node->type = PIPE_NODE;
	if ((*tk)->next == NULL)
		return (synthax_error(cmd_node, 4, tk));
	if (is_part_of_cmd((*tk)->next) == 3 || flag == 1)
		return (synthax_error(cmd_node, 5, tk));
	if (is_part_of_cmd((*tk)->next) == 0)
		return (synthax_error(cmd_node, 3, tk));
	return (synthax_error(cmd_node, 2, tk));
}

t_node	*synthax_error(t_node *node, int flag, t_token **tok)
{
	if (flag == 1 && (*tok == NULL || is_part_of_cmd(*tok) == 3))
		printf(BL"msh: syntax error near unexpected token `('\n"DEF);
	else if (flag == 1 && *tok != NULL)
		printf(BL"msh: syntax error near unexpected token `%s'\n"DEF, \
			(*tok)->str);
	else if (flag == 2)
		printf(BL"msh: Syntax error: word unexpected (expecting \")\")\n"DEF);
	else if (flag == 3 && *tok != NULL)
		printf(BL"msh: Syntax error: '%s' unexpected (expecting \")\"\n" \
				DEF, (*tok)->str);
	else if (flag == 4 && *tok != NULL)
		printf(BL"msh: Syntax error: newline unexpected\n"DEF);
	else if (flag == 5)
		printf(BL"msh: Syntax error: redirection unexpected\n"DEF);
	else if (flag == 6)
		printf(BL"msh: syntax error: unexpected end of file\n"DEF);
	if (node == NULL)
		make_node(&node, PIPE_NODE);
	node->error = 1;
	return (node);
}

t_node	*error_malloc(t_node *node)
{
	if (node != NULL)
		node->error = 1;
	perror(RED"Memory allocation failed\n"DEF);
	return (node);
}

int	init_parse_error(t_token **tk)
{
	if (*tk == NULL)
		return (1);
	if (is_part_of_cmd(*tk) == 0 && (*tk)->type != C_BRACKET)
		return (printf(BL"msh: Syntax error: '%s' unexpected\n" \
					DEF, (*tk)->str), 1);
	return (0);
}
