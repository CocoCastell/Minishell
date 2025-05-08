/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_redir_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:32 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/09 17:54:33 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles the |
 * @param line Input string. 
 * @param parse t_parse2, Parsing structure (len, curr_pos being used)
 * @param tokens t_token array being updated
 * @return 1 on creation, -1 on error
 */
int	handle_pipe(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '|', parse->len) == -1)
	{
		ft_printf(BL"msh: syntax error near unexpected token `|'\n"DEF);
		return (-1);
	}
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

/**
 * @brief Handles the &
 * @param line Input string. 
 * @param parse t_parse2, Parsing structure (len, curr_pos being used)
 * @param tokens t_token array being updated
 * @return 1 on creation, -1 on error
 */
int	handle_and(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '&', parse->len) == -1)
	{
		ft_printf(BL"msh: syntax error near unexpected token `&&'\n"DEF);
		return (-1);
	}
	if (line[parse->len + 1] == '&')
	{
		add_token(tokens, ft_substr(line, parse->curr_pos, 2), AND);
		parse->len++;
	}
	else
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), UNKNOWN);
	parse->len++;
	return (1);
}

/**
 * @brief Handles the redir in delimiter
 * @param line Input string. 
 * @param parse t_parse2, Parsing structure (len, curr_pos being used)
 * @param tokens t_token array being updated
 * @return 1 on creation, -1 on error
 */
int	hand_r_in(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '<', parse->len) == -1)
	{
		ft_printf(BL"msh: Syntax error: redirection unexpected\n"DEF);
		return (-1);
	}
	if (line[parse->len + 1] == '<')
	{
		add_token(tokens, ft_substr(line, parse->curr_pos, 2), HEREDOC);
		parse->len++;
		parse->prev_heredoc = 1;
	}
	else
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), REDIR_IN);
	parse->len++;
	return (1);
}

/**
 * @brief Handles the redir out delimiter
 * @param line Input string. 
 * @param parse t_parse2, Parsing structure (len, curr_pos being used)
 * @param tokens t_token array being updated
 * @return 1 on creation, -1 on error
 */
int	hand_r_out(char *line, t_parse2 *parse, t_token **tokens)
{
	if (find_repeated_delimiter(line, '>', parse->len) == -1)
	{
		ft_printf(BL"msh: syntax error near unexpected token `newline'\n"DEF);
		return (-1);
	}
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

/**
 * @brief Handles the bracket token generation
 * @param line Input string. 
 * @param parse t_parse2, Parsing structure (len, curr_pos being used)
 * @param tokens t_token array being updated
 * @return 1
 */
int	handle_brackets(char *line, t_parse2 *parse, t_token **tokens)
{
	if (line[parse->len] == '(')
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), O_BRACKET);
	if (line[parse->len] == ')')
		add_token(tokens, ft_substr(line, parse->curr_pos, 1), C_BRACKET);
	parse->len++;
	return (1);
}
