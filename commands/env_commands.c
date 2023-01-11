/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:55:34 by jlebre            #+#    #+#             */
/*   Updated: 2023/01/11 18:23:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
void	env_commands(char **input, char **env)
{
	int	a;

	cmd = input[0];
	a = fork();
	if (!a)
		exec_command(cmd, path);
	waitpid(com_info()->pid, &com_info()->status, 0);
	com_info()->exit_value = com_info()->status / 256;
	free(cmd);
}
*/

void	env_commands(char **input, char **env)
{
	char 		*path;

	path = find_path(input[0], com_info()->env_lst);
	printf("PATH: |%s|\n", path);
	if (!path)
	{
		ft_error("\033[0;31mCommand not found: %s\033[0m\n", input[0]);
		com_info()->exit_value = 127;
		return ;
	}
	com_info()->pid = fork();
	if (com_info()->pid == 0)
	{
		if (com_info()->pipe_no != 0)
			fd_dup(com_info()->cmds_done);
		if (execve(path, input, env) == -1)
		{
			com_info()->exit_value = 126;
			ft_error("Deu Merda\n");
		}
	}
	waitpid(com_info()->pid, &com_info()->status, 0);
	com_info()->exit_value = com_info()->status / 256;
}

// Não funciona com absoluth path
// Acrescentar condição para verificar se está no env!
// Norminette
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
		j++;
	}
	if (temp == NULL)
		return (0);
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
