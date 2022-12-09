/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:56:17 by jlebre            #+#    #+#             */
/*   Updated: 2022/12/07 17:58:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_dir(char **input)
{
	if (input[1])
	{
		if (!ft_strncmp(input[1], "-", 2))
		{
			chdir(getenv("OLDPWD"));
			com_info()->exit_value = 0;
		}
		else
		{
			chdir(input[1]);
			com_info()->exit_value = 0;
		}
	}
	else
	{
		chdir(ft_strjoin("/home/", getenv("USER")));
		com_info()->exit_value = 0;
	}
}

//Working with "cd -"
//		export(ft_strjoin("OLDPWD=", getenv("PWD")));