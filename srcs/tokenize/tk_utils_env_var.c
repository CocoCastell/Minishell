/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_utils_env_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:14:29 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/10 15:14:31 by sluterea         ###   ########.fr       */
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
 * @brief Handles adding $ delimiter.
 * @param ex t_expand Struct
 * @return void (expands string value)
*/
void	handle_else(t_expand *ex)
{
	ex->var = ft_strdup("$");
	ex->new_str = ft_strjoin(ex->str, ex->var);
	free_wrap(ex->str);
	ex->str = ex->new_str;
	free_wrap(ex->var);
	ex->var = NULL;
}

/**
 * @brief Initialize t_expand structure values
 * @param ex t_expand Struct
 * @return void
*/
void	init_expand_struct(t_expand *ex)
{
	ex->var = NULL;
	ex->str = ft_strdup("");
	ex->new_str = NULL;
	ex->j = 0;
	ex->count = 0;
	ex->env_var = NULL;
}

/**
 * @brief Handles retrieving of env variables value and shell specials.
 *        Supports `$?`, `$#`, and valid variable names.
 *        Other cases such as `$$`, `$-` are treated as literal. 
 * 		  DOES NOT FREE original string!
 * @param line original string to be expanded. 
 * @param sh t_shell Struct (env and error being used)
 * @return expanded new string value
*/
char	*handle_env_var_expand(char *l, t_shell *sh)
{
	t_expand	ex;

	init_expand_struct(&ex);
	while (l[ex.j] != '\0')
	{
		if (l[ex.j] == '$')
		{
			ex.j++;
			if (l[ex.j] && l[ex.j] == '?')
				handle_specials(&ex, sh, '?', l);
			else if (l[ex.j] && l[ex.j] == '#')
				handle_specials(&ex, sh, '#', l);
			else if (l[ex.j] && (ft_isalpha(l[ex.j]) || l[ex.j] == '_'))
				handle_valid_case(&ex, sh, l);
			else
				handle_else(&ex);
		}
		else
			handle_specials(&ex, sh, l[ex.j], l);
	}
	return (ex.str);
}
/* 
 //Casos no necesarios tratados como literales
 //-flags, no usadas en minish
 //$$ pid, no permite getpid()
      else if (next == '-')
     {
         str = ft_strdup("himBHs");
         p->len += 2;
         add_token(tk, str, UNKNOWN);
         return (1);
     }
     else if (next == '$')
     {
         str = ft_itoa(getpid());
         p->len += 2;
         add_token(tk, str, UNKNOWN);
         return (1);
     }


//  echo -asas$USER$?$%$$-$
//  echo -asas$USER$?$%$$-$&asasas&&ls
//  echo as--as$USER$?$%$$-$&asasas&&ls
*/
