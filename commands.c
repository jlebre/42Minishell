/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:02:49 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/18 01:16:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	commands(char **input, char **env)
{
	if (input[0])
	{
		if (!ft_strncmp(input[0], "echo", 5)) //-n ALMOST WORKING; PENDENT ON " "
			ft_echo(input);
		else if (!ft_strncmp(input[0], "cd", 3)) //WORKING
			change_dir(input);
		else if (!ft_strncmp(input[0], "pwd", 4)) //WORKING
		{
			printf("%s\n", print_dir());
			com_info()->exit_value = 0;
		}
		else if (!ft_strncmp(input[0], "export", 7))
			printf("EXPORT: %s\n", input[1]);
		else if (!ft_strncmp(input[0], "unset", 6))
			printf("UNSET: %s\n", input[1]);
		else if (!ft_strncmp(input[0], "env", 4))
			printf("ENV: %s\n", input[0]);
		else if (!ft_strncmp(input[0], "exit", 5)) //WORKING
			{
			rl_clear_history();
			free (input);
			exit(com_info()->exit_value);
			}
		else
			env_commands(input, env); //WORKING
	}
}
