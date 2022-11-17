/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 15:59:31 by jlebre            #+#    #+#             */
/*   Updated: 2022/10/25 15:59:31 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*com_info(void)
{
	static t_command	a;

	return (&a);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_env_lst	*env_lst;

	(void)ac;
	(void)av;
	init_shell(env);
	env_lst = env_to_lst(env);
	while (env_lst)
	{
		printf("name: %s, value: %s\n", env_lst->name, env_lst->value);
		env_lst = env_lst->next;
	} 
	ft_clear();
	while (1)
	{
		input = readline(print_info());
		if (!input)
		{
			rl_clear_history();
			free (input);
			exit(com_info()->exit_value);
		}
		process_input(input, env);
	}
	free (input);
	return (com_info()->exit_value);
}
