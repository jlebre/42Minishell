/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 17:03:49 by nvideira          #+#    #+#             */
/*   Updated: 2023/01/25 00:12:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Shell_Split não está de acordo com a Norminette

int	find_quotes(const char *str, int i, int type)
{
	int	st;

	st = i;
	i++;
	if (type == 34)
	{
		while (str[i] && str[i] != 34)
			i++;
		if (str[i] == 34)
			return (i);
	}
	else
	{
		while (str[i] && str[i] != 39)
			i++;
		if (str[i] == 39)
			return (i);
	}
	return (st);
}

static int	ft_space(char s)
{
	if (s == '\n' || s == '\t' || s == '\v'
		|| s == '\f' || s == '\r' || s == ' ')
		return (1);
	else
		return (0);
}

static int	ft_ispipe(char s, char c)
{
	if (s == c || s == '\0')
		return (1);
	else
		return (0);
}

int	ft_wordcount(const char *str, char c)
{
	int		words;
	size_t	i;

	i = 0;
	words = 0;
	while (str[i])
	{
		if (str[i] == 34)
			i = find_quotes(str, i, 34);
		if (str[i] == 39)
			i = find_quotes(str, i, 39);
		if (ft_ispipe(str[i], c) == 0 && ft_ispipe(str[i + 1], c) == 1)
			words++;
		i++;
	}
	return (words);
}

static int	split_it(char const *str, char c, int st, char **ns)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] != '\0' && str[i] == c)
		{
			i++;
			st++;
		}
		if (str[i] == 34)
			i = find_quotes(str, i, 34);
		if (str[i] == 39)
			i = find_quotes(str, i, 39);
		if (str[i] == '\0')
			break ;
		if (ft_ispipe(str[i], c) == 0 && ft_ispipe(str[i + 1], c) == 1)
		{
			while ((ft_space(str[st]) || ft_ispipe(str[st], c)) && str[st])
				st++;
			ns[j] = ft_substr(str, st, (i - st) + 1);
			if (!ns[j])
				return (0);
			j++;
			st = i + 1;
		}
		i++;
	}
	ns[1] = NULL;
	return (1);
}

//	Está a dar problemas ao corre o fsanitize
//	Não está a conseguir pôr o NULL corretamente,
//	portanto as outras funções não estão a funcionar
//	quando dependem de (while != NULL)

void	*freematrix(char **ns, int msize)
{
	while (msize--)
		free(ns[msize]);
	free(ns);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char	**ns;
	size_t	st;
	int		matlen;

	st = 0;
	if (!s)
		return (NULL);
	matlen = ft_wordcount(s, c);
	ns = malloc(sizeof(char *) * matlen + 1);
	if (!ns)
		return (NULL);
	if (!split_it(s, c, st, ns))
		return (freematrix(ns, matlen));
	return (ns);
}

/*
int count_words(char* str, const char* delimiters)
{
    int count = 0;
    int i = 0;

    while (i < ft_strlen(str)) {
        if (ft_strchr(delimiters, str[i])) {
            if (str[i - 1] != '\'' && str[i + 1] != '\'') {
                count++;
            }
        }
        i++;
    }

    return count;
}

void split_it(char* str, const char* delimiters, char*** new) {
    int i;
    int words = 0;
    char* start;

    i = 0;
	words = 0;
    *new = (char**)malloc(sizeof(char*) * (count_words(str, delimiters) + 1));
	start = str;
    while (i < ft_strlen(str))
	{
        if (ft_strchr(delimiters, str[i]))
		{
            if (str[i - 1] != '\'' && str[i + 1] != '\'')
			{
                str[i] = '\0';
                (*new)[words] = start;
                words++;
                start = &str[i + 1];
            }
        }
        i++;
    }
    (*new)[words] = start;
}

char	**ft_split(char *str, char *delimiters)
{
	char **new;

	split_it(str, delimiters, &new);
	return (new);
}
*/
