/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ds_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 04:25:07 by marvin            #+#    #+#             */
/*   Updated: 2023/01/12 04:25:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char c)
{
	if (c == ' ' || c == '\n' || c == '\v' || c == '\0'
		|| c == '\r' || c == '\t' || c == '\f' || c == '$'
		|| c == '\'')
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

// Remove $
char	*remove_ds(char *input, int size)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(input) - size));
	if (!new)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$' && i < size)
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

// Check If Dollar Sign Exists
int	cds(char *input, t_env_lst *temp)
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
