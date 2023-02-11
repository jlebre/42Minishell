/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_output.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:07:27 by marvin            #+#    #+#             */
/*   Updated: 2023/02/11 18:07:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_output_fd(char *input, int nb, int count)
{
	char	*filename;
	int		fd;

	if (input[count + 1] == '>')
	{
		count++;
		filename = get_filename(input, count + 1);
		fd = open(filename, O_RDWR | O_APPEND | O_CREAT, 0666);
	}
	else
	{
		filename = get_filename(input, count + 1);
		fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0666);
	}
	if (nb == 1)
	{
		free(filename);
		return (fd);
	}
	free(filename);
	close(fd);
	return (count * -1);
}

int	redirect_output(char *input)
{
	int	fd;
	int	i;
	int	nb;

	i = 0;
	fd = -1;
	nb = count_redir(input, '>');
	while(nb && input[i])
	{
		if (input[i] != '>')
			i++;
		else if (1)
		{
			fd = get_output_fd(input, nb, i);
			if (fd <= 0)
				i = (fd * -1) + 1;
			else
				return (fd);
			nb--;
		}
		else
			i++;
	}
	if (fd != -1 || i == -1)
		ft_error("Redirection output error\n");
	return (fd);
}
