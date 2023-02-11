/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 00:51:18 by marvin            #+#    #+#             */
/*   Updated: 2023/01/04 00:51:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Processa aspas duplas
char	**process_quotes(char **input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (find_quote(input[i]) % 2 == 0 && find_quote(input[i]) != 0)
			input[i] = remove_quotes(input[i]);
		i++;
	}
	return (input);
}

// Processa aspas simples
char	**process_peliculas(char **input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (find_pelicula(input[i]) % 2 == 0 && find_pelicula(input[i]) != 0)
			input[i] = remove_peliculas(input[i]);
		i++;
	}
	return (input);
}

int	check_special(char *input, char c)
{
	int i;

	i = 0;
	while (input[i])
	{
		if (input[i] == c)
		{
			if (special_quote(input, i) == 0)
				return (1);
		}
		i++;
	}
	return (0);
}

int	special_quote(char *input, int i)
{
	int a;
	int b;
	int count;

	if (i == -1)
		return (0);
	a = 0;
	b = 0;
	count = i;
	while (count >= 0)
	{
		if (input[count] == '\"' && count >= 1
			&& input[count - 1] != '\\')
			a++;
		else if (input[count] == '\'' && count >= 1
			&& input[count - 1] != '\\')
			b++;
		count--;
	}
	if (a % 2 == 0 && b % 2 == 0)
		return (0);
	return (1);
}