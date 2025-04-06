/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_redir_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:32 by sluterea          #+#    #+#             */
/*   Updated: 2025/03/18 18:29:34 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_pipe(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '|') == -1)
		return (ft_printf("minishell: parse error near `|'\n"), -1);
	if (line[parse->len + 1] && line[parse->len + 1] == '|')
	{
		add_token(tokens, ft_substr(line, parse->curr_pos, 2), OR);
		parse->len++;
	}
	else
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), PIPE);
	parse->len++;
	return (1);
}

int	handle_and(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '&') == -1)
		return (ft_printf("minishell: parse error near `&'\n"), -1);
	if (line[parse->len + 1] == '&')
	{
		add_token(tokens, ft_substr(line, parse->curr_pos, 2), AND);
		parse->len++;
	}
	else
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), ARGUMENT);
	parse->len++;
	return (1);
}

int	hand_r_in(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '<') == -1)
		return (ft_printf("minishell: parse error near `<'\n"), -1);
	if (line[parse->len + 1] == '<')
	{
		add_token(tokens, ft_substr(line, parse->curr_pos, 2), HEREDOC);
		parse->len++;
	}
	else
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), REDIR_IN);
	parse->len++;
	return (1);
}

int	hand_r_out(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '>') == -1)
		return (ft_printf("minishell: parse error near `>'\n"), -1);
	if (line[parse->len + 1] == '>')
	{
		add_token(tokens, ft_substr(line, parse->curr_pos, 2), APPEND);
		parse->len++;
	}
	else
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), REDIR_OUT);
	parse->len++;
	return (1);
}

int	handle_brackets(char *line, t_parse2 *parse, t_token **tokens)
{
	if (line[parse->len] == '(')
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), O_BRACKET);
	if (line[parse->len] == ')')
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), C_BRACKET);
	parse->len++;
	return (1);
}
