/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/07 17:27:56 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Sets the empty input error to 0 if empty, else 2
 * @param sh t_shell to modify sh->error
 */
void	set_error(t_shell *sh, int error)
{
	int	i;

	i = 0;
	while (sh->input[i] == 32 || sh->input[i] == '\t')
		i++;
	if (error == 130)
		sh->error = 130;
	else if (sh->input[i] == '\0')
		sh->error = 0;
	else
		sh->error = 2;
}

/**
 * @brief Gets current working directory shortened path (static char*)
 * @param long_dir The full directory path to be shortened
 * @return Shortened directory path, or NULL if input is NULL
 */
char	*ft_shorten_dir(char *long_dir)
{
	static char	cwd[5000];
	char		*last_slash;

	if (!long_dir)
		return (NULL);
	ft_strlcpy(cwd, long_dir, sizeof(cwd));
	last_slash = strrchr(cwd, '/');
	if (last_slash != NULL && *(last_slash + 1) != '\0')
		return (last_slash + 1);
	return (cwd);
}

void	handle_tokens_and_execute(t_main *m)
{
	m->tk_cpy = *(m->sh.tk);
	m->sh.tree = parsing(&m->tk_cpy, &m->sh);
	free_tokens(m->sh.tk);
	m->tree_ptr = m->sh.tree;
	if (m->sh.tree != NULL && m->sh.tree->error == 0)
		recursive_exec(&m->tree_ptr, &m->sh, false);
	else
		set_error(&m->sh, m->sh.tree->error);
	free_sh(&m->sh);
}

int	main(int argc, char *argv[], char *env[])
{
	t_main	m;

	init_sh(&m.sh, env);
	while (1)
	{
		manage_signals();
		m.sh.input = init_prompt(ft_shorten_dir(m.sh.dir), &m.sh);
		m.sh.tk = tokenize(m.sh.input, &m.sh);
		if (m.sh.tk)
			handle_tokens_and_execute(&m);
		else
			free_wrap(m.sh.input);
		m.sh.input = NULL;
	}
	ft_free_string_array(m.sh.env);
	return ((void)argc, (void)argv, EXIT_SUCCESS);
}
