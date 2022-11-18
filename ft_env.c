/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 12:29:02 by marvin            #+#    #+#             */
/*   Updated: 2022/11/18 12:29:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **env)
{
	t_env_lst	*env_lst;
	
	env_lst = env_to_lst(env);
	while (env_lst)
	{
		printf("name: %s, value: %s\n", env_lst->name, env_lst->value);
		env_lst = env_lst->next;
	}
	com_info()->exit_value = 0;
}
