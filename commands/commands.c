/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:02:49 by jlebre            #+#    #+#             */
/*   Updated: 2023/01/12 00:44:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	commands(char **input, char **env)
{
	if (input[0])
	{
		if (!ft_strncmp(input[0], "echo", 5))
			ft_echo(input);
		else if (!ft_strncmp(input[0], "cd", 3))
			ft_cd(input, env);
		else if (!ft_strncmp(input[0], "pwd", 4))
			ft_pwd();
		else if (!ft_strncmp(input[0], "export", 7))
			ft_export(input);
		else if (!ft_strncmp(input[0], "unset", 6))
			ft_unset(input);
		else if (!ft_strncmp(input[0], "env", 4))
			ft_env(input);
		else if (!ft_strncmp(input[0], "exit", 5))
			ft_exit(input);
		else if (!ft_strncmp(input[0], "change_color", 13))
			change_color(input);
		else
			env_commands(input, env);
	}
}

void	fork_commands(char **input, char **env)
{
	printf("test\n");
	com_info()->pid = fork();
	if (com_info()->pid == 0)
	{
		if (com_info()->pipe_no != 0)
		{
			printf("dup\n");	
			fd_dup(com_info()->cmds_done);
		}
		printf("Go to commands\n");
		commands(input, env);
	}
	else
		catch_signal();
	return ;
}

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