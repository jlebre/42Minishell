/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/11 16:58:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_input(char *input, char **env)
{
	char	**args;
	
	if (input[0] == '\0')
		return ;
	add_history(input);
	args = ft_split(input, 32);
	commands(args, env);
}
