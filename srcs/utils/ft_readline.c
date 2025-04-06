/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:25:22 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/04 13:25:43 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_readline(char *prompt, t_shell *sh)
{
	char	*line;
	char	*result;
	size_t	len;

	if (prompt)
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	line = get_next_line(STDIN_FILENO, &sh->error);
	if (g_signal_received)
	{
		g_signal_received = 0;
		if (line)
			free(line);
		return (ft_strdup(""));
	}
	if (!line)
		return (write(1, "\n", 1), NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	result = ft_strdup(line);
	return (free(line), result);
}
