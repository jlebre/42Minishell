/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 23:31:01 by marvin            #+#    #+#             */
/*   Updated: 2022/11/17 23:31:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Se não tiveres " " e tiveres mais de 1 argumento tem de imprimir todos

void	print_vars(char **input)
{
	t_env_lst	*temp;
	char 		*name;

	name = input[1];
	if (ft_strlen(name) < 2)
		printf("Error: no such file \"%s\"\n", com_info()->vars->name);
	temp = com_info()->vars;
	name++;
	while (temp)
	{
		if (!ft_strncmp(name, com_info()->vars->name, ft_strlen(name) - 1))
		{
			printf("%s\n", com_info()->vars->value);
			return ;
		}
		temp = temp->next;
	}
	printf("Error: no such file \"%s\"\n", com_info()->vars->name);
}

void	ft_echo(char **input)
{
	int	i;

	if (!ft_strncmp(input[1], "$?", 3))
		printf("%d\n", com_info()->exit_value);
	else if (input[1][0] == '$')
		print_vars(input);
	else if (!ft_strncmp(input[1], "-n", 3))
	{
		printf("%s", input[2]);
		com_info()->exit_value = 0;
	}
	else
	{
		i = 1;
		while (i < com_info()->nb_args)
		{
			printf("%s", input[i]);
			if ((com_info()->nb_args - i) != 1)
				printf(" ");
			i++;
		}
		printf("\n");
		com_info()->exit_value = 0;
	}
}
