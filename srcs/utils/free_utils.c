/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:57:52 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/06 17:50:28 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Free token structure
 * @param tokens t_token structure being freed
 * @return void
 */
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
	free_wrap(cmd->heredoc);
	free_wrap(cmd->append);
	if (cmd->args != NULL)
		ft_free_string_array(cmd->args);
	if (cmd->redir_in > -1)
		close(cmd->redir_in);
	if (cmd->redir_out > -1)
		close(cmd->redir_out);
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

/**
 * @brief Free shell structure. Free input and tree.
 * @param sh t_shell Shell structure
 * @return void
 */
void	free_sh(t_shell *sh)
{
	if (sh->input)
		free_wrap(sh->input);
	if (sh->tree)
		free_tree(sh->tree);
}

/**
 * @brief Free multiple strings at once. Min 1, max 3. Need to specify the
 * amount and nullify the ones not used. E.G. free_multiple(2, f, s, NULL) 
 * @param amount int specifying how many string will be freed (1 to 3)
 * @param str1 string to be freed
 * @param str2 string to be freed (or NULL)
 * @param str3 string to be freed (or NULL)
 * @return 0 on success, or -1
 */
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
