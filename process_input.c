/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/10 15:02:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_input(char *input, char **env, t_env_lst *env_lst)
{
	char	**args;
	(void)env_lst;
	//int		i;

	//i = 0;
	//printf("input: /%s/\n", input);
	if(input[0] == '\0')
		return ;
	add_history(input);
	args = ft_split(input, 32);
	commands(args, env);
}

/*
void	process_input(char *input, char **env)
{
	char	**args;
	
	if (input && input[0] != '\0')
		add_history(input);
	args = ft_split(input, 32);
	commands(args, env);
}

*/