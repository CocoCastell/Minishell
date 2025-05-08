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

/**
 * @brief Handles shell specials. `$?`, `$#`, and 'literal' chars.
 * @param ex t_expand Struct
 * @param sh t_shell Struct (error being used)
 * @param del delimiter to decide action
 * @param line original string to be expanded. 
 * @return void (expands string value)
*/
void	handle_specials(t_expand *ex, t_shell *sh, char del, char *line)
{
	if (del == '?')
		ex->var = ft_itoa(sh->error);
	else if (del == '#')
		ex->var = ft_strdup("0");
	else
		ex->var = ft_char_to_str(line[ex->j]);
	ex->new_str = ft_strjoin(ex->str, ex->var);
	free_wrap(ex->str);
	ex->str = ex->new_str;
	free_wrap(ex->var);
	ex->var = NULL;
	ex->j++;
}

/**
 * @brief Handles valid environment variable names.
 * @param ex t_expand Struct
 * @param sh t_shell Struct (error being used)
 * @param line original string to be expanded. 
 * @return void (expands string value or not if no valid env name)
*/
void	handle_valid_case(t_expand *ex, t_shell *sh, char *line)
{
	ex->count = ex->j;
	while (ft_isalnum(line[ex->j]) || line[ex->j] == '_')
		ex->j++;
	ex->var = ft_substr(line, ex->count, ex->j - ex->count);
	ex->env_var = ft_getenv(sh->env, ex->var);
	if (!ex->env_var)
		ex->env_var = ft_strdup("");
	ex->new_str = ft_strjoin(ex->str, ex->env_var);
	free_wrap(ex->str);
	ex->str = ex->new_str;
	free_wrap(ex->env_var);
	free_wrap(ex->var);
	ex->var = NULL;
}

/**
 * @brief Handles (not)adding $ delimiter.
 * @param ex t_expand Struct
 * @param l string
 * @param p t_parse2 Struct
 * @return void (expands string value)
*/
void	handle_else(t_expand *ex, char *l, t_parse2 *p)
{
	if (
		(ft_strchr(" <>|()", l[ex->j]))
		|| ((l[ex->j] == '"' || l[ex->j] == '\'')
			&& (l[ex->j - 2] && !ft_strchr(" <>|()", l[ex->j - 2]))
			&& p->is_dquote)
		|| (!(ft_isalpha(l[ex->j]) || l[ex->j] == '_')
			&& (l[ex->j] != '"' && l[ex->j] != '\''))
	)
	{
		ex->var = ft_strdup("$");
		ex->new_str = ft_strjoin(ex->str, ex->var);
		free_wrap(ex->str);
		ex->str = ex->new_str;
		free_wrap(ex->var);
		ex->var = NULL;
	}
	else
	{
		ex->var = ft_strdup("");
		ex->new_str = ft_strjoin(ex->str, ex->var);
		free_wrap(ex->str);
		ex->str = ex->new_str;
		free_wrap(ex->var);
		ex->var = NULL;
	}
}

/**
 * @brief Handles the if condition to decide what to do when finding $
 * @param ex t_expand struct
 * @param line Input string. 
 * @param sh t_shell Struct
 * @param p t_parse2, Parsing structure
 * @return void
*/
static void	handle_if_norm(t_expand *ex, char *l, t_shell *sh, t_parse2 *p)
{
	if (l[ex->j] && l[ex->j] == '?')
		handle_specials(ex, sh, '?', l);
	else if (l[ex->j] && l[ex->j] == '#')
		handle_specials(ex, sh, '#', l);
	else if (l[ex->j] && l[ex->j] == '$')
		handle_specials(ex, sh, '$', l);
	else if (l[ex->j] && (ft_isalpha(l[ex->j]) || l[ex->j] == '_'))
		handle_valid_case(ex, sh, l);
	else
		handle_else(ex, l, p);
}

/**
 * @brief Handles env variables / $ delimiter 
 * @param line Input string. 
 * @param sh t_shell Struct with env and error info
 * @param p t_parse2, Parsing structure (delimiters, len, curr_pos, diff used)
 * @return 1 on success, 0 on NULL
*/
char	*handle_env_var_inline(char *l, t_shell *sh, t_parse2 *p)
{
	t_expand	ex;

	init_expand_struct(&ex);
	if (p->prev_heredoc)
	{
		ex.j = 0;
		while (l[ex.j] && !ft_strchr(" |<>&\"'()\t\n\v\f\r", l[ex.j]))
			ex.j++;
		p->len += ex.j;
		return (ft_substr(l, 0, ex.j));
	}
	while (l[ex.j] && !ft_strchr(" |<>&\"'()\t\n\v\f\r", l[ex.j]))
	{
		if (l[ex.j] == '$')
		{
			ex.j++;
			handle_if_norm(&ex, l, sh, p);
		}
		else
			handle_specials(&ex, sh, l[ex.j], l);
	}
	p->len += ex.j;
	return (ex.str);
}
/* 
 //Casos no necesarios tratados como literales
 //-flags, no usadas en minish
 //$$ pid, no permite getpid()
    else if (next == '-')
    {
         ex->var = ft_strdup("himBHs");
    }
    else if (del == '$')
	{
		ex->var = ft_itoa(getpid());
	}
//  echo -asas$USER$?$%$$-$
//  echo -asas$USER$?$%$$-$&asasas&&ls
//  echo as--as$USER$?$%$$-$&asasas&&ls
*/
