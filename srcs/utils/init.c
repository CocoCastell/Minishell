/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:55:32 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/08 14:12:23 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Displays the prompt with colors.
 * @param curr_dir string representing the current dir
 * @param sh t_shell structure to init
 * @return input string
 */
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

/**
 * @brief Adds the last input to the history if valid
 * @param input string representing the input
 * @return 0 if not an empty string, 1 otherwise
 */
int	ft_add_history(char *input)
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
		return(add_history(input), 0);
	return (1);
}

/**
 * @brief Inits the prompt for minishell. also exits on empty input (ctrl + D).
 * @param curr_dir string representing the current dir
 * @param sh t_shell structure to init
 * @return input string or NULL if only spaces
 */
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
	if (ft_add_history(input) == 0)
		return (input);
	return (NULL);
}

/**
 * @brief Sets the empty input error to 0 if empty, else 2.
 * // lvls[0] = lvl lvls[1] = new_lvl lvls[2] = up_lvl
 * @param sh t_shell structure to init
 * @param env environment variables
 * @return void
 */
void	init_sh(t_shell *sh, char *env[])
{
	char	*lvls[3];
	char	*color;

	sh->tree = NULL;
	sh->tk = NULL;
	sh->input = NULL;
	sh->dir = NULL;
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
