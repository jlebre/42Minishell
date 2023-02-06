/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 01:09:31 by marvin            #+#    #+#             */
/*   Updated: 2023/01/04 01:09:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if ((str[i] > 47) && (str[i] < 58))
			i++;
		else
			return (0);
	}
	return (1);
}
