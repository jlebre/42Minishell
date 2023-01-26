/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:41:42 by nvideira          #+#    #+#             */
/*   Updated: 2023/01/26 02:01:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	do_redir(char **input)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		//	Como por o i e o j?
		//	redirections(new[i], i, j, com_info()->redir_type);
		commands(input, com_info()->env, 1);
	}
	else
		waitpid(pid, &com_info()->exit_value, 0);
	// E preciso dar close?
	//fd_close(com_info()->cmds_done_redir);
	unlink(".heredoc");
}

void	execute_redir(char **input)
{
	char	***new;
	int i;

	i = 0;
	new = split_redir(input);
	print_matrix_redir(new);
	while (new[i])
	{
		do_redir(new[i]);
		i++;
	}
}
