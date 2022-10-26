/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2022/10/26 19:52:52 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_input(char *input, char **env)
{
	char	**args;
	int		i;

	i = 0;
	add_history(input);
	args = ft_split(input, 32);
	while (args[i])
	{
		printf("%s\n", args[i]);
		i++;
	} 
	commands(args, env);
}

	/* printf("INPUT: %s\n", input);
	*/
	//print_header(env);