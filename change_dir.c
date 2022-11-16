/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:56:17 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/16 15:56:36 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_dir(char **input)
{
	if (input[1])
		chdir(input[1]);
	else
		chdir(ft_strjoin("/nfs/homes/", getenv("USER")));
}
