/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2022/12/14 17:04:55 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Se já existir na lista ENV, tem de alterar o valor lá

void	process_input(char *input, char **env)
{
	char	**args;
	
	if (input[0] == '\0')
		return ;
	add_history(input);
	args = ft_split(input, 32);
	com_info()->nb_args = count_args(args);
	if (find_es(args[0]) == 1)
	{
		exported_vars(args);
	}
	else
		commands(args, env);
}
