/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:56:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/06 19:43:55 by cochatel         ###   ########.fr       */
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
	cmd->heredoc = NULL;
	cmd->append = NULL;
	cmd->redir_in = -2;
	cmd->redir_out = -2;
	cmd->redir_err = 0;
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

static void	join_token(t_token *tk_to_add, t_token *tk)
{
	if (tk == NULL || tk_to_add == NULL)
		return ;
	while (tk->next != NULL)
		tk = tk->next;
	tk->next = tk_to_add;
}

int	ask_end_cmd(t_token **tk, t_shell *sh)
{
	char	*input;
	t_token	**end_tk;

	while (1)
	{
		input = readline(">");
		if (input == NULL)
			return (-1);
		end_tk = tokenize(input, sh);
		free_wrap(input);
		if (end_tk != NULL)
			break ;
	}
	join_token(*end_tk, *tk);
	free_wrap(end_tk);
	return (0);
}
