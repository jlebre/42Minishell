/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvideira <nvideira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 18:30:37 by marvin            #+#    #+#             */
/*   Updated: 2023/02/06 18:31:25 by nvideira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função que imprime as variáveis de ambiente
char	*print_vars_exit(char **input)
{
	t_env_lst	*temp;
	char		*name;

	name = input[1];
	temp = com_info()->vars;
	name++;
	while (temp)
	{
		if (!ft_strncmp(name, com_info()->vars->name, ft_strlen(name) - 1))
			return (com_info()->vars->value);
		temp = temp->next;
	}
	return (0);
}

// Verifica se o exit tem erros
void	exit_errors(int error, char **input)
{
	if (error == 1)
	{
		ft_error("exit\nexit: %s: numeric argument required\n", input[1]);
		rl_clear_history();
		free (input);
		exit(2);
	}
	else if (error == 2)
	{
		ft_error("exit\nexit: too many arguments\n");
		com_info()->exit_value = 1;
		return ;
	}
	else if (error == 3)
	{
		ft_error("exit\nexit: %s: numeric argument required\n", input[1]);
		rl_clear_history();
		free (input);
		exit(2);
	}
}

// Verifica se o exit tem erros
void	check_error_3(char *arg, char **input)
{
	if (!ft_isdigit(arg))
		exit_errors(3, input);
	else
		do_exit(ft_atoi(arg), input);
}
