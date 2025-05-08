/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:42:43 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/14 16:14:08 by cochatel         ###   ########.fr       */
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
			return ;
		tmp2 = ft_strjoin(*files, tmp);
		free_wrap(*files);
		free_wrap(tmp);
		if (tmp2 == NULL)
			return ;
		*files = ft_strdup(tmp2);
		free_wrap(tmp2);
	}
}

int	is_matching(char *match_str, const char *file_name)
{
	while (*match_str != '*' && file_name != NULL)
	{
		if (*file_name != *match_str)
			return (1);
		file_name++;
		match_str++;
	}
	return (0);
}

void	check_if_no_matching(char *wildcard_str, char **files)
{
	ft_strrev(wildcard_str);
	if (*files == NULL)
		*files = ft_strdup(wildcard_str);
}

char	*parse_wildcard(char *w_str, bool is_first_wcard)
{
	printf("Check\n");
	DIR				*curr_dir;
	char			*files;
	char			*file;
	struct dirent	*dir;

	printf("str: %s\n", w_str);
	files = NULL;
	curr_dir = opendir(".");
	if (curr_dir == NULL)
		return (NULL);
	if (*w_str == '*')
		ft_strrev(w_str);
	while (1)
	{
		dir = readdir(curr_dir);
		if (dir == NULL)
			break ;
		file = ft_strdup(dir->d_name);
		if (is_first_wcard == true)
			ft_strrev(file);
		if (is_matching(w_str, file) == 0)
			put_files(&files, dir->d_name);
		free_wrap(file);
	}
	return (closedir(curr_dir), check_if_no_matching(w_str, &files), files);
}
