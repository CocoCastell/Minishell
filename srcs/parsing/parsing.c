/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:55:32 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/06 19:39:41 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Recursive descendent parsing with a binary tree, the actual command 
 * being the foliage */
/* Create nodes by order of importance: cmd -> brackets -> pipe -> AND/OR */
/* If a bracket is found, start real recursion */
/* The parse_command function creates a special node containing cmd, args, 
 * redir and wildcards */

t_node	*bracket_condition(t_token **tk, t_node *l, int *brack_nb, t_shell *sh)
{
	if (*tk != NULL && is_part_of_cmd((*tk)->prev) == 0 && (*tk)->next == NULL)
	{
		if (ask_end_cmd(tk, sh) == -1)
			return (synthax_error(l, 6, tk));
	}
	*tk = (*tk)->next;
	if (*tk == NULL || (*tk != NULL && (is_part_of_cmd(*tk) == 0)))
		return (synthax_error(l, 1, tk));
	(*brack_nb)++;
	l = parsing(tk, sh);
	if (l->error == 0 && (*tk == NULL || \
		(*tk != NULL && (*tk)->type != C_BRACKET)))
		return ((synthax_error(l, 1, tk)));
	if (l->error == 1)
		return (l);
	(*brack_nb)--;
	*tk = (*tk)->next;
	return (l);
}

t_node	*parse_bracket(t_token **tk, t_shell *sh)
{
	static int	is_bracket;
	t_node		*left;

	left = NULL;
	if (*tk != NULL && is_part_of_cmd(*tk) > 1)
	{
		left = parse_command(tk, sh);
		if (left == NULL || left->error > 0)
			return (left);
	}
	else if (*tk == NULL || (*tk)->type != O_BRACKET)
		return (synthax_error(left, 1, tk));
	if (*tk != NULL && (*tk)->type == O_BRACKET)
		left = bracket_condition(tk, left, &is_bracket, sh);
	if (left->error == 0 && ((*tk != NULL && (*tk)->type == C_BRACKET \
		&& is_bracket == 0) || (*tk == NULL && is_bracket > 0)))
		return (synthax_error(left, 1, tk));
	return (left);
}

void	link_nodes(t_p_norm *pn)
{
	pn->op_node->left = pn->left;
	pn->op_node->right = pn->right;
	pn->left = pn->op_node;
}

t_node	*parse_pipe(t_token **token, t_shell *sh)
{
	t_p_norm	pn;

	pn.left = parse_bracket(token, sh);
	if (pn.left == NULL || pn.left->error > 0)
		return (pn.left);
	while (*token != NULL && (*token)->type == PIPE)
	{
		pn.op_node = NULL;
		if ((*token)->next == NULL && ask_end_cmd(token, sh) == -1)
			return (synthax_error(pn.left, 6, token));
		*token = (*token)->next;
		if (*token != NULL && is_part_of_cmd(*token) == 0)
			return (synthax_error(pn.left, 1, token));
		pn.right = parse_bracket(token, sh);
		if (pn.right == NULL || pn.right->error > 0)
			return (free_tree(pn.left), pn.right);
		if (make_node(&pn.op_node, PIPE_NODE) == -1)
			return (free_wrap(pn.left), error_malloc(pn.right));
		link_nodes(&pn);
	}
	return (pn.left);
}

t_node	*parsing(t_token **tk, t_shell *sh)
{
	t_p_norm	pn;

	pn.left = NULL;
	if (init_parse_error(tk) == 1)
		return (pn.left);
	pn.left = parse_pipe(tk, sh);
	if (pn.left == NULL || pn.left->error > 0)
		return (pn.left);
	while (*tk != NULL && ((*tk)->type == AND || (*tk)->type == OR))
	{
		pn.op_node = NULL;
		pn.token_type = (*tk)->type;
		if ((*tk)->next == NULL && ask_end_cmd(tk, sh) == -1)
			return (synthax_error(pn.left, 6, tk));
		*tk = (*tk)->next;
		if (*tk != NULL && is_part_of_cmd(*tk) == 0)
			return (synthax_error(pn.left, 1, tk));
		pn.right = parse_pipe(tk, sh);
		if (pn.right == NULL || pn.right->error > 0)
			return (free_tree(pn.left), pn.right);
		if (make_node(&pn.op_node, pn.token_type) == -1)
			return (free_wrap(pn.left), error_malloc(pn.right));
		link_nodes(&pn);
	}
	return (pn.left);
}
