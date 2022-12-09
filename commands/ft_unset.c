/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 17:44:22 by jlebre            #+#    #+#             */
/*   Updated: 2022/12/09 17:49:28 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(char **input)
{
	int			i;
	t_env_lst	*head;

	if (!input[1])
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	i = 1;
	head = com_info()->env_lst;
	while (input[i])
	{
		while (com_info()->env_lst)
		{
			if (!ft_strncmp(input[i], com_info()->env_lst->name,
				(ft_strlen(com_info()->env_lst->name) - 1) - 1))
			{
				printf("%s\n", input[i]);
				com_info()->env_lst->name = NULL;
				com_info()->env_lst->value = NULL;
				free(com_info()->env_lst->name);
				free(com_info()->env_lst->value);
				com_info()->env_lst->prev->next = com_info()->env_lst->next;
			}
			com_info()->env_lst = com_info()->env_lst->next;
		}
		com_info()->env_lst = head;
		i++;
	}
	com_info()->exit_value = 0;
	com_info()->env_lst = head;
}
