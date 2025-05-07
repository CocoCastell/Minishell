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
		d->env_value = handle_env_var_inline(line + p->len, sh, p);
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
char	*h_d_q_ns(char *l, t_parse2 *p, t_shell *sh)
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

/**
 * @brief Handles the literal quoted strings 'string'
 * @param line Input string. 
 * @param p t_parse2, Parsing structure (len, curr_pos and diff being used)
 * @return NULL on error, str on creation, "\0" on empty (no error)
 */
char	*h_l_ns(char *l, t_parse2 *p)
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
 * @brief Handles the arguments as long as there's no delimiter
 * @param line Input string. 
 * @param p t_parse2, Parsing structure (delimiters, len, 
 * curr_pos and diff being used)
 * @param tokens t_token array being updated
 * @return NULL on error, str on creation, "\0" on empty (no error)
 */
char	*h_str_ns(char *l, t_parse2 *p, t_shell *sh)
{
	char	*tmp;
	char	*expanded;
	int		pos;

	pos = p->len;
	while (l[p->len] != '\0' && !ft_strchr(" |<>'\"()\t\n\v\f\r", l[p->len]))
	{
		if (l[p->len] == '&' && l[p->len + 1] && l[p->len + 1] == '&')
			break ;
		p->len++;
	}
	tmp = ft_substr(l, pos, p->len - pos);
	if (!tmp)
		return (ft_printf("msh: error on parsing\n"), NULL);
	if (ft_strchr(tmp, '$'))
	{
		expanded = handle_env_var_expand(tmp, sh);
		if (expanded)
			return (free(tmp), expanded);
		else
			return (free(tmp), ft_printf("msh: error on parsing\n"), NULL);
	}
	else
		return (tmp);
}
