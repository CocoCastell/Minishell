/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_utils_env_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:14:29 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/10 15:14:31 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Creates a string out of a char. 'c' -> "c"
 * @param c Char to use
 * @return "c" string
 */
char	*ft_char_to_str(char c)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * 2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

/**
 * @brief Initialize t_expand structure values
 * @param ex t_expand Struct
 * @return void
*/
void	init_expand_struct(t_expand *ex)
{
	ex->var = NULL;
	ex->str = ft_strdup("");
	ex->new_str = NULL;
	ex->j = 0;
	ex->count = 0;
	ex->env_var = NULL;
}
