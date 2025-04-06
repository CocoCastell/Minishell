/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:55:32 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 13:13:17 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*add_token(t_token **tokens, char *str, enum e_type type)
{
	t_token	*new_token;
	t_token	*last_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (perror("Failed to allocate memory for new token"), NULL);
	new_token->str = ft_strdup(str);
	new_token->next = NULL;
	new_token->prev = NULL;
	if (*tokens == NULL)
		*tokens = new_token;
	else
	{
		last_token = *tokens;
		while (last_token->next != NULL)
			last_token = last_token->next;
		last_token->next = new_token;
		new_token->prev = last_token;
	}
	if (type == UNKNOWN)
		new_token->type = check_type(new_token);
	else
		new_token->type = type;
	return (free(str), new_token);
}

void	h_del(char *line, t_parse2 *parse, t_token **tk, t_shell *sh)
{
	if (line[parse->len] == ' ')
		parse->len++;
	else if (line[parse->len] == '-')
		parse->flag = handle_flag(line, parse, tk);
	else if (line[parse->len] == '$')
		parse->flag = h_env_var(line, parse, tk, sh);
	else if (line[parse->len] == '|')
		parse->flag = handle_pipe(line, parse, tk);
	else if (line[parse->len] == '&')
		parse->flag = handle_and(line, parse, tk);
	else if (line[parse->len] == '<')
		parse->flag = hand_r_in(line, parse, tk);
	else if (line[parse->len] == '>')
		parse->flag = hand_r_out(line, parse, tk);
	else if (line[parse->len] == '\'')
		parse->flag = handle_literal(line, parse, tk);
	else if (line[parse->len] == '"')
		parse->flag = handle_d_q(line, parse, tk, sh);
	else if (line[parse->len] == '(' || line[parse->len] == ')')
		parse->flag = handle_brackets(line, parse, tk);
	/* else if (line[parse->len] == '*') */
	/* 	parse->flag = handle_wildcard(line, parse, tk); */
}

void	init_tokenize_parse(t_parse2 *p)
{
	p->delimiters = ft_strdup(" |<>&$\"'-()");
	p->flag = 1;
	p->count = 0;
	p->len = 0;
	p->curr_pos = 0;
	p->last_pos = 0;
	p->error = 0;
	p->diff = 0;
}

t_token	**tokenize(char *line, t_shell *sh)
{
	t_parse2	p_info;
	t_token		**tokens;

	if (!line || line[0] == '\0')
		return (NULL);
	init_tokenize_parse(&p_info);
	tokens = malloc (sizeof(t_token *));
	if (!tokens)
		return (perror("Failed to allocate memory for tokens"), NULL);
	*tokens = NULL;
	while (line[p_info.len] != '\0')
	{
		if (p_info.flag == -1)
			return (free(p_info.delimiters), free_tokens(tokens), NULL);
		if (ft_strchr(p_info.delimiters, line[p_info.len]))
			h_del(line, &p_info, tokens, sh);
		else
			handle_str(line, &p_info, tokens);
		update_current_position(&p_info);
		get_diff_curr_len(&p_info);
	}
	return (free(p_info.delimiters), tokens);
}
