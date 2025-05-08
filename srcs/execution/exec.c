/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:42:43 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/08 14:09:33 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Check if the given command is a builtin and executes it if yes
 * @return 2 if it is not a builtin, the builtin's return if it is
 */
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

/**
 * @brief Call the function to apply the possible rendirections
 *		  Get the absolute path of the command
 *		  Execute the command if the path exists
 * @return 2 126, 127 in case of error
 *		   the exit return if the path exists
 */
void	check_and_exec(t_node *cmd_tree, t_shell *sh)
{
	char	*path;

	change_in_out(cmd_tree, false);
	path = get_path(cmd_tree->command.command, sh->env);
	if (path == NULL)
	{
		ft_free_string_array(sh->env);
		if (errno == ENOENT)
		{
			printf("msh: %s:command not found\n", cmd_tree->command.command);
			free_sh(sh);
			exit(127);
		}
		free_sh(sh);
		exit_error(BL"msh: permission denied"DEF, 126);
	}
	if (execve(path, cmd_tree->command.args, sh->env) == 1)
		exit_error(BL"msh: execution error"DEF, 127);
}

/**
 * @brief Saves the standards IN/OUT, apply possible redireccions changes
 *        Put back the standards IN/OUT and call the function to check if
 *        the command is a built-in
 * @return 0 is case of error or if the command was a built-in
 *         1 if the command is not a builtin
 */
int	handle_builtin(t_node *cmd_tree, t_shell *sh)
{
	int	res;
	int	saved_std[2];

	saved_std[0] = dup(STDIN_FILENO);
	saved_std[1] = dup(STDOUT_FILENO);
	if (change_in_out(cmd_tree, true) == -1)
		return (0);
	res = exec_builtin(cmd_tree, sh, 0);
	if (dup2(saved_std[0], STDIN_FILENO) == -1)
	{
		sh->error = 1;
		return (close(saved_std[0]), close(saved_std[1]), 0);
	}
	close(saved_std[0]);
	if (dup2(saved_std[1], STDOUT_FILENO) == -1)
	{
		sh->error = 1;
		return (close(saved_std[1]), 0);
	}
	if (res != 2)
	{
		sh->error = res;
		return (close(saved_std[1]), 0);
	}
	return (close(saved_std[1]), 1);
}

/**
 *@brief Call the function to execute built-in if it is
 *		 If it is not a built-in, call the execute command if
 *		 it is already in a pipe, if not it forks before
 *@return 0 in case of sucess
 *		  1 in case of error
 */
int	exec_command(t_node *cmd_tree, t_shell *sh, bool in_pipe)
{
	t_ec	ec;

	ec.old_sigint = signal(SIGINT, handle_fork_sig);
	ec.old_sigquit = signal(SIGQUIT, handle_fork_sig);
	if (handle_builtin(cmd_tree, sh) == 0)
		return (0);
	if (in_pipe == true)
		return (check_and_exec(cmd_tree, sh), 1);
	(void)in_pipe;
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

/**
 *@brief Handle recursively the binary tree of commands, in hierarchical order,
 *		 and executes each nodes based on its type
 *		 If the there was previously a redir error (wich doesnt stop the
 *		 compilation) and the node is a command, it skips the node;
 *@return void
 */
void	recursive_exec(t_node **cmd_tree, t_shell *sh, bool in_pipe)
{
	if ((*cmd_tree)->type == CMD_NODE && (*cmd_tree)->command.redir_err == 1)
	{
		sh->error = 1;
		return ;
	}
	if ((*cmd_tree)->type == CMD_NODE && (*cmd_tree)->command.command != NULL)
		exec_command(*cmd_tree, sh, in_pipe);
	if ((*cmd_tree)->type == PIPE_NODE)
		exec_pipe(cmd_tree, sh, in_pipe);
	if ((*cmd_tree)->type == AND_NODE)
		exec_and(cmd_tree, sh, in_pipe);
	if ((*cmd_tree)->type == OR_NODE)
		exec_or(cmd_tree, sh, in_pipe);
	if (sh->error == 256)
		sh->error = 1;
}
