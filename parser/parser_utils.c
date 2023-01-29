/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 16:56:15 by jlebre            #+#    #+#             */
/*   Updated: 2023/01/29 23:39:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *commands)
{
	int	i;
	int	quotes;
	int	dquotes;

	i = 0;
	quotes = 0;
	dquotes = 0;
	while (commands[i])
	{
		if (commands[i] == '\'' && dquotes == 0 && quotes == 0)
			quotes = 1;
		else if (commands[i] == '\'' && dquotes == 0 && quotes == 1)
			quotes = 0;
		else if (commands[i] == '\"' && dquotes == 0 && quotes == 0)
			dquotes = 1;
		else if (commands[i] == '\"' && dquotes == 1 && quotes == 0)
			dquotes = 0;
		i++;
	}
	if (quotes || dquotes)
		return (1);
	return (0);
}

int	skip_quotes(char *input, int i, char quote)
{
	int	j;

	j = ++i;
	while (input[j] && input[j] != quote)
		j++;
	if (!input[j])
		return (i);
	return (j);
}

// Se a seguir a > estiver um |, dá o mesmo erro
/*
if () // Se a seguir a > estiver \n dá erro
{
	ft_error("minishell: syntax error near unexpected token `newline'\n");
	return ;
}
*/

char	*ft_put_space_before(char *input, int i)
{
	char *new;
	int   len;
	int   j;

	j = 0;
	len = ft_strlen(input);
	new = malloc(sizeof(char) * (len + 2));
	if (!new)
		return (NULL);
	while (input[j] && (j < (i + 1)))
	{
		new[j] = input[j];
		printf("%c", new[j]);
		j++;
	}
	j++;
	new[j] = ' ';
	printf("%c", new[j]);
	j++;
	i ++;
	while (i < (len + 1))
	{
		new[j] = input[i];
		printf("%c", new[j]);
		i++;
		j++;
	}
	new[j] = '\0';
	printf("\n");
	return (new);
}

char	*ft_put_space_after(char *input, int i)
{
	char *new;
	int   len;
	int   j;

	j = 0;
	len = ft_strlen(input);
	new = malloc(sizeof(char) * (len + 2));
	if (!new)
		return (NULL);
	while (input[j] && (j < i))
	{
		new[j] = input[j];
		printf("%c", new[j]);
		j++;
	}
	j++;
	new[j] = ' ';
	printf("%c", new[j]);
	j++;
	//i += 2;
	while (i < (len + 1))
	{
		new[j] = input[i];
		printf("%c", new[j]);
		i++;
		j++;
	}
	new[j] = '\0';
	printf("\n");
	return (new);
}

char	*separate_input(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if ((input[i + 1] == '>' || input[i + 1] == '<'
			|| input[i + 1] == '|') && input[i] != ' ')
		{
			input = ft_put_space_before(input, i);
			i += 2;
			while (input[i] && (input[i] == '>'
			|| input[i] == '<' || input[i] == '|'))
				i++;	
		}
		if (input[i] && input[i] != ' ' && (input[i - 1] == '>'
			|| input[i - 1] == '<' || input[i - 1] == '|'))
		{
			input = ft_put_space_after(input, i);
			i += 2;
		}
		i++;
	}
	printf("test\n");
	return(input);
}

int	verify_redir_2(char *input)
{
	int	i;
	
	i = 0;
	printf("input: %s\n", input);
	while (input[i])
	{
		if (input[i] == '>' && input[i + 1] == '>' && (input[i + 2] == '>'
				|| input[i + 2] == '<' || input[i + 2] == '|'))
		{
			ft_error("minishell: syntax error near unexpected token `newline'\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	verify_redir(char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			j = i + 1;
			while (input[j] && ft_space(input[j]))
				j++;
			if (input[j] != input[i] && (input[j] == '>'
					|| input[j] == '<' || input[j] == '|'))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	check_xor(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (input[i + 1] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_and(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '&')
			return (1);
		i++;
	}
	return (0);
}
