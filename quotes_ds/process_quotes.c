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
char	*process_quotes(char *input)
{
	if (find_quote(input) % 2 == 0 && find_quote(input) != 0)
		input = remove_quotes(input);
	return (input);
}

// Processa aspas simples
char	*process_peliculas(char *input)
{
	if (find_pelicula(input) % 2 == 0 && find_pelicula(input) != 0)
		input = remove_peliculas(input);
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
			if (!special_quote(input, i) && !surround_quote(input, i, '"'))
				return (1);
		}
		i++;
	}
	return (0);
}

int	surround_quote(char *input, int index, int quote)
{
	int i;
	int quote_nb;
	int	a;

	i = 0;
	a = 0;
	quote_nb = 0;
	while (input[i])
	{
		if (quote_nb == 2)
			quote_nb = 0;
		if (input[i] == quote)
			quote_nb++;
		if (i == index)
			a = 1;
		if (quote_nb == 1 && i > index && a == 1)
			return (1);
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