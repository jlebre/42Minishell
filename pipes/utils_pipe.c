/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 17:58:53 by jlebre            #+#    #+#             */
/*   Updated: 2023/02/11 17:55:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Conta o numero de pipes
int	count_pipes(char *input)
{
	int	i;
	int	pipe_no;

	i = 0;
	pipe_no = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = skip_quotes(input, i, input[i]);
		else if (input[i] == '|')
			pipe_no++;
		i++;
	}
	return (pipe_no);
}

int	count_args(char **input)
{
	int count;

	count = 0;
	while (input[count])
		count++;
	return (count);
}

void	pipe_cleanup(void)
{
	int	i;

	i = 0;
	while (i < com_info()->pipe_no)
	{
		close(com_info()->pip[i][0]);
		close(com_info()->pip[i][1]);
		i++;
	}
	free(com_info()->pip);
}
