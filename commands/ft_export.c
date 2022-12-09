/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 17:45:11 by jlebre            #+#    #+#             */
/*   Updated: 2022/12/09 20:14:20 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_exists(char *str)
{
	t_env_lst	*temp;
	char		*name;
	int			len;
	int			i;

	len = 0;
	i = 0;
	temp = com_info()->env_lst;
	while (str[len] != '=')
		len++;
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (0);
	while (i < len)
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	while (temp)
	{
		if (!ft_strncmp(name, temp->name, len))
		{
			free(name);
			return (1);
		}
		temp = temp->next;
	}
	free(name);
	return (0);
}

void	change_value(char *str)
{
	t_env_lst	*head;
	char		*name;
	char		*value;
	int			len;
	int			len_val;
	int			i;
	int			j;

	len = 0;
	i = 0;
	j = 0;
	head = com_info()->env_lst;
	while (str[len] != '=')
		len++;
	name = malloc(sizeof(char) * (len + 2));
	if (!name)
		return ;
	len_val = (ft_strlen(str) - (len + 1));
	value = malloc(sizeof(char) * (len_val + 1));
	if (!value)
		return ;
	while (i < len && str[i])
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '=';
	i++;
	name[i] = '\0';
	while (str[i])
	{
		value[j] = str[i];
		i++;
		j++;
	}
	while (com_info()->env_lst->name != name)
	{
		if (!ft_strncmp(name, com_info()->env_lst->name, (len + 1)))
		{
			com_info()->env_lst->value = value;
			break ;
		}
		com_info()->env_lst = com_info()->env_lst->next;
	}
	com_info()->env_lst = head;
}

void   *ft_export(char **input)
{
    int	i;

    i = 1;
	if (!input[i])
		ft_env(input);
    while (input[i])
    {
		if (ft_strchr(input[i], '='))
		{
			if (check_if_exists(input[i]))
				change_value(input[i]);
			else
				lst_add_back(&com_info()->env_lst, new_node(input[i]));
		}
		else
			break ;
		i++;
    }
	return (0);
}
