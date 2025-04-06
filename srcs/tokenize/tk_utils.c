/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:07 by sluterea          #+#    #+#             */
/*   Updated: 2025/03/31 15:08:12 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

enum e_type	check_type(t_token *token)
{
	t_token	*current;

	current = token;
	if (!current->prev)
		return (COMMAND);
	else if (current->prev->type == PIPE || current->prev->type == O_BRACKET)
		return (COMMAND);
	else if (current->prev->type == AND || current->prev->type == OR)
		return (COMMAND);
	else
	{
		if (ft_strchr(token->str, '*'))
			return (WILDCARD);
		return (ARGUMENT);
	}
}

void	get_diff_curr_len(t_parse2 *parse)
{
	parse->diff = parse->len - parse->curr_pos;
}

void	update_current_position(t_parse2 *parse)
{
	parse->curr_pos = parse->len;
}

/**
 * @brief Checks for repeated patterns ($$$, >>>, <<<, |||)
 * @param line Input string to check
 * @return The found pattern character ('$', '>', '<', '|') or 0 if none found
 */
/*char	find_repeated_pattern(const char *line)
{
	const char	patterns[] = "&><|";
	size_t		i;
	size_t		len;
	int			j;

	i = 0;
	if (!line)
		return ('n');
	len = strlen(line);
	while (i < len - 2)
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			j = line[i];
			i++;
			while (i < len && line[i] != j)
				i++;
			if (i >= len)
				break ;
			i++;
			continue ;
		}
		if (line[i] == line[i + 1] && line[i] == line[i + 2])
		{
			if (strchr(patterns, line[i]))
				return (line[i]);
		}
		i++;
	}
	return (0);
}*/

/**
 * @brief Checks for repeated character (for: $, >, <, |)
 * @param l, Input line to check
 * @param c, c Input char to check
 * @return 1 if is repeated, 0 if it's not
 * 
 */
int	find_repeated_delimiter(char *l, char c)
{
	size_t	i;

	i = 0;
	if (!c)
		return (0);
	if (l[i + 1] && l[i + 1] == c && l[i + 2] && l[i + 2] == c)
		return (-1);
	return (0);
}
