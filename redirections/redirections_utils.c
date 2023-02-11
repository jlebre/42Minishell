/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:10:11 by marvin            #+#    #+#             */
/*   Updated: 2023/02/11 18:10:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_redir(char *input, char redir)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == redir)
		{
			if (input[i + 1] == redir)
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

char	*get_filename(char *input, int count)
{
	char	*filename;
	int		i;

	i = 0;
	filename = malloc(sizeof(char) * (ft_strlen(input) + 1));
	while (input[count] && input[count] == ' ')
		count++;
	while (ft_strchr("<> ", input[count]) == 0 && input[count])
	{
		filename[i] = input[count];
		i++;
		count++;
	}
	filename[i] = '\0';
	return (filename);
}

int	check_file_access(char *file)
{
	if (access(file, F_OK))
	{
		ft_error("%s: No such file or directory\n", file);
		return (1);
	}
	else if (access(file, R_OK))
	{
		ft_error("%s: Permission denied\n", file);
		return (1);
	}
	return (0);
}

int	count_redirs(char **input)
{
	int	i;
	int	j;
	int	redir_no;

	i = 0;
	redir_no = 0;
	while (input[i])
	{
		j = 0;
		while (input[i][j])
		{
			if (input[i][j] == '>' || input[i][j] == '<')
			{
				if ((input[i][j] == '>' && input[i][j + 1] == '>')
					|| (input[i][j] == '<' && input[i][j + 1] == '<'))
					j++;
				redir_no++;
			}
			j++;
		}
		i++;
	}
	com_info()->redir_no = redir_no;
	return (redir_no);
}

int	check_redir_type(char *input)
{
	if (input[0] == '>')
	{
		if (input[1] && input[1] == '>')
			return (1);
		else
			return (2);
	}
	else if (input[0] == '<')
	{
		if (input[1] && input[1] == '<')
			return (3);
		else
			return (4);
	}
	return (0);
}
