/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_sound.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 18:44:55 by jlebre            #+#    #+#             */
/*   Updated: 2023/02/09 16:56:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	play_sound(char **env, char *sound)
{
	char	*arr[3];
	int		cenas;

	arr[0] = strdup("/bin/paplay");
	arr[1] = strdup(sound);
	arr[2] = 0;
	cenas = fork();
	if (!cenas)
	{	
		if (execve(arr[0], arr, env) == -1)
		{
			play_sound(env, "error.ogg");
			ft_error("ERROR!\n");
		}
	}
	free(arr[0]);
	free(arr[1]);
}

//ADD TO INIT.SHELL.c
//play_sound(env, "init.ogg");

//ADD TO MINISHELL.H
/*
//SOUND
void	play_sound(char **env, char *sound);
*/
