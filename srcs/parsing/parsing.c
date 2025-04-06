/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:55:32 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 16:15:27 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Recursive descendent parsing with a binary tree, the actual command 
 * being the foliage */
/* Create nodes by order of importance: cmd -> brackets -> pipe -> AND/OR */
/* If a bracket is found, start real recursion */
/* The parse_command function creates a special node containing cmd, args, 
 * redir and wildcards */

t_node	*parse_command(t_token **tk)
{
	t_cmd_node	cmd;
	t_node		*cmd_node;
	char		*ag;

	init_cmd_data(&cmd);
	ag = NULL;
	cmd_node = NULL;
	if (make_node(&cmd_node, CMD_NODE) == -1)
		return (error_malloc(cmd_node));
	while (*tk != NULL && is_part_of_cmd(*tk) > 1)
	{
		if (is_part_of_cmd(*tk) == 3)
		{
			handle_redir(&cmd, tk);
			if ((*tk)->type != ARGUMENT)
				return (free_wrap(cmd.command), cmd_err(0, cmd_node, &ag, tk));
		}
		else
			put_cmd_arg(&(cmd.command), tk, &ag);
		*tk = (*tk)->next;
		if (*tk != NULL && (*tk)->type == O_BRACKET && (*tk)->next != NULL && \
				(*tk)->next->type == COMMAND)
			return (free_wrap(cmd.command), cmd_err(0, cmd_node, &ag, tk));
	}
	return (return_cmd_node(ag, cmd, cmd_node));
}

t_node	*parse_bracket_condition(t_token **tk, t_node *l, int *is_bracket)
{
	*tk = (*tk)->next;
	if (*tk == NULL || (*tk != NULL && (is_part_of_cmd(*tk) == 0)))
		return (synthax_error(l, 1, tk));
	(*is_bracket)++;
	l = parsing(tk);
	if (l->error == 0 && (*tk == NULL || \
		(*tk != NULL && (*tk)->type != C_BRACKET)))
		return ((synthax_error(l, 1, tk)));
	if (l->error == 1)
		return (l);
	(*is_bracket)--;
	*tk = (*tk)->next;
	return (l);
}

t_node	*parse_bracket(t_token **tk)
{
	static int	is_bracket;
	t_node		*left;

	left = NULL;
	if (*tk != NULL && (*tk)->type == COMMAND)
	{
		left = parse_command(tk);
		if (left == NULL || left->error == 1)
			return (left);
	}
	else if (*tk == NULL || (*tk)->type != O_BRACKET)
		return (synthax_error(left, 1, tk));
	if (*tk != NULL && (*tk)->type == O_BRACKET)
		left = parse_bracket_condition(tk, left, &is_bracket);
	if (left->error == 0 && ((*tk != NULL && (*tk)->type == C_BRACKET \
		&& is_bracket == 0) || (*tk == NULL && is_bracket > 0)))
		return (synthax_error(left, 1, tk));
	return (left);
}

t_node	*parse_pipe(t_token **token)
{
	t_p_norm	pn;

	pn.left = parse_bracket(token);
	if (pn.left == NULL || pn.left->error == 1)
		return (pn.left);
	while (*token != NULL && (*token)->type == PIPE)
	{
		pn.op_node = NULL;
		*token = (*token)->next;
		if (*token != NULL && is_part_of_cmd(*token) == 0)
			return (synthax_error(pn.left, 1, token));
		pn.right = parse_bracket(token);
		if (pn.right == NULL || pn.right->error == 1)
			return (free_tree(pn.left), pn.right);
		if (make_node(&pn.op_node, PIPE_NODE) == -1)
			return (free_wrap(pn.left), error_malloc(pn.right));
		pn.op_node->left = pn.left;
		pn.op_node->right = pn.right;
		pn.left = pn.op_node;
	}
	return (pn.left);
}

t_node	*parsing(t_token **tk)
{
	t_p_norm	pn;

	pn.left = NULL;
	if (init_parse_error(tk) == 1)
		return (pn.left);
	pn.left = parse_pipe(tk);
	if (pn.left == NULL || pn.left->error == 1)
		return (pn.left);
	while (*tk != NULL && ((*tk)->type == AND || (*tk)->type == OR))
	{
		pn.op_node = NULL;
		pn.token_type = (*tk)->type;
		*tk = (*tk)->next;
		if (*tk != NULL && is_part_of_cmd(*tk) == 0)
			return (synthax_error(pn.left, 1, tk));
		pn.right = parse_pipe(tk);
		if (pn.right == NULL || pn.right->error == 1)
			return (free_tree(pn.left), pn.right);
		if (make_node(&pn.op_node, pn.token_type) == -1)
			return (free_wrap(pn.left), error_malloc(pn.right));
		pn.op_node->left = pn.left;
		pn.op_node->right = pn.right;
		pn.left = pn.op_node;
	}
	return (pn.left);
}
