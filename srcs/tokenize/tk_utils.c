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

char	*ft_add_quotes(const char *input)
{
	size_t	len;
	char	*result;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = malloc(len + 3);
	if (result == NULL)
		return (perror("Failed to allocate memory"), NULL);
	result[0] = '\'';
	ft_strlcpy(result + 1, input, len + 1);
	result[len + 1] = '\'';
	result[len + 2] = '\0';
	return (result);
}

/**
 * @brief Calculates token's type.
 * @param token t_token being checked
 * @return enum e_type
 */
enum e_type	check_type(t_token *token)
{
	t_token	*current;
	char	*tmp;

	current = token;
	if (!current->prev)
		return (COMMAND);
	else if (current->prev->type == PIPE || current->prev->type == O_BRACKET)
		return (COMMAND);
	else if (current->prev->type == AND || current->prev->type == OR)
		return (COMMAND);
	else
	{
		if (ft_strchr(token->str, '*') && token->type != LITERAL)
			return (WILDCARD);
		if (token->str[0] == '-' && token->str[1] && token->str[1] != '-')
			return (FLAG);
		if (token->type == LITERAL && current->prev->type == HEREDOC)
		{
			tmp = ft_add_quotes(token->str);
			free_wrap(token->str);
			token->str = tmp;
		}
		return (ARGUMENT);
	}
}

/**
 * @brief Calculates the difference between len and current position.
 * @param parse t_parse2, Parsing structure (len, curr_pos being used)
 * @return void (Updates diff)
 */
void	get_diff_curr_len(t_parse2 *parse)
{
	parse->diff = parse->len - parse->curr_pos;
}

/**
 * @brief Updates current position. Necessary for the loop condition.
 * @param parse t_parse2, Parsing structure (len, curr_pos being used)
 * @return void (Updates current position)
 */
void	update_current_position(t_parse2 *parse)
{
	parse->curr_pos = parse->len;
}

/**
 * @brief Checks for repeated character (for: $, >, <, |)
 * @param l, Input line to check
 * @param c, c Input char to check
 * @return 1 if is repeated, 0 if it's not
 * 
 */
int	find_repeated_delimiter(char *l, char c, int i)
{
	if (!c)
		return (0);
	if (l[i + 1] && l[i + 1] == c && l[i + 2] && l[i + 2] == c)
		return (-1);
	return (0);
}
