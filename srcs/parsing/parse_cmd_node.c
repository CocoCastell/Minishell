/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:56:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 18:57:05 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_node	*return_cmd_node(char *arg_line, t_cmd_node cmd, t_node *cmd_node)
{
	char		*temp;

	temp = ft_strdup(arg_line);
	free_wrap(arg_line);
	if (temp != NULL)
		cmd.args = ft_split(temp, '\n');
	free_wrap(temp);
	cmd_node->command = cmd;
	return (cmd_node);
}

void	put_ls_color_flag(char **arg_line)
{
	char	*tmp;
	char	*ls_line;

	tmp = ft_strjoin(*arg_line, "\n");
	if (tmp == NULL)
		return ; //error
	free_wrap(*arg_line);
	ls_line = ft_strjoin(tmp, "--color=always");
	free_wrap(tmp);
	*arg_line = ft_strdup(ls_line);
	free_wrap(ls_line);
}

void	put_wildcard(char **arg_line, t_token **tk)
{
	bool	is_first_wcard;
	char	*wildcard_files;
	char	*tmp;

	is_first_wcard = false;
	tmp = ft_strjoin(*arg_line, "\n");
	free_wrap(*arg_line);
	if ((*tk)->str[0] == '*')
		is_first_wcard = true;
	wildcard_files = parse_wildcard((*tk)->str, is_first_wcard);
	*arg_line = ft_strjoin(tmp, wildcard_files); 
	free_wrap(wildcard_files);
	free_wrap(tmp);
}

void	put_cmd_arg(char **cmd, t_token **tk, char **ag_l)
{
	char	*new_line;

	if ((*tk)->type == COMMAND)
	{
		*cmd = ft_strdup((*tk)->str);
		*ag_l = ft_strdup((*tk)->str);
		if (ft_strncmp(*cmd, "ls", 3) == 0 || \
				ft_strncmp(*cmd, "/bin/ls", 8) == 0)
			put_ls_color_flag(ag_l);
	}
	else
	{
		if ((*tk)->type == ARGUMENT)
		{
			new_line = ft_strjoin(*ag_l, "\n");
			if (new_line == NULL)
				return ; //error
			free_wrap(*ag_l);
			*ag_l = ft_strjoin(new_line, (*tk)->str);
			free_wrap(new_line);
		}
		else
			put_wildcard(ag_l, tk);
	}
}
