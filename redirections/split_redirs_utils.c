/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_redirs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 01:59:57 by marvin            #+#    #+#             */
/*   Updated: 2023/01/26 01:59:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_matmeasures(char **input)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 1;
	while (input[i])
	{
		j = 0;
		while (input[i][j])
		{
			if (input[i][j] == '>' || input[i][j] == '<')
			{
				if (input[i][j + 1] == '>' || input[i][j + 1] == '<')
					j++;
				count += 2;
			}
			j++;
		}
		i++;
	}
	return (count);
}

int	count_second_word(char **input, int i)
{
	int count;

	count = 0;
	if (input[i][0] == '>' || input[i][0] == '<')
		return (1);
	while (input[i] && input[i][0] != '>' && input[i][0] != '<')
	{
		count++;
		i++;
	}
	return (count);
}

int	ft_strstr(char *str, char *set)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (set[j])
		{
			if (str[i] == set[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	get_size(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '>' && input[i] != '<')
		i++;
	return (i);
}

void print_matrix_redir(char ***matrix)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (matrix[i])
	{
		j = 0;
		while (matrix[i][j])
		{
			printf("matrix[%i][%i]: %s\n", i, j, matrix[i][j]);
			j++;
		}
		i++;
	}
}
