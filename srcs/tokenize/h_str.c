/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_str_flag_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:40 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/10 20:43:22 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Returns token and success value, or frees str and value
 * @param s t_hstr struct
 * @param tokens t_token array being updated
 * @return 1 
 */
int	return_handle_str(t_token **tk, t_hstr *s)
{
	if (s->joined && *s->joined)
	{
		if (s->starts_with_quote)
			return (add_token(tk, s->joined, LITERAL), 1);
		else
			return (add_token(tk, s->joined, UNKNOWN), 1);
	}
	else
	{
		if (s->starts_with_quote)
			return (add_token(tk, s->joined, LITERAL), 1);
		else
			return (free_wrap(s->joined), 1);
	}
}

/**
 * @brief Handles the special delimiters \'\"\$
 * @param s t_hstr struct.
 * @param l Input string. 
 * @param p t_parse2, Parsing structure 
 * @param sh t_shell struct
 * @return void
 */
void	split_hand_str(t_hstr *s, t_shell *sh, char *l, t_parse2 *p)
{
	if (s->is_first_char && (l[p->len] == '\'' || l[p->len] == '\"'))
		s->starts_with_quote = 1;
	if (l[p->len] == '\"')
		s->part = h_d_quotes(l, p, sh);
	else if (l[p->len] == '\'')
		s->part = h_literal(l, p);
	else if (l[p->len] == '$')
		s->part = handle_env_var_inline(l + p->len, sh, p);
	else
	{
		s->part = ft_char_to_str(l[p->len]);
		p->len++;
	}
	s->is_first_char = 0;
}

/**
 * @brief Handles the arguments as long as there's no delimiter
 * @param line Input string. 
 * @param p t_parse2, Parsing structure (delimiters, len, 
 * curr_pos and diff being used)
 * @param tokens t_token array being updated
 * @return 1 success, -1 false
 */
int	handle_str(char *l, t_parse2 *p, t_token **tk, t_shell *sh)
{
	t_hstr	s;

	s.starts_with_quote = 0;
	s.is_first_char = 1;
	s.joined = ft_strdup("");
	if (!s.joined)
		return (ft_printf("msh: malloc error\n"), -1);
	while (l[p->len] && !ft_strchr(" |<>()\t\n\v\f\r", l[p->len]))
	{
		if (l[p->len] == '&' && l[p->len + 1] == '&')
			break ;
		split_hand_str(&s, sh, l, p);
		if (!s.part)
			return (free(s.joined), -1);
		s.tmp = s.joined;
		s.joined = ft_strjoin(s.joined, s.part);
		free_multiple(2, s.tmp, s.part, NULL);
		if (!s.joined)
			return (ft_printf("msh: malloc error\n"), -1);
	}
	p->prev_heredoc = 0;
	return (return_handle_str(tk, &s));
}
/*
// 
//  * @brief Handles the literal quoted strings 'string'
//  * @param line Input string. 
//  * @param p t_parse2, Parsing structure (len, curr_pos and diff being used)
//  * @param tokens t_token array being updated
//  * @return 1 or -1 on error
//  
int	handle_literal(char *line, t_parse2 *p, t_token **tokens)
{
	p->len++;
	update_current_position(p);
	while (line[p->len] != '\'' && line[p->len] != '\0')
	p->len++;
	if (line[p->len] == '\'')
	{
		if (line[p->len - 1] == '\'')
		{
			p->len++;
			p->empty = 0;
			return (0);
		}
		p->len++;
		get_diff_curr_len(p);
		add_token(tokens, ft_substr(line, p->curr_pos, p->diff - 1), LITERAL);
		return (1);
	}
	else
	{
		ft_printf("msh: Syntax error: Unterminated quoted string `\''\n");
		return (-1);
	}
}
*/
