/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:42:43 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/22 11:31:05 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_builtin(t_node *tree, t_shell *sh, int error)
{
	if (!ft_strncmp(tree->command.command, "echo", 5))
		return (ft_echo(tree->command.args));
	if (!ft_strncmp(tree->command.command, "goat", 5))
		return (ft_sorpresa(), 0);
	if (!ft_strncmp(tree->command.command, "cd", 3))
		return (ft_cd(tree->command.args[1], tree->command.args, sh));
	if (!ft_strncmp(tree->command.command, "pwd", 4))
		return (ft_pwd(sh), 0);
	if (!ft_strncmp(tree->command.command, "export", 7))
		return (ft_handle_export(sh, tree->command.args));
	if (!ft_strncmp(tree->command.command, "unset", 6))
		return (ft_handle_unset(sh, tree->command.args));
	if (!ft_strncmp(tree->command.command, "env", 4))
		return (ft_env(tree->command.args, sh));
	if (!ft_strncmp(tree->command.command, "exit", 5))
	{
		if (ft_exit(tree, sh, &error) == 1)
			return (1);
		exit(error);
	}
	return (2);
}

void	check_and_exec(t_node *cmd_tree, t_shell *sh)
{
	char	*path;

	path = get_path(cmd_tree->command.command, sh->env);
	if (path == NULL)
	{
		ft_free_string_array(sh->env);
		if (errno == ENOENT)
		{
			printf(BL"msh: %s: command not found\n"DEF, \
					cmd_tree->command.command);
			free_sh(sh);
			exit(127);
		}
		free_sh(sh);
		exit_error(BL"msh: permission denied"DEF, 126);
	}
	if (execve(path, cmd_tree->command.args, sh->env) == 1)
		exit_error(BL"msh: execution error"DEF, 127);
}

int	handle_builtin(t_node *cmd_tree, t_shell *sh)
{
	int	res;

	res = exec_builtin(cmd_tree, sh, 0);
	if (res != 2)
	{
		sh->error = res;
		return (0);
	}
	return (1);
}

int	exec_command(t_node *cmd_tree, t_shell *sh, bool in_pipe)
{
	t_ec	ec;

	ec.old_sigint = signal(SIGINT, handle_fork_sig);
	ec.old_sigquit = signal(SIGQUIT, handle_fork_sig);
	if (cmd_tree->command.command == NULL)
		return (1);
	if (handle_builtin(cmd_tree, sh) == 0)
		return (0);
	if (in_pipe == true)
		check_and_exec(cmd_tree, sh);
	ec.pid = fork();
	if (ec.pid == -1)
		return (perror("Fork error"), 1);
	if (ec.pid == 0)
		check_and_exec(cmd_tree, sh);
	waitpid(ec.pid, &ec.status, 0);
	if (WIFEXITED(ec.status))
		sh->error = WEXITSTATUS(ec.status);
	else
		sh->error = 128 + WTERMSIG(ec.status);
	return (signal(SIGINT, ec.old_sigint), signal(SIGQUIT, ec.old_sigquit), 0);
}

void	recursive_exec(t_node **cmd_tree, t_shell *sh, bool in_pipe)
{
	int		saved_std[2];
	t_ec	ec;

	ec.old_sigint = signal(SIGINT, handle_others);
	// ec.old_sigquit = signal(SIGQUIT, handle_fork_sig);
	if ((*cmd_tree)->type == CMD_NODE)
	{
		saved_std[0] = dup(STDIN_FILENO);
		saved_std[1] = dup(STDOUT_FILENO);
		if (exec_redir(*cmd_tree, sh) == 0)
			exec_command(*cmd_tree, sh, in_pipe);
		dup2(saved_std[0], STDIN_FILENO);
		dup2(saved_std[1], STDOUT_FILENO);
	}
	if ((*cmd_tree)->type == PIPE_NODE)
		exec_pipe(cmd_tree, sh, in_pipe);
	if ((*cmd_tree)->type == AND_NODE)
		exec_and(cmd_tree, sh, in_pipe);
	if ((*cmd_tree)->type == OR_NODE)
		exec_or(cmd_tree, sh, in_pipe);
	if (sh->error == 256)
		sh->error = 1;
	signal(SIGINT, ec.old_sigint);
	// signal(SIGQUIT, ec.old_sigquit);
}
