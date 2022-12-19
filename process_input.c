/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2022/12/19 11:50:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_input(char **env)
{
	while (com_info()->commands)
	{
		if (com_info()->pipe_no > 0)
		{
			if (com_info()->pipe_no % 2 == 1)
			{
				if (pipe(com_info()->pipe1) < 0)
				{
					write(1, "minishell: pipe error\n", 22);
					return ;
				}
			}
			else
			{	
				if (pipe(com_info()->pipe2) < 0)
				{
					write(1, "minishell: pipe error\n", 22);
					return ;
				}
			}
			com_info()->pipe_no--;
		}
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else
			commands(com_info()->commands->arg, env);
		com_info()->commands = com_info()->commands->next;
	}
}

/* void	process_input(char **env)
{
	while (com_info()->commands)
	{
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else
			commands(com_info()->commands->arg, env);
		com_info()->commands = com_info()->commands->next;
	}
}

void	process_input(char **env)
{
	while (com_info()->commands)
	{
		if (com_info()->pipe_no % 2 == 1)
			if (use_pipe(com_info()->pipe1))
				return ;
		else
			if (use_pipe(com_info()->pipe2))
				return ;
		com_info()->pipe_no--;
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else
			commands(com_info()->commands->arg, env);
		com_info()->commands = com_info()->commands->next;
	}
}
 */
