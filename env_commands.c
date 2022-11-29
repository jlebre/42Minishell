/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:55:34 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/29 17:30:10 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_commands(char **input, char **env)
{
	char	*arr[2];
	int		i;
	int		cenas;
	int		temp;

	i = 0;
	arr[0] = find_path(input[0], com_info()->env_lst);
	printf("%s\n", arr[0]);
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
		while (com_info()->env_lst->next)
		{
			while (env[i])
			{
				if (ft_strcmp(com_info()->env_lst->value, env[i]))
				{
					if (execve(arr[0], arr, env) == -1)
					{
						com_info()->exit_value = 126;
						ft_error("Deu Merda");
					}
					else
					{
						printf("Ola\n");	
					}
				}
				i++;
			}
			i = 0;
			com_info()->env_lst = com_info()->env_lst->next;
		}
		
	}
	waitpid(cenas, &temp, 0);
	com_info()->exit_value = temp / 256;
}

char	*find_path(char *cmd, t_env_lst *env_lst)
{
	int			j;
	t_env_lst	*temp;
	char		*path;
	char		*ret_path;

	j = 0;
	temp = env_lst;
	while (temp && ft_strncmp(temp->name, "PATH=", 5))
	{
		temp = temp->next;
	}
	path = temp->value;
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
