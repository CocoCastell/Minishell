/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_double_q.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:22 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/05 13:40:56 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* void	handle_wildcard(char *line, t_parse2 *parse, t_token **tokens) */
/* { */
	/* char	*str; */

/* } */

char	*handle_env_var_quote(char *line, t_parse2 *parse, t_shell *sh)
{
	t_var_content	var;
	char			*str;

	var.value = NULL;
	var.len = 0;
	update_current_position(parse);
	parse->len++;
	if (line[parse->len] == '?')
	{
		var.value = ft_itoa(sh->error);
		parse->len++;
	}
	else
	{
		while (!ft_strchr(parse->delimiters, line[parse->len]))
			parse->len++;
		get_diff_curr_len(parse);
		str = ft_substr(line, parse->curr_pos + 1, parse->diff - 1);
		var.value = ft_getenv(sh->env, str);
		if (!var.value)
			var.value = ft_strdup("");
		free(str);
	}
	return (var.value);
}

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

static void	norm_split_condition(char *line, t_parse2 *p, t_h_d *d, t_shell *sh)
{
	if (line[p->len] == '$')
	{
		d->env_value = handle_env_var_quote(line, p, sh);
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
	update_current_position(p);
}

int	handle_d_q(char *line, t_parse2 *parse, t_token **tk, t_shell *sh)
{
	t_h_d	d;

	d.str = ft_strdup("");
	parse->len++;
	while (line[parse->len] && line[parse->len] != '\"')
		norm_split_condition(line, parse, &d, sh);
	add_token(tk, d.str, UNKNOWN);
	return (1);
}
