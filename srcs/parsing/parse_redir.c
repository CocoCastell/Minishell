/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:56:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/06 20:07:54 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_redir_out(t_cmd_node *cmd, enum e_type type, char *outfile)
{
	int		fd;

	if (cmd->redir_out >= 0)
		close(cmd->redir_out);
	if (type == APPEND)
		fd = open(outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
	else
		fd = open(outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
	cmd->redir_out = fd;
	if (fd == -1)
		return (perror("Error opening file"), -1);
	return (0);
}

int	handle_redir(t_cmd_node *cmd, t_token **tk, t_shell *sh, int fd)
{
	if ((*tk)->next == NULL || (*tk)->next->type != ARGUMENT)
		return (-1);
	*tk = (*tk)->next;
	if (((*tk)->prev->type == REDIR_OUT || (*tk)->prev->type == APPEND) && \
		handle_redir_out(cmd, (*tk)->type, (*tk)->str) == -1)
		return (-4);
	else if ((*tk)->prev->type == REDIR_IN)
	{
		if (cmd->redir_in >= 0)
			close(cmd->redir_in);
		fd = open((*tk)->str, O_RDONLY);
		if (fd == -1)
			return (perror("Error opening file"), -4);
		cmd->redir_in = fd;
	}
	if ((*tk)->prev->type == HEREDOC)
	{
		if (cmd->redir_in >= 0)
			close(cmd->redir_in);
		cmd->redir_in = handle_heredoc((*tk)->str, sh);
		if (cmd->redir_in < 0)
			return (cmd->redir_in);
	}
	return (0);
}

int	check_redir(t_token **tk, t_shell *sh, t_cmd_node *cmd)
{
	int	err_redir;

	err_redir = 0;
	if (is_part_of_cmd(*tk) == 3)
	{
		err_redir = handle_redir(cmd, tk, sh, 0);
		if (err_redir == -4)
			cmd->redir_err = 1;
		else if (err_redir < 0)
			return (err_redir);
		return (1);
	}
	return (0);
}
