/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:55:32 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/04 15:12:19 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Prints the minishell banner
 * @param void
 * @return void
 */
void	print_banner(void)
{
	printf(WHITE
		"\n			       _______ _______ ______ _______ \n"
		"                              (_______|_______|_____ (______ \\ \n"
		RED"                               _       _______ _____) )_____) ) \n"
		"                              | |     |  ___  |  __  /|  ____/  \n"
		"                              | |_____| |   | | |  | || |  \n"
		WHITE"                              \\_______)_|   |_|_|  |_||_|   \n");
	printf("\n");
	printf(RED"\t\t\t___________________________________________\n"WHITE);
	printf("\n");
}

/**
 * @brief Returns color
 * @param void
 * @return color string
 */
char	*get_ls_color(void)
{
	char	*ls_color;

	ls_color = "LS_COLORS=rs=0:ex=01;38;2;247;209;23:fi=38;2;255;255;255:"
		"di=01;38;2;115;173;225:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:"
		"bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00;su=37;41:sg=30;43:"
		"ca=30;41:tw=30;42:ow=34;42:st=37;44:";
	return (ls_color);
}

/**
 * @brief Surprise surprise
 * @return void
 */
void	ft_sorpresa(void)
{
	printf("🩵🤍🩵 🇦🇷 MESSI 🧉 🇦🇷 🩵🤍🩵\n");
}

/**
 * @brief Frees str array.
 * @param s string array to free
 * @param i iterator where it left
 * @return void
 */
void	free_str(char **s, int i)
{
	while (i > 0)
	{
		free(s[i - 1]);
		i--;
	}
	free(s);
}
