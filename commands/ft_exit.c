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

#include "minishell.h"

// Verifica se o input é maior que um int64
int	check_size_int(char *str)
{
	char *max_int;
	int	i;

	i = 0;
	max_int = "9223372036854775807";
	while (str[i] && max_int[i])
	{
		if (str[i] > max_int[i])
			return (0);
		else if (str[i] < max_int[i])
			return (1);
		i++;
	}
	return (1);
}

// Recria a função exit
void	ft_exit(char **input)
{
	int		i;
	char	*value;

	i = 0;
	while (input[i])
		i++;
	if (i > 2)
	{
		if (!ft_isdigit(input[1]))
			exit_errors(1, input);
		else
			exit_errors(2, input);
	}
	else if (i == 2)
	{
		if (!check_size_int(input[1]))
			exit_errors(1, input);
		else if (input[1][0] == '$' && ft_strlen(input[1]) > 1)
		{
			value = print_vars_exit(input);
			check_error_3(value, input);
		}
		check_error_3(input[1], input);
	}
	else
		do_exit(com_info()->exit_value, input);
}

// Função que executa o exit
void	do_exit(int exit_value, char **input)
{
	printf("exit\n");
	rl_clear_history();
	free (input);
	exit(exit_value);
}
