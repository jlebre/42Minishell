#include "../minishell"

int	matmeasures(char **input, char *set)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		j = 0;
		while (input[i][j])
		{
			if (ft_strchr(set, input[i][j]))
				count++;
			j++;
		}
		i++;
	}
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

int	split_all(char **input, char *set, int start, char ***new)
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
}

char ***split_redir(char **input, char *set)
{
	char	***new;
	size_t	start;
	int		matlen;

	start = 0;
	if (!input)
		return (NULL);
	matlen = ft_matmeasures(input, set);
	new = (char ***)malloc(sizeof(char **) * (matlen + 1));
	if (!new)
		return (NULL);
	
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
	fd_close(com_info()->cmds_done_redir);
	unlink(".heredoc");
}

void	execute_redir(char **input)
{
	char	***new;
	int 	i;

	i = 0;
	check_redir(input);
	//new = split_redir(input);
	while (new[i])
	{
		do_redir(new[i]);
		i++;
	}
}
