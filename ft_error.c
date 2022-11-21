/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:15:16 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/21 17:51:44 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(char *err, t_env_lst *env_lst)
{
	(void)env_lst;
	red("Error!\n");
	red(err);
	//play_sound(env, "error.ogg");
}
