#include "../minishell.h"

int	ft_matmeasures(char **input)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 1;
	while (input[i])
	{
		j = 0;
		while (input[i][j])
		{
			if (input[i][j] == '>' || input[i][j] == '<')
			{
				if (input[i][j + 1] == '>' || input[i][j + 1] == '<')
					j++;
				count += 2;
			}
			j++;
		}
		i++;
	}
	return (count);
}

int	count_second_word(char **input, int i)
{
	int count;

	count = 0;
	while (input[i] && input[i][0] != '>' && input[i][0] != '<')
	{
		count++;
		i++;
	}
	if (count == 0)
		return (1);
	return (count);
}

int	ft_strstr(char *str, char *set)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (set[j])
		{
			if (str[i] == set[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	get_size(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '>' && input[i] != '<')
		i++;
	return (i);
}

void	fill_word(char **input, int i, int nb_words, char **new)
{
	int j;

	j = 0;
	while (j < nb_words)
	{
		new[j] = ft_strdup(input[i]);
		if (!new[j])
			return ;
		j++;
		i++;
	}
}

int	split_all(char **input, char ***new, int matlen)
{
	int		i;
	int		j;
	int		nb_words;

	i = 0;
	j = 0;
	printf("NB_ARGS: %i\n", com_info()->nb_args);
	while (j < matlen)
	{
		nb_words = count_second_word(input, i);
		i += nb_words;
		printf("nb_words[%i]: %i\n", j, nb_words);
		new[j] = malloc(sizeof(char *) * (nb_words + 1));
		if (!new[j])
			return (0);
		fill_word(input, i, nb_words,new[j]);
		j++;
	}
	new[nb_words] = NULL;
	return (0);
}

//	Tem de se criar uma função no parser para confirmar se os redir
//	estão rodeados por espaços! Para o input vir "ls > a.txt" em vez de 
//	"ls>a.txt"

//	O split está feito a contar que as coisas vêm bem separadas

char ***split_redir(char **input)
{
	char	***new;
	int		matlen;

	if (!input)
		return (NULL);
	matlen = ft_matmeasures(input);
	printf("Total Len: %i\n", matlen);
	new = (char ***)malloc(sizeof(char **) * (matlen + 1));
	if (!new)
		return (NULL);
	split_all(input, new, matlen);
	new[matlen] = NULL;
	return (new);
}

void	do_redir(char **input)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		//	Como por o i e o j
		//	redirections(new[i], i, j, com_info()->redir_type);
		commands(input, com_info()->env, 1);
	}
	else
		waitpid(pid, &com_info()->exit_value, 0);
	//fd_close(com_info()->cmds_done_redir);
	unlink(".heredoc");
}

void	execute_redir(char **input)
{
	char	***new;
	int 	i;
	int		j;

	i = 0;
	j = 0;
	while (input[i])
	{
		printf("input[%i]: %s\n", i, input[i]);
		i++;
	}
	com_info()->nb_args = i;
	i = 0;
	new = split_redir(input);
	while (new[i])
	{
		while (new[i][j])
		{
			printf("matrix[%i][%i]: %s\n", i, j, new[i][j]);
			j++;
		}
		//do_redir(new[i]);
		i++;
	}
}

/*
int	split_all(char **input, int start, char ***new)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (input[i])
	{
		// while (input[i][j] && !ft_strchr(set, input[i][j]))
		// 	j++;
		while (input[i][j])
		{
			if (input[i][j] == 34)
				j = find_quotes(input[i], j, 34);
			if (input[i][j] == 39)
				j = find_quotes(input[i], j, 39);
			if (input[i][j] == '\0')
				break ;
		}
	}
	return (0);
}
*/
