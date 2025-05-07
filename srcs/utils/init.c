/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:55:32 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/14 15:55:58 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*display_prompt(char *curr_dir, t_shell *sh)
{
	char	*input;
	char	*prompt;
	char	*tmp_prompt;

	(void)sh;
	tmp_prompt = ft_strjoin("\001\033[0;31m\002", curr_dir);
	prompt = ft_strjoin(tmp_prompt, "\001\033[0m\002 > ");
	free(tmp_prompt);
	input = readline(prompt);
	free(prompt);
	return (input);
}

void	ft_add_history(char *input)
{
	int	i;
	int	is_char;

	i = 0;
	is_char = 0;
	while (input[i] != '\0')
	{
		if (ft_isprint(input[i]) == 1 && input[i] != 32)
			is_char = 1;
		i++;
	}
	if (is_char == 1)
		add_history(input);
}

char	*init_prompt(char *curr_dir, t_shell *sh)
{
	char	*input;

	input = display_prompt(curr_dir, sh);
	if (!input)
	{
		rl_clear_history();
		ft_free_string_array(sh->env);
		exit(0);
	}
	ft_add_history(input);
	return (input);
}

// lvls[0] = lvl
// lvls[1] = new_lvl
// lvls[2] = up_lvl
void	init_sh(t_shell *sh, char *env[])
{
	char	*lvls[3];
	char	*color;

	sh->tree = NULL;
	sh->tk = NULL;
	sh->input = NULL;
	sh->dir = NULL;
	sh->std[0] = dup(STDIN_FILENO);
	sh->std[1] = dup(STDOUT_FILENO);
	sh->env = ft_create_env(env);
	lvls[0] = ft_getenv(sh->env, "SHLVL");
	if (lvls[0])
	{
		lvls[1] = ft_itoa(ft_atoi(lvls[0]) + 1);
		lvls[2] = ft_strjoin("SHLVL=", lvls[1]);
		ft_update_env(sh, lvls[2]);
		free_multiple(3, lvls[0], lvls[1], lvls[2]);
	}
	color = ft_strdup(get_ls_color());
	ft_update_env(sh, color);
	free(color);
	print_banner();
	manage_signals();
	sh->dir = ft_getpwd();
	sh->error = 0;
}
