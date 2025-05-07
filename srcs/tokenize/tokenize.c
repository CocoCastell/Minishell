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

/**
 * @brief Adds a token to the array
 * @param tokens t_token array being updated
 * @param str Content of the new token. FREES str passed. 
 * @param type e_type, sets the new token type
 * @return -1 if wrong args, 0 on finish
 */
t_token	*add_token(t_token **tokens, char *str, enum e_type type)
{
	t_token	*new_token;
	t_token	*last_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (perror("Failed to allocate memory for new token\n"), NULL);
	new_token->str = ft_strdup(str);
	if (!new_token->str)
		return (perror("Strdupp error\n"), NULL);
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
	new_token->type = type;
	if (type == UNKNOWN || type == LITERAL)
		new_token->type = check_type(new_token);
	return (free(str), new_token);
}

/**
 * @brief Adds a token to the array
 * @param l Content of the new token. FREES str passed. 
 * @param p t_parse2 Parsing structure. 
 * @param tokens t_token array
 * @param sh t_shell shell structure
 * @return void
 */
void	h_del(char *l, t_parse2 *p, t_token **tk, t_shell *sh)
{
	if (l[p->len] == ' ' || (l[p->len] >= 9 && l[p->len] < 14))
		p->len++;
	else if (l[p->len] == '|')
		p->flag = handle_pipe(l, p, tk);
	else if (l[p->len] == '&')
		p->flag = handle_and(l, p, tk);
	else if (l[p->len] == '<')
		p->flag = hand_r_in(l, p, tk);
	else if (l[p->len] == '>')
		p->flag = hand_r_out(l, p, tk);
	else if (l[p->len] == '\'')
		p->flag = handle_literal(l, p, tk);
	else if (l[p->len] == '"')
		p->flag = handle_d_q(l, p, tk, sh);
	else if (l[p->len] == '(' || l[p->len] == ')')
		p->flag = handle_brackets(l, p, tk);
}

/**
 * @brief Initializes all the parsing structure variables
 * @return 1 on success, -1 on alloc error
 */
int	init_tokenize_parse(t_parse2 *p)
{
	p->delimiters = ft_strdup(" |<>&()\t\n\v\f\r");
	if (!p->delimiters)
		return (perror("Strdup error\n"), -1);
	p->flag = 1;
	p->count = 0;
	p->len = 0;
	p->curr_pos = 0;
	p->last_pos = 0;
	p->error = 0;
	p->diff = 0;
	p->empty = 0;
	return (1);
}

/**
 * @brief Adds parsing error value to sh struct (optionally stderr)
 */
void	handle_lexing_error(t_shell *sh)
{
	sh->error = 2;
}

/**
 * @brief Splits input into tokens
 * @param l User input. 
 * @param sh t_shell Shell structure
 * @return tokens array or NULL
 */
t_token	**tokenize(char *line, t_shell *sh)
{
	t_parse2	p_info;
	t_token		**tokens;

	if (!line || line[0] == '\0')
		return (NULL);
	if (init_tokenize_parse(&p_info) == -1)
		return (NULL);
	tokens = malloc (sizeof(t_token *));
	if (!tokens)
		return (perror("Failed to allocate memory for tokens"), NULL);
	*tokens = NULL;
	while (line[p_info.len] != '\0')
	{
		if (ft_strchr(p_info.delimiters, line[p_info.len]))
			h_del(line, &p_info, tokens, sh);
		else
			p_info.flag = handle_str(line, &p_info, tokens, sh);
		update_current_position(&p_info);
		get_diff_curr_len(&p_info);
		if (p_info.flag == -1)
			return (free(p_info.delimiters), free_tokens(tokens), \
			handle_lexing_error(sh), NULL);
	}
	return (free(p_info.delimiters), tokens);
}
