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

// A função change_val2 serve para alterar o valor de $_ no meio de uma str

int	is_valid(char c)
{
	if (c == ' ' || c == '\n' || c == '\v' || c == '\0'
		|| c == '\r' || c == '\t' || c == '\f' || c == '$')
		return (0);
	return (1);
}

char	*create_new(char *input, int i, int j, char *name)
{
	char	*new;
	char	*val;
	int		k;
	int		l;
	int		size;

	k = 0;
	l = 0;
	val = ft_strdup(change_val(name));
	size = ((ft_strlen(input) - j) + ft_strlen(val));
	new = malloc(sizeof(char) * size);
	if (!new)
		return (NULL);
	while (k < (i - j - 1))
	{
		new[k] = input[k];
		k++;
	}
	while (val[l])
		new[k++] = val[l++];
	while (new[k] || k < size)
		new[k++] = input[i++];
	new[k] = '\0';
	return (new);
}

char	*remove_ds(char *input, int size)
{
	char	*new;
	int 	i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(input) - size));
	if (!new)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$')
		{
			i++;
			while (is_valid(input[i]) && input[i])
				i++;
			while (input[i])
				new[j++] = input[i++];
		}
		else
			new[j++] = input[i++];
	}
	new[j] = '\0';
	return (new);
}

int	check_if_exists_ds(char *input, t_env_lst *temp)
{
	input++;
	while (temp)
	{
		if (!ft_strncmp(ft_strjoin(input, "="), temp->name, ft_strlen(input)))
			return (1);
		temp = temp->next;
	}
	return (0);
}

char	*change_val2(char *input)
{
	int			i;
	int			j;
	char		*name;
	
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			i++;
			while (is_valid(input[i]))
			{
				i++;
				j++;
			}
			name = ft_substr(input, (i - j - 1), (j + 1));
			if (check_if_exists_ds(name, com_info()->env_lst)
				|| check_if_exists_ds(name, com_info()->vars))
				input = create_new(input, i, j, name);
			else
				input = remove_ds(input, ft_strlen(name));
			i = 0;
			j = 0;
		}
		else
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
		if (!ft_strncmp(input[i], "$?", 3) && find_pelicula(input[i]) != 2)
			input[i] = ft_itoa(com_info()->exit_value);
		else if (input[i][0] == '$' && ft_strlen(input[i]) > 1)
			input[i] = ft_strdup(change_val(input[i]));
		else if ((ft_strchr(input[i], '$') != 0) && (ft_strlen(input[i]) > 1)
			&& (find_pelicula(input[i]) == 0))
			input[i] = change_val2(input[i]);
		i++;
	}
	return (input);
}
