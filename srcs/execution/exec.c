/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:42:43 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 18:19:48 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_builtin(t_node *tree, t_shell *sh)
{
	int	error;

	error = 0;
	if (!ft_strncmp(tree->command.command, "echo", 5))
		return (ft_echo(tree->command.args));
	if (!ft_strncmp(tree->command.command, "goat", 3))
		return (ft_sorpresa(), 0);
	if (!ft_strncmp(tree->command.command, "cd", 3))
		return (ft_cd(tree->command.args[1], sh), 0);
	if (!ft_strncmp(tree->command.command, "pwd", 4))
		return (ft_printf("%s\n", ft_getpwd()), 0);
	if (!ft_strncmp(tree->command.command, "export", 7))
		return (ft_export(sh, tree->command.args[1]), 0);
	if (!ft_strncmp(tree->command.command, "unset", 6))
		return (ft_unset(sh, tree->command.args[1]), 0);
	if (!ft_strncmp(tree->command.command, "env", 4))
		return (ft_env(tree->command.args, sh));
	if (!ft_strncmp(tree->command.command, "exit", 4))
	{
		if (ft_exit(tree, sh, &error) == 1)
			return (1);
		/* rl_clear_history(); */
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
		if (errno == ENOENT)
		{
			printf("msh: %s: not found\n", cmd_tree->command.command);
			exit(127);
		}
		exit_error("msh: permission denied", 126);
	}
	if (execve(path, cmd_tree->command.args, sh->env) == 1)
		exit_error("msh: execution error", 127);
}

void	exec_command(t_node *cmd_tree, t_shell *sh, bool in_pipe)
{
	pid_t	pid;
	int		status;

	(void)in_pipe;
	if (exec_builtin(cmd_tree, sh) <= 1)
		return ;
	if (in_pipe == true)
		check_and_exec(cmd_tree, sh);
	pid = fork();
	if (pid == 0)
		check_and_exec(cmd_tree, sh);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->error = WEXITSTATUS(status);
	else
		sh->error = status;
}

void	recursive_exec(t_node **cmd_tree, t_shell *sh, bool in_pipe)
{
	int	saved_stdin;
	int	saved_stdout;

	if ((*cmd_tree)->type == CMD_NODE)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		exec_redir(*cmd_tree);
		exec_command(*cmd_tree, sh, in_pipe);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
	}
	if ((*cmd_tree)->type == PIPE_NODE)
		exec_pipe(cmd_tree, sh);
	if ((*cmd_tree)->type == AND_NODE)
		exec_and(cmd_tree, sh, in_pipe);
	if ((*cmd_tree)->type == OR_NODE)
		exec_or(cmd_tree, sh, in_pipe);
}
