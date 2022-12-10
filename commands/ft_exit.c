/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 19:08:28 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/29 19:08:28 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(char **input)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (input[i])
		i++;
	if (i > 2)
	{
		if (!ft_isdigit(input[1]))
		{
			printf("exit\nexit: %s: numeric argument required\n", input[1]);
			rl_clear_history();
			free (input);
			exit(2);
		}
		else
		{
			printf("exit\nexit: too many arguments\n");
			com_info()->exit_value = 1;
			return ;
		}
	}
	else if (i == 2)
	{
		if (!ft_isdigit(input[1]))
		{
			printf("exit\nexit: %s: numeric argument required\n", input[1]);
			rl_clear_history();
			free (input);
			exit(2);
		}
		else
		{
			printf("exit\n");
			rl_clear_history();
			nb = ft_atoi(input[1]);
			free (input);
			exit(nb);
		}
	}
	else
	{
		printf("exit\n");
		rl_clear_history();
		free (input);
		exit(com_info()->exit_value);
	}	
}
