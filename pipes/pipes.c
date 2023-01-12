/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 02:22:13 by nvideira          #+#    #+#             */
/*   Updated: 2023/01/11 19:21:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(void)
{
	int	i;

	i = -1;
	com_info()->pip = malloc(sizeof(int *) * (com_info()->pipe_no + 1));
	com_info()->cmds_done = 0;
	while (++i < com_info()->pipe_no)
	{
		com_info()->pip[i] = malloc(sizeof(int) * 2);
		if (pipe(com_info()->pip[i]) < 0)
		{
			write(2, "Pipe error\n", 10);
			return ;
		}
	}
	com_info()->pip[i] = NULL;
}

void	fd_dup(int pos)
{
	pipe(com_info()->pip[pos]);
	if (pos == 0)
	{
		close(com_info()->pip[0][0]);
		dup2(com_info()->pip[0][1], STDOUT_FILENO);
		close(com_info()->pip[0][1]);
	}
	else if (pos == com_info()->pipe_no)
	{
		dup2(com_info()->pip[pos - 1][0], STDIN_FILENO);
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
