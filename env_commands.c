/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:55:34 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/18 01:30:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_commands(char **input, char **env)
{
	char	*arr[2];
	int		cenas;

	arr[0] = find_path(input[0], env);
	if (!arr[0])
	{
		printf("\033[0;31mCommand not found: %s\033[0m\n", input[0]);
		com_info()->exit_value = 127;
		return ;
	}
	arr[1] = input[1];
	arr[2] = 0;
	cenas = fork();
	if (!cenas)
	{
		if (execve(arr[0], arr, env) == -1)
			ft_error("Failed", env);
	}
	waitpid(cenas, NULL, 0);
	com_info()->exit_value = 0;
}

char	*find_path(char *cmd, char **env)
{
	int		j;
	char	*path;
	char	*ret_path;

	j = 0;
	while (env[j] && ft_strncmp(env[j], "PATH=", 5))
		j++;
	path = env[j];
	while (path[j] && ft_strichr(path, j, ':') > -1)
	{
		ret_path = join_strings(path, j, cmd);
		if (!access(ret_path, F_OK))
			return (ret_path);
		free(ret_path);
		j += ft_strichr(path, j, ':') - j + 1;
	}
	if (path[j] && ft_strichr(path, j, ':') < 0)
	{
		ret_path = join_strings(path, j, cmd);
		if (!access(ret_path, F_OK))
			return (ret_path);
		free(ret_path);
	}
	return (0);
}
