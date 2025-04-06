/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:54:42 by sluterea          #+#    #+#             */
/*   Updated: 2025/03/27 12:54:43 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_tokens(t_token **result)
{
	t_token	*current;

	current = *result;
	printf("Tokens: ");
	while (current != NULL)
	{
		printf(WHITE" . %s(type: %d)", current->str, current->type);
		current = current->next;
	}
	printf("\n");
}
