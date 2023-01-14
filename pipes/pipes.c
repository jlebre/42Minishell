/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 02:22:13 by nvideira          #+#    #+#             */
/*   Updated: 2023/01/14 00:23:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(void)
{
	int	i;

	i = 0;
	com_info()->pip = malloc(sizeof(int *) * (com_info()->pipe_no + 1));
	com_info()->cmds_done = 0;
	while (i < com_info()->pipe_no)
	{
		com_info()->pip[i] = malloc(sizeof(int) * 2);
		if (pipe(com_info()->pip[i]) < 0)
		{
			ft_error("Pipe error\n");
			return ;
		}
		i++;
	}
	com_info()->pip[i] = NULL;
	//do_pipes();
	//free_pipes();
}

void	fd_dup(int pos)
{
	if (pos == 0)
	{
		close(com_info()->pip[0][0]);
		dup2(com_info()->pip[0][1], 1);
		close(com_info()->pip[0][1]);
	}
	else if (pos == com_info()->pipe_no)
	{
		dup2(com_info()->pip[pos - 1][0], 0);
		close(com_info()->pip[pos - 1][0]);
	}
	else
	{
		close(com_info()->pip[pos][0]);
		dup2(com_info()->pip[pos - 1][0], STDIN_FILENO);
		close(com_info()->pip[pos - 1][0]);
		dup2(com_info()->pip[pos][1], STDOUT_FILENO);
		close(com_info()->pip[pos][1]);
	}
}

void	fork_commands(char **input, char **env)
{
	com_info()->pid = fork();
	//printf("PID: %i\n", com_info()->pid);
	if (com_info()->pid == 0)
	{
		//printf("child\n");
		if (com_info()->pipe_no != 0)
		{
			if (com_info()->cmds_done == 0)
			{
				close(com_info()->pip[0][0]);
				dup2(com_info()->pip[0][1], 1);
			}
			else if (com_info()->cmds_done == com_info()->pipe_no)
			{
				close(com_info()->pip[com_info()->cmds_done - 1][1]);
				dup2(com_info()->pip[com_info()->cmds_done - 1][0], 0);
			}
		}
		//printf("Go to commands child %s\n", input[0]);
		commands(input, env);
	}
	else
	{
		waitpid(com_info()->pid, &com_info()->status, 0);
		com_info()->exit_value = com_info()->status / 256;	
	}
	return ;
}

/*
void	fork_commands(char **input, char **env)
{
	com_info()->pid = fork();
	if (com_info()->pid == 0)
	{
		if (com_info()->pipe_no != 0)
			fd_dup(com_info()->cmds_done);
		printf("Go to commands %s\n", input[0]);
		commands(input, env);
	}
	else
		catch_signal();
	return ;
}
*/

void	wait_pid(int counter)
{
	int	pid_counter;

	pid_counter = 0;
	while (pid_counter < counter)
	{
		waitpid(com_info()->pid, &com_info()->status, 0);
		com_info()->exit_value = com_info()->status / 256;
		pid_counter++;
	}
}

int	needs_fork(char	**input)
{
	if (com_info()->pipe_no > 0)
		return (1);
	else if (!ft_strncmp(input[0], "cd", 3) || !ft_strncmp(input[0], "export", 7)
		|| !ft_strncmp(input[0], "unset", 6) || !ft_strncmp(input[0], "exit", 5)
		|| !ft_strncmp(input[0], "echo", 5) || !ft_strncmp(input[0], "pwd", 4)
		|| !ft_strncmp(input[0], "env", 4) || !ft_strncmp(input[0], "change_color", 13))
		return (0);
	else
		return (1);
}