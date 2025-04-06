/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:27:53 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 16:14:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_loop(int pipe_fd[2], char *hd)
{
	char	*input;

	while (1)
	{
		input = readline("heredoc> ");
		if (ft_strncmp(input, hd, ft_strlen(hd) + 1) == 0)
			break ;
		write(pipe_fd[1], input, ft_strlen(input));
		write(pipe_fd[1], "\n", 1);
		free_wrap(input);
	}
	free_wrap(input);
}

void	handle_heredoc(char *args)
{
	char	**hd;
	int		pipe_fd[2];
	int		i;

	i = 0;
	hd = ft_split(args, '\n');
	if (pipe(pipe_fd) == -1)
		return ; //error
	while (hd[i] != NULL)
	{
		heredoc_loop(pipe_fd, hd[i]);
		i++;
	}
	ft_free_string_array(hd);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}

void	handle_redir_out(char *redir_out, char *append)
{
	char	**all_redir;
	char	**all_app;
	int		fd;
	int		i;

	i = 0;
	all_redir = ft_split(redir_out, '\n');
	all_app = ft_split(append, '\n');
	while (all_redir[i] != NULL)
	{
		if (*all_app[i] == '1')
			fd = open(all_redir[i], O_RDWR | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(all_redir[i], O_RDWR | O_CREAT | O_APPEND, 0644);
		i++;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	ft_free_string_array(all_redir);
	ft_free_string_array(all_app);
}

void	exec_redir(t_node *cmd_tree)
{
	int	in_fd;

	if (cmd_tree->command.redir_out != NULL)
		handle_redir_out(cmd_tree->command.redir_out, cmd_tree->command.append);
	if (cmd_tree->command.redir_in != NULL)
	{
		in_fd = open(cmd_tree->command.redir_in, O_RDONLY);
		dup2(in_fd, STDIN_FILENO);
	}
	if (cmd_tree->command.heredoc != NULL)
		handle_heredoc(cmd_tree->command.heredoc);
}
