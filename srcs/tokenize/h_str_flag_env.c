/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_str_flag_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:40 by sluterea          #+#    #+#             */
/*   Updated: 2025/03/27 17:06:57 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	h_env_var(char *line, t_parse2 *p, t_token **tk, t_shell *sh)
{
	char			*str;

	if (line[p->len + 1] == '?')
	{
		str = ft_itoa(sh->error);
		p->len += 2;
		add_token(tk, str, UNKNOWN);
	}
	else
	{
		while (!ft_strchr(p->delimiters, line[p->len + 1]))
			p->len++;
		get_diff_curr_len(p);
		str = ft_getenv(sh->env, ft_substr(line, p->curr_pos + 1, p->diff));
		if (!str)
			str = ft_strdup("");
		p->len++;
		add_token(tk, str, UNKNOWN);
	}
	return (1);
}

int	handle_flag(char *line, t_parse2 *parse, t_token **tokens)
{
	while (!ft_strchr(parse->delimiters, line[parse->len + 1]))
		parse->len++;
	parse->len++;
	get_diff_curr_len(parse);
	add_token(tokens, ft_substr(line, parse->curr_pos, parse->diff), FLAG);
	return (1);
}

int	handle_literal(char *line, t_parse2 *p, t_token **tokens)
{
	p->len++;
	update_current_position(p);
	while (line[p->len] != '\'')
		p->len++;
	p->len++;
	get_diff_curr_len(p);
	add_token(tokens, ft_substr(line, p->curr_pos, p->diff - 1), UNKNOWN);
	return (1);
}

void	handle_str(char *line, t_parse2 *p, t_token **tokens)
{
	while (!strchr(p->delimiters, line[p->len])
		&& line[p->len] != '\0')
		p->len++;
	get_diff_curr_len(p);
	add_token(tokens, ft_substr(line, p->curr_pos, p->diff), UNKNOWN);
}
