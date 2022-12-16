/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 23:31:01 by marvin            #+#    #+#             */
/*   Updated: 2022/11/17 23:31:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Se não tiveres " " e tiveres mais de 1 argumento tem de imprimir todos
//O que faz o -e ?

void	do_print(char **input, int start, int type)
{
	while (start < com_info()->commands->nb_args)
	{
		printf("%s", input[start]);
		if ((com_info()->commands->nb_args - start) != 1 && type == 3)
			printf("\n");
		else if ((com_info()->commands->nb_args - start) != 1 && type != 3)
			printf(" ");
		start++;
	}
	if (type != 2)
		printf("\n");
	com_info()->exit_value = 0;
}

void	ft_echo(char **input)
{
	if (!ft_strncmp(input[1], "$?", 3))
		printf("%d\n", com_info()->exit_value);
	else if (input[1][0] == '$' && ft_strlen(input[1]) < 2)
		printf("Error: no such file \"%s\"\n", input[1]);
	else if (input[1][0] == '$')
		print_vars(input);
	else if (!ft_strncmp(input[1], "-n", 3))
		do_print(input, 2, 2);
	else if (!ft_strncmp(input[1], "-e", 3))
		do_print(input, 2, 3);
	else
		do_print(input, 1, 1);
}

/*
Se tiver uma barra sozinha \, tem de ignorar
Se tiver uma barra com alguma coisa a frente, tem de processar
*/

/*
INPUT: echo a b c
a b c

INPUT: echo "a" "b" "c"
a b c

INPUT: echo a\nb\nc
anbnc

INPUT: echo a \n b \n c
a n b n c

INPUT: echo "a\nb\nc"
a
b
c
*/