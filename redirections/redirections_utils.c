#include "../minishell.h"

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

/*
int	check_redir_type(char *input, int j)
{
	if (input[j] == '>')
	{
		if (input[j + 1] == '>')
			return (1);
		else
			return (2);
	}
	else if (input[j] == '<')
	{
		if (input[j + 1] == '<')
			return (3);
		else
			return (4);
	}
	return (0);
}
*/

void	redirections(char **input, int i, int j, int type)
{
	int		fd;
	char	*file;

	if (input[i + 1] && input[i + 1] == input[i])
		perror("minishell: syntax error near unexpected token");
	if (((type == 1 || type == 3) && input[i][j + 2] != '\0'))
		file = ft_substr(input[i], j + 2, ft_strlen(input[i]));
	else if ((type == 2 || type == 4) && input[i][j + 1] != '\0')
		file = ft_substr(input[i], j + 1, ft_strlen(input[i]));
	else
		file = ft_substr(input[i + 1], 0, ft_strlen(input[i + 1]));
	if (type == 1)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (type == 2)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == 4)
		fd = open(file, O_RDONLY);
	else if (type == 3)
		fd = heredoc(file);
	if (type == 1 || type == 2)
		dup2(fd, STDOUT_FILENO);
	else if (type == 4 || type == 3)
		dup2(fd, STDIN_FILENO);
	close(fd);
	free(file);
}

/*
void	check_redir(char **input)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		j = 0;
		while (input[i][j])
		{
			if (input[i][j] == '>' || input[i][j] == '<')
			{
				com_info()->redir_type = check_redir_type(input[i], j);
				break ;
			}
			j++;
		}
		i++;
	}
}
*/

/* void	check_redir(char **input)
{
	int	i;
	int	j;
	int	type;

	i = 0;
	type = 0;
	while (input[i])
	{
		j = 0;
		while (input[i][j])
		{
			if (input[i][j] == '>' || input[i][j] == '<')
			{
				type = check_redir_type(input[i], j);
				redirections(input, i, j, type);
			}
			j++;
		}
		i++;
	}
}
 */
