/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_n_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:24:21 by sluterea          #+#    #+#             */
/*   Updated: 2025/05/04 14:24:22 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	cnt_words(char const *s)
{
	size_t	i;
	size_t	count;

	if (s == NULL)
		return (0);
	count = 0;
	i = 0;
	if (s[0] && s[0] != '\n')
		count = 1;
	while (s[i])
	{
		if (s[i] == '\n')
		{
			count++;
		}
		i++;
	}
	return (count);
}

static int	cnt_letters(char const *s, size_t i)
{
	size_t	j;

	j = 0;
	while (s[i] != '\n' && s[i] != '\0')
	{
		j++;
		i++;
	}
	return (j);
}

int	init_sp(t_split_p *sp, const char *s)
{
	sp->count = cnt_words(s);
	sp->arr = malloc((sp->count + 1) * sizeof(char *));
	if (!sp->arr)
		return (-1);
	sp->index = 0;
	sp->i = 0;
	return (1);
}

int	inside_while_sp(t_split_p *sp, const char *s)
{
	sp->len = cnt_letters(s, sp->index);
	sp->arr[sp->i] = malloc(sp->len + 1);
	if (!sp->arr[sp->i])
	{
		free_str(sp->arr, sp->i);
		return (-1);
	}
	sp->j = 0;
	while (s[sp->index] && s[sp->index] != '\n')
	{
		sp->arr[sp->i][sp->j] = s[sp->index];
		sp->j++;
		sp->index++;
	}
	sp->arr[sp->i][sp->j] = '\0';
	sp->i++;
	if (s[sp->index] == '\n')
		sp->index++;
	return (1);
}

char	**ft_split_parse(char const *s)
{
	t_split_p	sp;

	if (!s || init_sp(&sp, s) == -1)
		return (NULL);
	if (*s == '\0' || (ft_strlen(s) > 0 && sp.count == 0))
	{
		sp.arr[0] = NULL;
		return (sp.arr);
	}
	while (s[sp.index] || sp.i < sp.count)
	{
		if (inside_while_sp(&sp, s) == -1)
			return (NULL);
		if (!s[sp.index] && sp.i < sp.count)
		{
			sp.arr[sp.i] = malloc(1);
			if (!sp.arr[sp.i])
				return (free_str(sp.arr, sp.i), NULL);
			sp.arr[sp.i++][0] = '\0';
		}
		if (sp.i >= sp.count)
			break ;
	}
	sp.arr[sp.i] = NULL;
	return (sp.arr);
}
