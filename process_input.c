/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2023/01/26 02:11:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Temos de fazer um check se há alguma coisa a seguir as redir

void	process_input(char **env)
{
	while (com_info()->commands)
	{
		printf("INPUT: %s\n", com_info()->commands->arg[0]);
		print_matrix(com_info()->commands->arg);
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else if (count_redirs(com_info()->commands->arg) > 0)	
			execute_redir(com_info()->commands->arg);
		else if (com_info()->pipe_no > 0)
			execute_pipe(com_info()->commands->arg);
		else
			commands(com_info()->commands->arg, env, 0);
		com_info()->cmds_done++;
		com_info()->commands = com_info()->commands->next;
	}
	//free_all();
	catch_signal();
}
