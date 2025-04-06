/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:57:52 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/31 17:02:59 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current != NULL)
	{
		next = current->next;
		free_wrap(current->str);
		free_wrap(current);
		current = next;
	}
	free_wrap(tokens);
}

void	free_cmd(t_cmd_node *cmd)
{
	free_wrap(cmd->command);
	free_wrap(cmd->redir_in);
	free_wrap(cmd->redir_out);
	free_wrap(cmd->heredoc);
	free_wrap(cmd->append);
	if (cmd->args != NULL)
		ft_free_string_array(cmd->args);
}

void	free_tree(t_node *tree)
{
	if (tree->type == CMD_NODE)
		free_cmd(&(tree->command));
	if (tree->left != NULL)
		free_tree(tree->left);
	if (tree->right != NULL)
		free_tree(tree->right);
	free_wrap(tree);
	tree = NULL;
}

void	free_sh(t_shell *sh)
{
	if (sh->input)
		free_wrap(sh->input);
	if (sh->tree)
		free_tree(sh->tree);
}

int	free_multiple(int amount, char *f, char *s, char *t)
{
	if (amount == 1)
		return (free(f), 0);
	if (amount == 2)
		return (free(f), free(s), 0);
	if (amount == 3)
		return (free(f), free(s), free(t), 0);
	return (-1);
}
