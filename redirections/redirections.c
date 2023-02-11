/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:29:35 by marvin            #+#    #+#             */
/*   Updated: 2023/02/11 18:24:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Faz as redireções e as duplicações de file descriptors
// O que se faz quando tem vários argumentos a seguir ao redirecionador?
void	redirections(char *input, char **env)
{
	int	fd_in;
	int	fd_out;

	if (check_special(input, '<'))
	{
		fd_in = redirect_input(input);
		if (fd_in <= -1)
			exit(com_info()->exit_value);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (check_special(input, '>'))
	{
		fd_out = redirect_output(input);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	commands(input, env, 1);
	exit(com_info()->exit_value);
}
