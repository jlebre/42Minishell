/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 16:56:15 by jlebre            #+#    #+#             */
/*   Updated: 2023/02/11 17:45:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int parser_checks(char *input)
{
	if (empty_prompt(input))
	{
		free(input);
		return (1);
	}
	if (ft_strlen(input))
		add_history(input);
	return (0);
}

void	parser_checks2(char *input)
{
	if (check_quotes(input))
	{
		ft_error("minishell: syntax error: unclosed quotes\n");
		return ;
	}
	if (check_xor(input))
	{
		ft_error("minishell: syntax error near unexpected token `|'\n");
		return ;
	}
	if (check_and(input))
	{
		ft_error("minishell: syntax error near unexpected token `&'\n");
		return ;
	}
	if (!verify_redir(input))
	{
		ft_error("minishell: syntax error near unexpected token `'\n");
		return ;
	}
	if (!verify_redir_2(input))
		return ;
}

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

int	check_xor(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			i++;
			while (input[i] && is_space(input[i]))
				i++;
			if (input[i] == '|')
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
