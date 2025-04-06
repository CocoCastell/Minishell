/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:55:32 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 12:46:57 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
void	*display_prompt(char *curr_dir, t_shell *sh)
{
	char	*input;
	char	*prompt;
	char	*tmp_prompt;

	(void)sh;
	tmp_prompt = ft_strjoin(RED, curr_dir);
	prompt = ft_strjoin(tmp_prompt, WHITE" > ");
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

//char	*init_prompt(char *curr_dir, t_shell *sh)
{
	char	*input;

	input = display_prompt(curr_dir, sh);
	if (!input)
	{
		//rl_clear_history();
		ft_free_string_array(sh->env);
		exit(0);
	}
	ft_add_history(input);
	return (input);
}
*/
void	*display_prompt(char *curr_dir, t_shell *sh)
{
	char	*input;
	char	*prompt;
	char	*tmp_prompt;

	(void)sh;
	tmp_prompt = ft_strjoin(RED, curr_dir);
	prompt = ft_strjoin(tmp_prompt, WHITE" > ");
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

char *init_prompt(char *curr_dir, t_shell *sh)
{
	char	*input;
	char	*prompt;
	char	*tmp_prompt;

	tmp_prompt = ft_strjoin(RED, curr_dir);
	prompt = ft_strjoin(tmp_prompt, WHITE" > ");
	free(tmp_prompt);
	input = ft_readline(prompt, sh);
	free(prompt);
	if (!input)
	{
		ft_free_string_array(sh->env);
		exit(0);
	}
	ft_add_history(input);
	return (input);
}

void	init_sh(t_shell *sh, char *env[])
{
	char	*lvl;
	char	*new_lvl;
	char	*up_lvl;
	char	*color;

	sh->tree = NULL;
	sh->tk = NULL;
	sh->input = NULL;
	sh->dir = NULL;
	sh->env = ft_create_env(env);
	lvl = ft_getenv(sh->env, "SHLVL");
	if (lvl)
	{
		new_lvl = ft_itoa(ft_atoi(lvl) + 1);
		up_lvl = ft_strjoin("SHLVL=", new_lvl);
		ft_update_env(sh, up_lvl);
		free_multiple(3, lvl, new_lvl, up_lvl);
	}
	color = ft_strdup(get_ls_color());
	ft_update_env(sh, color);
	free(color);
	print_banner();
	manage_signals();
	sh->dir = ft_getpwd_s();
	sh->error = 0;
}
