/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2023/01/13 23:47:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Doesn't work
// a=3
// b=$a
// echo $b
// $a
// but works with exported

void	process_input(char **env)
{
	while (com_info()->commands)
	{
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else if (needs_fork(com_info()->commands->arg))
			fork_commands(com_info()->commands->arg, env);
		else
			commands(com_info()->commands->arg, env);
		com_info()->cmds_done++;
		com_info()->commands = com_info()->commands->next;
	}
	//catch_signal();
}

/* int i = 0;
	while (com_info()->commands)
	{
		i = 0;
		while (com_info()->commands->arg[i])
		{
			printf("INPUT[%i]: %s\n", i, com_info()->commands->arg[i]);
			i++;
		}
		com_info()->commands = com_info()->commands->next;
	}
	printf("printed everything\n"); */