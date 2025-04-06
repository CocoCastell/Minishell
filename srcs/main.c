/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 18:50:12 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;
// print_tree(sh.tree, 0);
int	main(int argc, char *argv[], char *env[])
{
	t_shell	sh;
	t_token	*tk_cpy;
	t_node	*tree_ptr;

	// Problemas con space y tabs y erase
	// echo -n no funciona
	// ctrl-\ deberia quittar si se escritó algo?
	manage_signals();
	init_sh(&sh, env);
	while (1)
	{
		sh.input = init_prompt(sh.dir, &sh);
		sh.tk = tokenize(sh.input, &sh);
		if (sh.tk)
		{
			tk_cpy = *(sh.tk);
			sh.tree = parsing(&tk_cpy);
			free_tokens(sh.tk);
			tree_ptr = sh.tree;
			if (sh.tree != NULL && sh.tree->error == 0)
				recursive_exec(&tree_ptr, &sh, false);
			else
				sh.error = 2;
			free_sh(&sh);
		}
		else
			free(sh.input);
		sh.input = NULL;
	}
	return ((void)argc, (void)argv, EXIT_SUCCESS);
}
