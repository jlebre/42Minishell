/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 00:23:13 by marvin            #+#    #+#             */
/*   Updated: 2023/01/04 00:23:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*change_val(char *input)
{
	t_env_lst	*temp;
	t_env_lst	*temp2;
	char		*name;

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
		if (!ft_strncmp(ft_strjoin(name, "=")
				, temp2->name, ft_strlen(name) + 1))
			return (temp2->value);
		temp2 = temp2->next;
	}
	return ("");
}

// ' and " handling not working properly when there's more than 2

int	count_ds(char *str)
{
	int i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		i++;
	}
	return (count);
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
		else if (input[i][0] == '$' && ft_strlen(input[i]) > 1
			&& count_ds(input[i]) == 1)
			input[i] = ft_strdup(change_val(input[i]));
		else if (count_ds(input[i]) >= 1 && (ft_strlen(input[i]) > 1)
			&& (find_pelicula(input[i]) == 0))
			input[i] = change_val2(input[i], 0, 0);
		i++;
	}
	return (input);
}
