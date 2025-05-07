/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:27:53 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/22 11:20:53 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	signal_handle(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		exit(130);
	}
}

int	handle_redir_out(char *redir_out, char *append, t_shell *sh, int i)
{
	char	**all_redir;
	char	**all_app;
	int		fd;

	all_redir = ft_split(redir_out, '\n');
	all_app = ft_split(append, '\n');
	while (all_redir[i] != NULL)
	{
		if (*all_app[i] == '1')
			fd = open(all_redir[i], O_RDWR | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(all_redir[i], O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			sh->error = 1;
			return (perror("Error opening file"), 1);
		}
		i++;
	}
	ft_free_string_array(all_redir);
	ft_free_string_array(all_app);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), perror("Dup2 error"), 1);
	return (close(fd), 0);
}

int	handle_redir_in(char *redir_in, t_shell *sh, int i)
{
	char	**all_redir;
	int		fd;

	all_redir = ft_split(redir_in, '\n');
	while (all_redir[i] != NULL)
	{
		fd = open(all_redir[i], O_RDONLY);
		if (fd == -1)
		{
			sh->error = 1;
			return (perror("Error opening file"), 1);
		}
		i++;
	}
	ft_free_string_array(all_redir);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), perror("Dup2 error"), 1);
	return (close(fd), 0);
}

int	exec_redir(t_node *cmd_tree, t_shell *sh)
{
	if (cmd_tree->command.heredoc != NULL)
	{
		if (handle_heredoc(cmd_tree->command.heredoc, sh) == 1)
			return (1);
	}
	if (cmd_tree->command.redir_out != NULL && handle_redir_out(\
		cmd_tree->command.redir_out, cmd_tree->command.append, sh, 0) == 1)
		return (1);
	if (cmd_tree->command.redir_in != NULL && handle_redir_in(\
				cmd_tree->command.redir_in, sh, 0) == 1)
		return (1);
	return (0);
}

/* OLD HANDLE_HEREDOC */
/*void	handle_heredoc(char *args, t_shell *sh)
{
	pid_t	pid;
	char	**hd;
	int		pipe_fd[2];
	int		status;
	int		i;

	i = 0;
	void (*old_sigint)(int);
	void (*old_sigquit)(int);    
	hd = ft_split(args, '\n');
	if (pipe(pipe_fd) == -1)
		return; // error
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, signal_handle);
		signal(SIGQUIT, SIG_IGN);
		close(pipe_fd[0]);
		while (hd[i])
		{
			bool quoted = is_quoted(hd[i]);
			char *clean_hd = strip_quotes(hd[i]);
			heredoc_loop(pipe_fd, clean_hd, quoted, sh);
			free_wrap(clean_hd);
			i++;
		}
		ft_free_string_array(hd);
		close(pipe_fd[1]);
		exit(0);
	}
	else
	{
		old_sigint = signal(SIGINT, SIG_IGN);
		old_sigquit = signal(SIGQUIT, SIG_IGN);
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		ft_free_string_array(hd);
		signal(SIGINT, old_sigint);
		signal(SIGQUIT, old_sigquit);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			g_signal_received = 1;
			close(pipe_fd[0]);
			return ;
		}
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
}*/
