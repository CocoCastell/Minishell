/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ope.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:27:53 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 16:39:13 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exec_left_cmd(int pipe_fd[2], t_node **cmd_tree, t_shell *sh)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	*cmd_tree = (*cmd_tree)->left;
	recursive_exec(cmd_tree, sh, true);
	exit(0);
}

static void	exec_right_cmd(int pipe_fd[2], t_node **cmd_tree, t_shell *sh)
{
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	*cmd_tree = (*cmd_tree)->right;
	recursive_exec(cmd_tree, sh, true);
	exit(0);
}

void	exec_pipe(t_node **cmd_tree, t_shell *sh)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		pipe_fd[2];

	pipe(pipe_fd);
	left_pid = fork();
	if (left_pid == 0)
		exec_left_cmd(pipe_fd, cmd_tree, sh);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	right_pid = fork();
	if (right_pid == 0)
		exec_right_cmd(pipe_fd, cmd_tree, sh);
	close(pipe_fd[0]);
	waitpid(right_pid, NULL, 0);
	sh->error = errno;
}

void	exec_or(t_node **cmd_tree, t_shell *sh, bool in_pipe)
{
	t_node	*right_node;

	right_node = (*cmd_tree)->right;
	*cmd_tree = (*cmd_tree)->left;
	recursive_exec(cmd_tree, sh, in_pipe);
	if (sh->error != 0)
		recursive_exec(&right_node, sh, in_pipe);
}

void	exec_and(t_node **cmd_tree, t_shell *sh, bool in_pipe)
{
	t_node	*right_node;

	right_node = (*cmd_tree)->right;
	*cmd_tree = (*cmd_tree)->left;
	recursive_exec(cmd_tree, sh, in_pipe);
	if (sh->error == 0)
		recursive_exec(&right_node, sh, in_pipe);
}
