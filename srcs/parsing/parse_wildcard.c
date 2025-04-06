/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:42:43 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 18:57:14 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	put_files(char **files, const char *new_file)
{
	char	*tmp;
	char	*tmp2;

	if (*files == NULL)
		*files = ft_strdup(new_file);
	else
	{
		tmp = ft_strjoin("\n", new_file);
		if (tmp == NULL)
			return ; //error
		tmp2 = ft_strjoin(*files, tmp);
		if (tmp2 == NULL)
			return ; //error
		free_wrap(*files);
		free_wrap(tmp);
		*files = ft_strdup(tmp2);
		free_wrap(tmp2);
	}
}

int	is_matching(char *match_str, const char *file_name)
{
	while (*match_str!= '*' && file_name != NULL)
	{
		if (*file_name != *match_str)
			return (1);
		file_name++;
		match_str++;
	}
	return (0);
}

char	*parse_wildcard(char *wildcard_str, bool is_first_wcard)
{
	DIR		*curr_dir;
	char		*files;
	char		*file;
	struct dirent	*dir;

	files = NULL;
	curr_dir = opendir(".");
	if (curr_dir == NULL)
		return (NULL); //error
	if (*wildcard_str == '*')
		ft_strrev(wildcard_str);
	while (1)
	{
		dir = readdir(curr_dir);
		if (dir == NULL)
			break ;
		file = ft_strdup(dir->d_name);
		if (is_first_wcard == true)
			ft_strrev(file);
		if (is_matching(wildcard_str, file) == 0)
			put_files(&files, dir->d_name);
		free_wrap(file);
	}
	closedir(curr_dir);
	return (files);
}
