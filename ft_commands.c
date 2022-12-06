/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_commands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 19:08:28 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/29 19:08:28 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_pwd(void)
{
	printf("%s\n", print_dir());
	com_info()->exit_value = 0;
}

void	ft_env(char **input)
{
	t_env_lst	*temp;

	if (!find_path("env", com_info()->env_lst))
	{
		printf("\033[0;31mCommand not found: env\033[0m\n");
		com_info()->exit_value = 127;
		return ;
	}
	if (input[1])
	{
		printf("\033[0;31mEnv: '%s': No such file or directory\033[0m\n", input[1]);
		com_info()->exit_value = 127;
		return ;
	}
	temp = com_info()->env_lst;
	while (temp)
	{
		printf("%s%s\n", temp->name, temp->value);
		temp = temp->next;
	}
	com_info()->exit_value = 0;
}

void	ft_unset(char **input)
{
	int			i;
	t_env_lst	*head;

	if (!input[1])
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	i = 1;
	head = com_info()->env_lst;
	while (input[i])
	{
		while (com_info()->env_lst)
		{
			if (!ft_strncmp(input[i], com_info()->env_lst->name,
				(ft_strlen(com_info()->env_lst->name) - 1)))
			{
				printf("INPUT: %s\n", input[i]);
				printf("NAME: %s\n", com_info()->env_lst->name);
				printf("VALUE: %s\n", com_info()->env_lst->value);
				com_info()->env_lst->name = NULL;
				com_info()->env_lst->value = NULL;
				free(com_info()->env_lst->name);
				free(com_info()->env_lst->value);
				com_info()->env_lst->prev->next = com_info()->env_lst->next;
			}
			com_info()->env_lst = com_info()->env_lst->next;
		}
		com_info()->env_lst = head;
		i++;
	}
	com_info()->exit_value = 0;
	com_info()->env_lst = head;
}

void   *export(char **input)
{
    int			i;

    i = 1;
	if (!input[i])
		ft_env(input);
    while (input[i])
    {
		if(ft_strchr(input[i], '='))
			lst_add_back(&com_info()->env_lst, new_node(input[i]));
		else
			break;
		i++;
    }
	return (0);
}

//(export aaa bbb=) (cria aaa= e bbb=)