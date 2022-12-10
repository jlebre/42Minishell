/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2022/12/10 20:22:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void   exported_vars(char **input)
{
	int	i;

	i = 0;
	if (input[1][i] == '=')
	{
		printf("%s not found\n", input[1]);
		return ;
	}
	while (input[i])
	{
		if (ft_strchr(input[i], '='))
		{
			if (check_if_exists(input[i]))
				change_value(input[i]);
			else
				lst_add_back(&com_info()->vars, new_node(input[i]));
		}
		else
			break ;
		i++;
	}
	return ;
}

//Find Equal Sign
int	find_es(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

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

int	count_args(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		i++;
	return (i);
}
