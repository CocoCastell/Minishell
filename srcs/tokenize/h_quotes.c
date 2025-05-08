/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_nospace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:50:15 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/14 10:50:18 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles the literal quoted strings 'string'
 * @param line Input string. 
 * @param p t_parse2, Parsing structure (len, curr_pos and diff being used)
 * @return NULL on error, str on creation, "\0" on empty (no error)
 */
char	*h_literal(char *l, t_parse2 *p)
{
	char	*lit;
	int		pos;

	p->len++;
	pos = p->len;
	while (l[p->len] != '\'' && l[p->len] != '\0')
		p->len++;
	if (l[p->len] == '\'')
	{
		lit = ft_substr(l, pos, p->len - pos);
		if (!lit)
			return (ft_printf("msh: parsing error\n"), NULL);
		p->len++;
		return (lit);
	}
	else
	{
		ft_printf("msh: unexpected EOF while looking for matching `\''\n");
		return (NULL);
	}
}

/**
  * @brief Handles the double quotes
  * @param line Input string. 
  * @param p t_parse2, Parsing structure (len being used)
  * @param d t_ns handle double quote Struct (all being used)
  * @param sh t_shell Struct
  * @return 1 on creation, -1 on error
  */
static void	norm_split_condition(char *line, t_parse2 *p, t_ns *d, t_shell *sh)
{
	if (line[p->len] == '$')
	{
		p->is_dquote = 1;
		d->env_value = handle_env_var_inline(line + p->len, sh, p);
		p->is_dquote = 0;
		d->temp = d->str;
		d->str = ft_strjoin(d->str, d->env_value);
		free(d->temp);
		free(d->env_value);
	}
	else
	{
		d->temp = d->str;
		d->char_str = ft_char_to_str(line[p->len]);
		d->str = ft_strjoin(d->str, d->char_str);
		free(d->temp);
		free(d->char_str);
		p->len++;
	}
	d->pos = p->len;
}

/**
 * @brief Handles the double quotes
 * @param line Input string. 
 * @param parse t_parse2, Parsing structure (len being used)
 * @param sh t_shell Struct
 * @return NULL on error, str on creation, "\0" on empty (no error)
 */
char	*h_d_quotes(char *l, t_parse2 *p, t_shell *sh)
{
	t_ns	d;

	d.str = ft_strdup("");
	if (!d.str)
		return (ft_printf("msh: error on parsing\n"), NULL);
	p->len++;
	while (l[p->len] && l[p->len] != '\"')
		norm_split_condition(l, p, &d, sh);
	if (l[p->len] != '\"')
	{
		free(d.str);
		ft_printf("msh: unexpected EOF while looking for matching `\"'\n");
		return (NULL);
	}
	p->len++;
	return (d.str);
}
