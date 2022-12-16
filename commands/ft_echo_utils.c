/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 18:34:43 by marvin            #+#    #+#             */
/*   Updated: 2022/12/15 18:34:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_vars2(char **input)
{
	t_env_lst	*temp;
	char 		*name;

	name = input[1];
	temp = com_info()->env_lst;
	name++;
	while (temp)
	{
		if (!ft_strncmp(ft_strjoin(name, "="), temp->name, ft_strlen(name) - 1))
		{
			printf("%s\n", temp->value);
			return ;
		}
		temp = temp->next;
	}
}

void	print_vars(char **input)
{
	t_env_lst	*temp;
	char 		*name;

	name = input[1];
	temp = com_info()->vars;
	name++;
	while (temp)
	{
		if (!ft_strncmp(ft_strjoin(name, "="),temp->name, ft_strlen(name) - 1))
		{
			printf("%s\n", temp->value);
			return ;
		}
		temp = temp->next;
	}
	print_vars2(input);
}
