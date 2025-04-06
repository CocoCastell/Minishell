/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:56:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 18:57:05 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	put_redir(char **cmd, t_token **tk)
{
	char	*tmp;
	char	*tmp2;

	*tk = (*tk)->next;
	if (*cmd == NULL)
		*cmd = ft_strdup((*tk)->str);
	else
	{
		tmp = ft_strjoin("\n", (*tk)->str);
		if (tmp == NULL)
			return ; //error
		tmp2 = ft_strjoin(*cmd, tmp);
		free_wrap(*cmd);
		free_wrap(tmp);
		*cmd = ft_strdup(tmp2);
		free_wrap(tmp2);
	}
}

static void	put_append(char **cmd, char *app)
{
	char	*tmp;
	char	*tmp2;

	if (*cmd == NULL)
		*cmd = ft_strdup(app);
	else
	{
		tmp = ft_strjoin("\n", app);
		if (tmp == NULL)
			return ; //error
		tmp2 = ft_strjoin(*cmd, tmp);
		free_wrap(*cmd);
		free_wrap(tmp);
		*cmd = ft_strdup(tmp2);
		free_wrap(tmp2);
	}
}

void	handle_redir(t_cmd_node *cmd, t_token **tk)
{
	if ((*tk)->next == NULL || (*tk)->next->type != ARGUMENT)
		return ;
	else if ((*tk)->type == REDIR_OUT || (*tk)->type == APPEND)
	{
		if ((*tk)->type == APPEND)
			put_append(&cmd->append, "0");
		else
			put_append(&cmd->append, "1");
		put_redir(&cmd->redir_out, tk);
	}
	else if ((*tk)->type == REDIR_IN)
		put_redir(&cmd->redir_in, tk);
	else if ((*tk)->type == HEREDOC)
		put_redir(&cmd->heredoc, tk);
}
