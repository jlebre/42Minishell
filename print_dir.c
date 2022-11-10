/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:17:25 by jlebre            #+#    #+#             */
/*   Updated: 2022/11/10 15:02:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*print_info(void)
{
	char	cwd[1024];
	char	*str;
	char	*tmp;
	char	*username;
	char	*dir;
	
	dir = getcwd(cwd, sizeof(cwd));
	username = getenv("USER");
	tmp = ft_strjoin(username, "\033[0;34m:");
	str = ft_strdup(tmp);
	free(tmp);
	tmp = ft_strjoin(str, dir);
	free(str);
	str = ft_strjoin(tmp, "$ \033[0m");
	free(tmp);
	return (str);
}

/*
char	*print_info(void)
{
	char	cwd[1024];
	char	*str;
	char	*username;
	char	*dir;
	
	dir = getcwd(cwd, sizeof(cwd));
	username = getenv("USER");
	str = ft_strjoin(username, "\033[0;34m:");
	str = ft_strjoin(str, dir);
	str = ft_strjoin(str, "$ \033[0m");
	return (str);
}
*/

char	*print_dir(void)
{
	char	cwd[1024];

	return (getcwd(cwd, sizeof(cwd)));
}
