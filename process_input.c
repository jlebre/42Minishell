/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:05 by jlebre            #+#    #+#             */
/*   Updated: 2022/12/28 03:02:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		size_of_number(long nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		nb = nb * -1;
		len++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int number)
{
	char	*str;
	long	nb;
	int		len;
	
	nb = number;
	len = size_of_number(nb);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (nb == 0)
	{
		str[0] = 48;
		return (str);
	}
	if (nb < 0)
	{
		str[0] = '-';
		nb = nb * -1;
	}
	while (nb > 0)
	{
		str[len] = 48 + (nb % 10);
		nb = nb / 10;
		len--;
	}
	return (str);
}

char	*change_val2(char *input)
{
	int			i;
	int			j;
	static char	*name;
	
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			i++;
			while (input[i] != ' ')
			{
				name[j] = input[i];
				j++;
				i++;
			}
			//check name;
			//change;
			//join to previous str
		}
		i++;
	}
	
	return (input);
}

char	*change_val(char *input)
{
	t_env_lst	*temp;
	t_env_lst	*temp2;
	char 		*name;

	name = input;
	temp = com_info()->vars;
	name++;
	while (temp)
	{
		if (!ft_strncmp(ft_strjoin(name, "="), temp->name, ft_strlen(name) + 1))
			return (temp->value);
		temp = temp->next;
	}
	temp2 = com_info()->env_lst;
	while (temp2)
	{
		if (!ft_strncmp(ft_strjoin(name, "="), temp2->name, ft_strlen(name) + 1))
			return (temp2->value);
		temp2 = temp2->next;
	}
	return ("");
}

char	**check_ds(char **input)
{
	int	i;

	i = 1;
	if (!input[1])
		return (input);
	while (input[i])
	{
		if (!ft_strncmp(input[i], "$?", 3))
			input[i] = ft_itoa(com_info()->exit_value);
		else if (input[i][0] == '$' && ft_strlen(input[i]) > 1)
			input[i] = ft_strdup(change_val(input[i]));
		else if (ft_strchr(input[i], '$') && ft_strlen(input[i]) > 1)
			input[i] = ft_strdup(change_val2(input[i]));
		i++;
	}
	return (input);
}

void	process_input(char **env)
{	
	while (com_info()->commands)
	{
		if (com_info()->pipe_no > 0)
		{
			if (com_info()->pipe_no % 2 == 1)
			{
				if (pipe(com_info()->pipe1) < 0)
				{
					write(1, "minishell: pipe error\n", 22);
					return ;
				}
			}
			else
			{	
				if (pipe(com_info()->pipe2) < 0)
				{
					write(1, "minishell: pipe error\n", 22);
					return ;
				}
			}
			com_info()->pipe_no--;
		}
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else
		{
			com_info()->commands->arg = check_ds(com_info()->commands->arg);
			commands(com_info()->commands->arg, env);
		}
		com_info()->commands = com_info()->commands->next;
	}
}

/* void	process_input(char **env)
{
	while (com_info()->commands)
	{
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else
			commands(com_info()->commands->arg, env);
		com_info()->commands = com_info()->commands->next;
	}
}

void	process_input(char **env)
{
	while (com_info()->commands)
	{
		if (com_info()->pipe_no % 2 == 1)
			if (use_pipe(com_info()->pipe1))
				return ;
		else
			if (use_pipe(com_info()->pipe2))
				return ;
		com_info()->pipe_no--;
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else
			commands(com_info()->commands->arg, env);
		com_info()->commands = com_info()->commands->next;
	}
}
 */
