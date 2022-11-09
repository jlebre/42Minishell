/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_sound.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 18:44:55 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/09 17:28:29 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void  play_sound(char **env, char *sound)
{
	char	*arr[3];
	int		cenas;

	arr[0] = strdup("/bin/paplay");
	arr[1] =  strdup(sound);
	arr[2] = 0;
	cenas = fork();
	if (!cenas)
	{	
		if (execve(arr[0], arr, env) == -1)
		{
			play_sound(env, "error.ogg");
			printf("ERROR!\n");
		}
	}
}
