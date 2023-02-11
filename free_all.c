/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 15:35:18 by jlebre            #+#    #+#             */
/*   Updated: 2023/02/11 17:55:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(void)
{
	free(com_info()->color);
}

void	free_list(t_env_lst *lst)
{
	while (lst)
	{
		free(lst->name);
		free(lst->value);
		lst = lst->next;
	}
	free(lst);
}
