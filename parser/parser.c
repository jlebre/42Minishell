/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 14:18:36 by marvin            #+#    #+#             */
/*   Updated: 2023/02/11 14:18:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    parser(char *input, char **env)
{
	char	**args;
	int		pid;

	if (parser_checks(input))
		return ;
	input = parse_input(input);
	parser_checks2(input);
	if (check_special(input, '|'))
	{
		args = ft_split(input, '|');
		pipe_commands(args, env);
		free_matrix(args);
		return ;
	}
	args = ft_split(input, ' ');
	args = parse_cenas(args);
	if (find_es(args[0]))
		exported_vars(args);
	else if (check_special(args[0], '>') || check_special(args[0], '<'))
	{
		pid = fork();
		if (pid == 0)
			redirections(args[0], env);
		waitpid(pid, &com_info()->exit_value, 0);
	}
	else
		default_commands(args, env, 0);
	free_matrix(args);
}
