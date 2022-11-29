/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 17:18:35 by nvideira          #+#    #+#             */
/*   Updated: 2022/11/29 17:31:38 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env_lst **env)
{
	while ((*env)->next)
	{
		free((*env)->name);
		free((*env)->value);
		free(env);
		(*env)->name = NULL;
		(*env)->value = NULL;
		*env = (*env)->next;
	}
}