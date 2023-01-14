/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:55:34 by jlebre            #+#    #+#             */
/*   Updated: 2023/01/14 00:00:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_commands(char **input, char **env)
{
	char 		*path;

	printf("INPUT: |%s|\n", input[0]);
	path = find_path(input[0], com_info()->env_lst);
	if (!path)
	{
		ft_error("\033[0;31mCommand not found: %s\033[0m\n", input[0]);
		com_info()->exit_value = 127;
		return ;
	}
	if (execve(path, input, env) == -1)
	{
		com_info()->exit_value = 126;
		ft_error("Deu Merda\n");
	}
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
