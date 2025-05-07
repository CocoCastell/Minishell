/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ope.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:27:53 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/14 16:15:16 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exec_left_cmd(int pipe_fd[2], t_node **cmd_tree, t_shell *sh)
{
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		free_all(sh, pipe_fd, BL"Pipe error"DEF, true);
	close(pipe_fd[1]);
	*cmd_tree = (*cmd_tree)->left;
	recursive_exec(cmd_tree, sh, true);
	ft_free_string_array(sh->env);
	free_sh(sh);
	exit(0);
}

static void	exec_right_cmd(int pipe_fd[2], t_node **cmd_tree, t_shell *sh)
{
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		free_all(sh, pipe_fd, BL"Pipe error"DEF, true);
	close(pipe_fd[0]);
	*cmd_tree = (*cmd_tree)->right;
	recursive_exec(cmd_tree, sh, true);
	ft_free_string_array(sh->env);
	free_sh(sh);
	exit(0);
}

void	exec_pipe(t_node **cmd_tree, t_shell *sh, bool in_pipe)
{
	pid_t	pids[2];
	int		pipe_fd[2];
	t_ec	ec;

	// ec.old_sigint = signal(SIGINT, handle_fork_sig);
	// ec.old_sigquit = signal(SIGQUIT, handle_fork_sig);
	if (pipe(pipe_fd) == -1)
		free_all(sh, pipe_fd, BL"Pipe error"DEF, in_pipe);
	pids[0] = fork();
	if (pids[0] == -1)
		free_all(sh, pipe_fd, BL"Fork error"DEF, in_pipe);
	if (pids[0] == 0)
		exec_left_cmd(pipe_fd, cmd_tree, sh);
	close(pipe_fd[1]);
	pids[1] = fork();
	if (pids[1] == -1)
		free_all(sh, pipe_fd, BL"Fork error"DEF, in_pipe);
	if (pids[1] == 0)
		exec_right_cmd(pipe_fd, cmd_tree, sh);
	close(pipe_fd[0]);
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], &ec.status, 0);
	// signal(SIGINT, ec.old_sigint);
	// signal(SIGQUIT, ec.old_sigquit);
	sh->error = ec.status;
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
