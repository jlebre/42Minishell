#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <dirent.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <fcntl.h>
#include <errno.h>

typedef struct s_env_lst
{
	char				*name;
	char				*value;
	struct s_env_lst	*next;
	struct s_env_lst	*prev;
}	t_env_lst;

typedef struct s_args
{
	char				**arg;
	int					nb_args;
	int					redir_type;
	struct s_args		*next;
}	t_args;

typedef struct s_command
{
	int					**pip;
	pid_t				*pid;
	int					pid_counter;
	int					cmds_done;
	int					exit_value;
	t_env_lst			*env_lst;
	t_env_lst			*vars;
	t_args				*commands;
	int					nb_args;
	char				*color;
	int					pipe_no;
	int					redir_no;
	char				**env;
	int					redir_type;
	int					hereflag;
}	t_command;

t_command	*com_info(void)
{
	static t_command	a;

	return (&a);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;

	(void)argc;
	(void)argv;
	init_shell(env);
	while (1)
	{
		catch_signal();
		input = readline(print_info());
		if (!input)
		{
			write(1, "exit\n", 5);
			rl_clear_history();
			free (input);
			exit(com_info()->exit_value >> 8 & 0xFF);
		}
		parser(input);
		process_input(env);
	}
	return (com_info()->exit_value);
}

static void	ctrl_c(int sig)
{
	if (sig == 2)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	catch_signal(void)
{
	signal(SIGINT, &ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

static void	ctrl_c_block(int sig)
{
	if (sig == 2)
	{
		rl_on_new_line();
		write(1, "\n", 1);
		rl_replace_line("", 0);
	}
}

static void	ctrl_s_block(int sig)
{
	(void) sig;
	write(1, "Quit (core dumped)\n", 19);
	rl_on_new_line();
	rl_replace_line("", 0);
}

//Para não dar prompt várias vezes
//Ctrl + C
//Ctrl + /
void	signal_block(void)
{
	signal(SIGINT, &ctrl_c_block);
	signal(SIGQUIT, &ctrl_s_block);
}


char	*get_cenas_do_env(char *str)
{
	char		*user;
	t_env_lst	*temp;

	temp = com_info()->env_lst;
	while (temp)
	{
		if (!ft_strncmp(temp->name, str, sizeof(str)))
		{
			user = ft_strdup(temp->value);
			return (user);
		}
		temp = temp->next;
	}
	return (NULL);
}

// Função que inicializa a shell
void	init_shell(char **env)
{
	char	*username;

	com_info()->env_lst = env_to_lst(env);
	com_info()->vars = NULL;
	com_info()->cmds_done = 0;
	com_info()->redir_type = 0;
	com_info()->hereflag = 0;
	com_info()->color = "\033[1;32m:";
	com_info()->env = env;
	if (check_if_exists("USER", com_info()->env_lst))
		username = getenv("USER");
	else
	{
		write(1, "USER not found, setting to 'user'\n", 35);
		lst_add_back(&com_info()->env_lst, new_node("USER=user"));
		username = get_cenas_do_env("USER=");
	}
	write(1, "\n\nUSER is: @", 12);
	write(1, username, ft_strlen(username));
	write(1, "\n", 1);
}


char	**parse_cenas(char **arg)
{
	arg = process_quotes(arg);
	arg = check_ds(arg);
	arg = process_peliculas(arg);
	return (arg);
}

char	*parse_input(char *input)
{
	input = put_spaces(input);
	input = separate_input(input);
	return (input);
}

void	parser(char *input)
{
	char		*tmp2;
	char		*tmp3;

	tmp2 = NULL;
	tmp3 = NULL;
	if (empty_prompt(input))
		return ;
	if (tmp2)
	{
		tmp3 = ft_strjoin(input, tmp2);
		free(tmp2);
		free(input);
		input = ft_strdup(tmp3);
		free(tmp3);
	}
	if (ft_strlen(input))
		add_history(input);
	parser2(input);
}

void	parser2(char *input)
{
	input = parse_input(input);
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
	parser3(input);
}

void	parser3(char *input)
{
	char		**tmp;
	int			pipe_no;
	int			i;

	i = 0;
	pipe_no = count_pipes(input);
	com_info()->pipe_no = pipe_no;
	tmp = ft_split(input, '|');
	while (pipe_no >= 0)
	{
		lst_add_front(&com_info()->commands, add_mat_node(tmp[pipe_no]));
		pipe_no--;
		i++;
	}
	free_matrix(tmp);
	com_info()->commands->arg = parse_cenas(com_info()->commands->arg);
	if (com_info()->pipe_no > 0)
		init_pipes();
	// count_redirs_total(com_info()->commands->arg);
	// 		com_info()->pid = malloc(sizeof(pid_t *) * (com_info()->pipe_no + com_info()->redir_no + 2));
	// com_info()->pid_counter = 0;
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

void	print_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		printf("matrix[%i] = %s\n", i, matrix[i]);
		i++;
	}
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

int	empty_prompt(char *input)
{
	int	i;

	i = 0;
	if (!input[i])
		return (1);
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

int	is_space(char c)
{
	if (c == '\n' || c == '\t' || c == '\v'
		|| c == '\f' || c == '\r' || c == '\0'
		|| c == ' ')
		return (1);
	return (0);
}

int	count_words(char *input)
{
	int	i;
	int	count;
	int	words;

	i = 0;
	count = 0;
	words = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i] && !is_space(input[i]))
		{
			words++;
			while (input[i] && !is_space(input[i]))
			{
				count++;
				i++;
			}	
		}
	}
	return (count + words);
}

char	*put_spaces(char *input)
{
	int		i;
	int		j;
	char	*new;
	int		words;

	i = 0;
	j = 0;
	words = count_words(input);
	new = malloc(sizeof(char) * (words + 1));
	if (!new)
		return (NULL);
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		while (input[i] && !is_space(input[i]))
			new[j++] = input[i++];
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i])
			new[j++] = ' ';
	}
	new[j] = '\0';
	return (new);
}

char	*ft_put_space_before(char *input, int i)
{
	char	*new;
	int		len;
	int		j;

	j = 0;
	len = ft_strlen(input);
	new = malloc(sizeof(char) * (len + 2));
	if (!new)
		return (NULL);
	while (input[j] && (j < (i + 1)))
	{
		new[j] = input[j];
		j++;
	}
	new[j++] = ' ';
	i++;
	while (i < (len + 1))
	{
		new[j] = input[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}

char	*ft_put_space_after(char *input, int i)
{
	char	*new;
	int		len;
	int		j;

	j = 0;
	len = ft_strlen(input);
	new = malloc(sizeof(char) * (len + 2));
	if (!new)
		return (NULL);
	while (input[j] && (j < i))
	{
		new[j] = input[j];
		j++;
	}
	new[j] = ' ';
	j++;
	while (i < (len + 1))
	{
		new[j] = input[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}

char	*separate_input(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<'	|| input[i] == '|')
		{
			if (input[i - 1] != ' ' && input[i - 1] != input[i])
				input = ft_put_space_before(input, i - 1);
			if (input[i + 1] && input[i + 1] != ' ' && input[i + 1] != input[i])
				input = ft_put_space_after(input, i + 1);
		}
		i++;
	}
	return (input);
}

void	process_input(char **env)
{
	while (com_info()->commands)
	{
		com_info()->commands->nb_args = count_args(com_info()->commands->arg);
		if (find_es(com_info()->commands->arg[0]) == 1)
			exported_vars(com_info()->commands->arg);
		else if (count_redirs(com_info()->commands->arg) > 0)
			execute_redir(com_info()->commands->arg);
		else if (com_info()->pipe_no > 0)
			execute_pipe(com_info()->commands->arg);
		else
			commands(com_info()->commands->arg, env, 0);
		com_info()->cmds_done++;
		com_info()->commands = com_info()->commands->next;
		catch_signal();
	}
	unlink(".heredoc");
	//pipe_cleanup();
	ft_wait_pid();
}

void	change_color(char	**input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	if (!input[1])
	{
		printf("\033[0;31mChange_Color: Not enough arguments\n");
		printf("Try \"change_color --help\" for help\n\033[0m");
		return ;
	}
	if (i > 3)
	{
		printf("\033[0;31mChange_Color: Too much arguments\n");
		printf("Try \"change_color --help\" for help\n\033[0m");
		return ;
	}
	if (!ft_strncmp(input[1], "--help", 6))
		change_color_help();
	else if (ft_isdigit(input[1]) && ft_strlen(input[1]) == 1)
		do_change(input, i);
	else
		printf("\033[0;31mChange_Color: Invalid arguments\n\033[0m");
}

// Função que muda a cor do prompt
void	do_change(char **input, int bold)
{
	char	*b;
	char	*color;

	if (bold == 3)
	{
		if (ft_strlen(input[2]) != 1 || ft_atoi(input[1]) < 0
			|| ft_atoi(input[1]) > 7 || ft_atoi(input[2]) < 0
			|| ft_atoi(input[2]) > 1)
			printf("\033[0;31mChange_Color: Invalid arguments\033[0m\n");
		else
			b = "1";
	}
	else
		b = "0";
	color = ft_strjoin("\033[", b);
	color = ft_strjoin(color, ";3");
	color = ft_strjoin(color, input[1]);
	color = ft_strjoin(color, "m:");
	com_info()->color = color;
	free(color);
}

// Menu de ajuda
void	change_color_help(void)
{
	printf("HELP:\n");
	printf("0: \033[1;30mBlack\033[0m\n");
	printf("1: \033[0;31mRed\033[0m\n");
	printf("2: \033[0;32mGreen\033[0m\n");
	printf("3: \033[0;33mYellow\033[0m\n");
	printf("4: \033[0;34mBlue\033[0m\n");
	printf("5: \033[0;35mPurple\033[0m\n");
	printf("6: \033[0;36mCyan\033[0m\n");
	printf("7: \033[0;37mWhite\033[0m\n\n");
	printf("To make the color bold, ");
	printf("add a third argument with the number \"1\"\n");
	printf("\nchange_color 2 1\n\n");
	printf("Changes prompt color to \033[0;32mBold Green\033[0m\n\n\n\n");
}

void	commands(char **input, char **env, int is_fork)
{
	if (input[0])
	{
		if (!ft_strncmp(input[0], "echo", 5))
			ft_echo(input);
		else if (!ft_strncmp(input[0], "cd", 3))
			ft_cd(input, env);
		else if (!ft_strncmp(input[0], "pwd", 4))
			ft_pwd();
		else if (!ft_strncmp(input[0], "export", 7))
			ft_export(input);
		else if (!ft_strncmp(input[0], "unset", 6))
			ft_unset(input);
		else if (!ft_strncmp(input[0], "env", 4))
			ft_env(input);
		else if (!ft_strncmp(input[0], "exit", 5))
			ft_exit(input);
		else if (!ft_strncmp(input[0], "change_color", 13))
			change_color(input);
		else
			fork_commands(input, env, is_fork);
	}
}

// Esta função é chamada para executar os comandos que não são builtins
// Verifica se está dentro de um fork ou não (por causa dos pipes)
void	fork_commands(char **input, char **env, int is_fork)
{
	int	pid;

	signal_block();
	if (!is_fork)
	{
		pid = fork();
		if (pid == 0)
			env_commands(input, env);
		else
			waitpid(pid, &com_info()->exit_value, 0);
	}
	else
		env_commands(input, env);
}

// Executa os comandos que não são builtins
void	env_commands(char **input, char **env)
{
	char	*path;
	
	path = find_path(input[0], com_info()->env_lst);
	if (!path)
	{
		ft_error("Command not found: %s", input[0]);
		free(path);
		catch_signal();
		exit(127);
	}
	else if (execve(path, input, env) == -1)
	{
		ft_error("Deu Merda\n");
		free(path);
		exit(126);
	}
}

// Encontra o path do comando
char	*find_path(char *cmd, t_env_lst *env_lst)
{
	int			j;
	t_env_lst	*temp;
	char		*path;

	j = 0;
	temp = env_lst;
	while (temp && ft_strncmp(temp->name, "PATH=", 5))
	{
		temp = temp->next;
		j++;
	}
	if (temp == NULL)
		return (0);
	path = temp->value;
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (!access(cmd, F_OK))
			return (cmd);
	}
	else
		return (find_return_path(path, j, cmd));
	return (0);
}

// Junta o path com o comando
char	*find_return_path(char *path, int j, char *cmd)
{
	char		*ret_path;

	while (path[j] && ft_strichr(path, j, ':') > -1)
	{
		ret_path = join_strings(path, j, cmd);
		if (!access(ret_path, F_OK))
			return (ret_path);
		free(ret_path);
		j += ft_strichr(path, j, ':') - j + 1;
	}
	if (path[j] && ft_strichr(path, j, ':') < 0)
	{
		ret_path = join_strings(path, j, cmd);
		if (!access(ret_path, F_OK))
			return (ret_path);
		free(ret_path);
	}
	return (0);
}

// Recria o comando cd
// Tem as funcionalidades de - e ~
// Também tem a funcionalidade de mudar o diretório atual
void	ft_cd(char **input, char **env)
{
	char	*curr;
	char	*new;

	if (!cd_errors(input))
		return ;
	curr = print_dir();
	new = ft_strjoin(curr, "/");
	if (input[1])
	{
		if (!ft_strncmp(input[1], "-", 2))
		{
			if (!check_if_exists("OLDPWD", com_info()->env_lst))
			{
				ft_error("minishell: cd: OLDPWD not set\n");
				return ;
			}
			do_cd(get_cenas_do_env("OLDPWD="), get_cenas_do_env("OLDPWD="), env);
		}
		else
			do_cd(input[1], ft_strjoin(new, input[1]), env);
	}
	else
		do_cd(get_cenas_do_env("HOME="), get_cenas_do_env("OLDPWD="), env);
	com_info()->exit_value = 0;
	change_pwd("OLDPWD=", curr, env);
}

// Função que muda o diretório atual
void	do_cd(char *new_dir, char *new_pwd, char **env)
{
	chdir(new_dir);
	change_pwd("PWD=", new_pwd, env);
}

// Função que vê se há erros no comando cd
int	cd_errors(char **input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	if (i > 2)
	{
		ft_error("cd: string not in pwd: %s\n", input[1]);
		return (0);
	}
	if (i == 2)
	{
		if (ft_strlen(input[1]) == 1 && !ft_strncmp(input[1], "-", 2))
			return (1);
		if (i == 1)
			return (1);
		if (access(input[1], F_OK) != 0)
		{
			ft_error("cd: no such file or directory: %s\n", input[1]);
			return (0);
		}
	}
	return (1);
}

// FUnção que altera o PWD no env
void	change_pwd_env(char *type, int size, char *val, char **env)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env[i])
	{
		if (!ft_strncmp(type, env[i], size))
		{
			while (val[j])
			{
				env[i][j + size] = val[j];
				j++;
			}
			env[i][j + size] = '\0';
		}
		i++;
	}
}

// Função que altera o PWD na lista de variáveis de ambiente
void	change_pwd(char *type, char *str, char **env)
{
	t_env_lst	*head;
	char		*val;

	head = com_info()->env_lst;
	while (com_info()->env_lst)
	{
		if (!ft_strncmp(type, com_info()->env_lst->name, ft_strlen(type)))
		{
			val = ft_strdup(str);
			com_info()->env_lst->value = val;
			change_pwd_env(type, ft_strlen(type), val, env);
			break ;
		}
		com_info()->env_lst = com_info()->env_lst->next;
	}
	com_info()->env_lst = head;
}

int	check_n(char **input)
{
	int i;

	i = 1;
	while (input[1][i] && input[1][i] != ' ')
	{
		if (input[1][i] != 'n' && input[1][i] != 'e')
			return (0);
		i++;
	}
	return (1);
}

// Recria o comando echo
// Tem as funcionalidades de -n e -e
// Também tem a funcionalidade de imprimir o que é passado como argumento
void	ft_echo(char **input)
{
	if (!input[1])
		write(1, "\n", 1);
	else if (!ft_strncmp(input[1], "-n", 2))
	{
		if (check_n(input))
			do_print(input, 2, 2);
		else
			do_print(input, 1, 1);
	}
	else if (!ft_strncmp(input[1], "-e", 3))
		process_flags(input, 2);
	else
		do_print(input, 1, 1);
	com_info()->exit_value = 0;
}

// Imprime o que foi passado para o comando echo
void	do_print(char **input, int start, int type)
{
	while (start < com_info()->commands->nb_args)
	{
		write (1, input[start], ft_strlen(input[start]));
		if ((com_info()->commands->nb_args - start) != 1
			&& ft_strncmp(com_info()->commands->arg[start], "", 1)
			&& ft_strncmp(com_info()->commands->arg[start + 1], "", 1))
			write (1, " ", 1);
		start++;
	}
	if (type != 2)
		write (1, "\n", 1);
	com_info()->exit_value = 0;
}

// Verifica qual flag foi passada para o comando echo -e
void	check_flag(char c)
{
	if (c == 'a')
		write(1, "\a", 1);
	else if (c == 'b')
		write(1, "\b", 1);
	else if (c == 't')
		write(1, "\t", 1);
	else if (c == 'n')
		write(1, "\n", 1);
	else if (c == 'v')
		write(1, "\v", 1);
	else if (c == 'f')
		write(1, "\f", 1);
	else if (c == 'r')
		write(1, "\r", 1);
	return ;
}

// Processa as flags para o comando echo -e
void	process_flags(char **input, int start)
{
	int	i;

	i = 0;
	while (start < com_info()->commands->nb_args)
	{
		while (input[start][i])
		{
			if (input[start][i] == '\\')
			{
				i++;
				if (ft_strchr("abtnvfr", input[start][i]))
					check_flag(input[start][i]);
			}
			else
				write(1, &input[start][i], 1);
			i++;
		}
		if ((com_info()->commands->nb_args - start) != 1)
			write(1, " ", 1);
		start++;
	}
	write(1, "\n", 1);
	com_info()->exit_value = 0;
}

// Recria o comando env
void	ft_env(char **input)
{
	t_env_lst	*temp;

	if (!find_path("env", com_info()->env_lst))
	{
		ft_error("Command not found: env\n");
		com_info()->exit_value = 127;
		return ;
	}
	if (input[1])
	{
		ft_error("Env: '%s': No such file or directory\n",
			input[1]);
		com_info()->exit_value = 127;
		return ;
	}
	temp = com_info()->env_lst;
	while (temp)
	{
		if (ft_strchr(temp->name, '='))
		{
			write(1, temp->name, ft_strlen(temp->name));
			write(1, temp->value, ft_strlen(temp->value));
			write(1, "\n", 1);
		}
		temp = temp->next;
	}
	com_info()->exit_value = 0;
}

// Recria a função exit
void	ft_exit(char **input)
{
	int		i;
	char	*value;

	i = 0;
	while (input[i])
		i++;
	if (i > 2)
	{
		if (!ft_isdigit(input[1]))
			exit_errors(1, input);
		else
			exit_errors(2, input);
	}
	else if (i == 2)
	{
		if (input[1][0] == '$' && ft_strlen(input[1]) > 1)
		{
			value = print_vars_exit(input);
			check_error_3(value, input);
		}
		check_error_3(input[1], input);
	}
	else
		do_exit(com_info()->exit_value, input);
}

// Função que executa o exit
void	do_exit(int exit_value, char **input)
{
	write(1, "exit\n", 5);
	rl_clear_history();
	free (input);
	exit(exit_value);
}

// Função que imprime as variáveis de ambiente
char	*print_vars_exit(char **input)
{
	t_env_lst	*temp;
	char		*name;

	name = input[1];
	temp = com_info()->vars;
	name++;
	while (temp)
	{
		if (!ft_strncmp(name, com_info()->vars->name, ft_strlen(name) - 1))
			return (com_info()->vars->value);
		temp = temp->next;
	}
	return (0);
}

// Verifica se o exit tem erros
void	exit_errors(int error, char **input)
{
	if (error == 1)
	{
		ft_error("exit\nexit: %s: numeric argument required\n", input[1]);
		rl_clear_history();
		free (input);
		exit(2);
	}
	else if (error == 2)
	{
		ft_error("exit\nexit: too many arguments\n");
		com_info()->exit_value = 1;
		return ;
	}
}

// Verifica se o exit tem erros
void	check_error_3(char *arg, char **input)
{
	if (!ft_isdigit(arg) || !check_size_int(arg))
		exit_errors(1, input);
	else
		do_exit(ft_atoi(arg), input);
}

// Verifica se o input é maior que um int64
int	check_size_int(char *str)
{
	char *max_int;
	int	i;

	i = 0;
	max_int = "9223372036854775807";
	while (str[i] && max_int[i])
	{
		if (str[i] > max_int[i])
			return (0);
		if (str[i] < max_int[i])
			return (1);
		i++;
	}
	return (1);
}

void	export_without_value(char *name)
{
	t_env_lst	*tmp;
	char		*value;

	tmp = com_info()->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
			return ;
		tmp = tmp->next;
	}
	value = ft_strdup("");
	lst_add_back(&com_info()->env_lst, new_node(name));
	free(value);
}

void	check_export_without_value(char *name)
{
	int	exist1;
	int	exist2;

	exist1 = check_if_exists(name, com_info()->env_lst);
	exist2 = check_if_exists(name, com_info()->vars);
	if (exist1 || exist2)
		return ;
	else
		export_without_value(name);
}

// Recria o comando export
void	*ft_export(char **input)
{
	int	i;

	i = 1;
	if (!input[i])
	{
		print_exported(input);
		return (0);
	}
	while (input[i])
	{
		if (ft_strchr(input[i], '='))
			check_export(input[i]);
		else
			check_export_without_value(input[i]);
		i++;
	}
	return (0);
}

// Compara o nome das variaveis
int	cmp_name(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i] && s1[i] > s2[i])
			return (1);
		else if (s1[i] != s2[i] && s1[i] < s2[i])
			return (0);
		i++;
	}
	return (0);
}

// Ordena a lista de variaveis
t_env_lst	*sort_list(t_env_lst *curr)
{
	char		*temp;
	char		*value;
	t_env_lst	*head;

	head = curr;
	while (curr->next)
	{
		if (cmp_name(curr->name, curr->next->name) == 1)
		{
			temp = ft_strdup(curr->name);
			value = ft_strdup(curr->value);
			curr->name = ft_strdup(curr->next->name);
			curr->value = ft_strdup(curr->next->value);
			curr->next->name = ft_strdup(temp);
			curr->next->value = ft_strdup(value);
			curr = head;
		}
		else
			curr = curr->next;
	}
	curr = head;
	return (curr);
}

// Imprime as variaveis exportadas
void	print_exported(char **input)
{
	t_env_lst	*temp;

	(void)input;
	temp = sort_list(com_info()->env_lst);
	while (temp)
	{
		if (ft_strncmp(temp->value, "", 1))
		{
			write(1, "declare -x ", 11);
			write(1, temp->name, ft_strlen(temp->name));
			write(1, "\"", 1);
			write(1, temp->value, ft_strlen(temp->value));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, "declare -x ", 11);
			write(1, temp->name, ft_strlen(temp->name));
			write(1, "\n", 1);
		}
		temp = temp->next;
	}
	com_info()->exit_value = 0;
}

// Verifica se a variável já existe na lista de variáveis de ambiente
// e na lista de variáveis locais

// Se existir na env_lst e na vars_lst, altera o valor da variável nas duas
// Se não existir na env_lst, mas existir na vars_lst,
// altera o valor da variável na vars_lst e adiciona a variável na env_lst
void	check_export(char *input)
{
	int	exist1;
	int	exist2;

	exist1 = check_if_exists(input, com_info()->env_lst);
	exist2 = check_if_exists(input, com_info()->vars);
	if (exist1)
	{
		if (exist2)
			change_value(input, com_info()->vars);
		change_value(input, com_info()->env_lst);
	}
	else if (!exist1)
	{
		if (exist2)
			change_value(input, com_info()->vars);
		lst_add_back(&com_info()->env_lst, new_node(input));
	}
}

// Verifica se a variável já existe na lista passada como argumento
int	check_if_exists(char *str, t_env_lst *lst)
{
	t_env_lst	*temp;
	char		*name;
	int			len;

	if (!str)
		return (0);
	len = 0;
	temp = lst;
	while (str[len] && str[len] != '=' && str[len] != '\0')
		len++;
	name = get_name_export(str, len);
	while (temp)
	{
		if (!ft_strncmp(name, temp->name, len))
		{
			free(name);
			return (1);
		}
		temp = temp->next;
	}
	free(name);
	return (0);
}

// Altera o valor da variável na lista passada como argumento
void	change_value(char *str, t_env_lst *lst)
{
	t_env_lst	*head;
	char		*name;
	char		*value;
	int			len;

	len = 0;
	head = lst;
	while (str[len] && str[len] != '=' && str[len] != '\0')
		len++;
	name = get_name_change_export(str, len);
	value = get_value_export(str, len + 1);
	while (lst->name != ft_substr(name, 0, len))
	{
		if (!ft_strncmp(ft_substr(name, 0, len), lst->name, (len)))
		{
			if (!ft_strchr(lst->name, '='))
				lst->name = ft_strdup(name);
			lst->value = ft_strdup(value);
			break ;
		}
		lst = lst->next;
	}
	lst = head;
	free(name);
	free(value);
}

// Retorna o nome da variavel
char	*get_name_export(char *str, int len)
{
	char	*name;
	int		i;

	i = 0;
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (NULL);
	while (i < len)
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

// Retorna o nome da variavel com o '='
char	*get_name_change_export(char *str, int len)
{
	char	*name;
	int		i;

	i = 0;
	name = malloc(sizeof(char) * (len + 2));
	if (!name)
		return (NULL);
	while (i < len && str[i])
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '=';
	i++;
	name[i] = '\0';
	return (name);
}

// Retorna o valor da variavel
char	*get_value_export(char *str, int len)
{
	char	*value;
	int		len_val;
	int		i;

	i = 0;
	len_val = (ft_strlen(str) - (len + 1));
	value = malloc(sizeof(char) * (len_val + 1));
	if (!value)
		return (NULL);
	while (str[len])
	{
		value[i] = str[len];
		i++;
		len++;
	}
	value[i] = '\0';
	return (value);
}

// Recria o comando pwd
void	ft_pwd(void)
{
	write(1, print_dir(), ft_strlen(print_dir()));
	write(1, "\n", 1);
	com_info()->exit_value = 0;
}

// Recria o comando unset
void	ft_unset(char **input)
{
	int			i;

	i = 1;
	if (!input[i])
		unset_error();
	while (input[i])
	{
		check_unset(input[i]);
		i++;
	}
	com_info()->exit_value = 0;
}

// Verifica se a variável existe na env_lst e na vars
// Executa o unset
void	check_unset(char *input)
{
	int	exist1;
	int	exist2;

	exist1 = check_if_exists_unset(input, com_info()->env_lst);
	exist2 = check_if_exists_unset(input, com_info()->vars);
	if (exist1)
		do_unset(input, com_info()->env_lst);
	if (exist2)
		do_unset(input, com_info()->vars);
}

// Verifica se a variável existe
int	check_if_exists_unset(char *input, t_env_lst *temp)
{
	while (temp)
	{
		if (!ft_strncmp(input, temp->name, ft_strlen(input)))
			return (1);
		temp = temp->next;
	}
	return (0);
}

// Executa o unset
void	do_unset(char *input, t_env_lst *lst)
{
	t_env_lst	*head;

	head = lst;
	while (lst)
	{
		if (!ft_strncmp(input, lst->name, (ft_strlen(lst->name) - 1))
			&& ft_strncmp(lst->name, "_=", 2))
		{
			free(lst->name);
			free(lst->value);
			if (!lst->next && !lst->prev)
				head = NULL;
			else if (!lst->next)
				lst->prev->next = NULL;
			else
			{
				lst->prev->next = lst->next;
				lst->next->prev = lst->prev;
			}
			break ;
		}
		lst = lst->next;
	}
	lst = head;
}

// Erros do comando unset
void	unset_error(void)
{
	ft_error("unset: not enough arguments\n");
	return ;
}

// Inicializa os pipes
void	init_pipes(void)
{
	int	i;

	i = 0;
	com_info()->pid_counter = 0;
	com_info()->cmds_done = 0;
	com_info()->pip = malloc(sizeof(int *) * (com_info()->pipe_no + 1));
	while (i < com_info()->pipe_no)
	{
		com_info()->pip[i] = malloc(sizeof(int) * 2);
		if (pipe(com_info()->pip[i]) < 0)
		{
			ft_error("Pipe error\n");
			return ;
		}
		i++;
	}
	//com_info()->pip[i] = NULL;
	com_info()->pid = malloc(sizeof(int) * (com_info()->pipe_no + 1));
	com_info()->pid[com_info()->pipe_no] = 0;
}

// Executa os pipes
void	execute_pipe(char **input)
{
	signal_block();
	com_info()->pid[com_info()->pid_counter] = fork();
	if (com_info()->pid[com_info()->pid_counter] == 0)
	{
		fd_dup(com_info()->cmds_done);
		commands(input, com_info()->env, 1);
	}
	com_info()->pid_counter++;
	fd_close(com_info()->cmds_done);
	unlink(".heredoc");
}

// Espera por todos os processos
void	ft_wait_pid(void)
{
	int	i;

	i = 0;
	signal_block();
	printf("com_info()->pid_counter: %d\n", com_info()->pid_counter);
	while (i < (com_info()->pid_counter))
	{
		waitpid(com_info()->pid[i], &com_info()->exit_value, 0);
		i++;
	}
	catch_signal();
}

// Duplica os file descriptors
void	fd_dup(int pos)
{
	if (pos == 0)
	{
		dup2(com_info()->pip[0][1], STDOUT_FILENO);
	}
	else if (pos == com_info()->pipe_no)
	{
		dup2(com_info()->pip[pos - 1][0], STDIN_FILENO);
	}
	else
	{
		dup2(com_info()->pip[pos - 1][0], STDIN_FILENO);
		dup2(com_info()->pip[pos][1], STDOUT_FILENO);
	}
}

// Fecha os file descriptors
void	fd_close(int pos)
{
	if (pos == 0)
	{
		if (close(com_info()->pip[0][1]) == -1)
			ft_error("Close error 1\n");
	}
	else if (pos == com_info()->pipe_no)
	{
		if (close(com_info()->pip[pos - 1][0]) == -1)
			ft_error("Close error 2\n");
	}
	else
	{
		if (close(com_info()->pip[pos][1]) == -1)
			ft_error("Close error 3\n");
		if (close(com_info()->pip[pos - 1][0]) == -1)
			ft_error("Close error 4\n");
	}
}

// Recria a função strichr
int	ft_strichr(const char *s, int start, int c)
{
	while (s[start] != (char )c && s[start] != '\0')
		start++;
	if (s[start] == (char )c)
		return (start);
	else
		return (-1);
}

// Recria a função strncmp
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (str1[i] && str2[i] && i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	if (i < n)
		return (str1[i] - str2[i]);
	return (0);
}

// Retorna o path do comando
char	*join_strings(char *path, int j, char *cmd)
{
	char	*dir;
	char	*ret_path;

	dir = ft_substring(path, j, ft_strichr(path, j, ':') - j);
	ret_path = ft_strjoin(dir, cmd);
	free(dir);
	return (ret_path);
}

// Retorna o path do comando
char	*ft_substring(char const *s, unsigned int start, size_t len)
{
	char			*sub;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	if (!len || (unsigned int)ft_strlen(s) < start)
		return (ft_strdup(""));
	if (len > (size_t)ft_strlen(s))
	{
		len = ft_strlen(s);
		return (ft_substr(s, start, len));
	}
	sub = malloc((len + 2) * sizeof(char));
	if (!sub)
		return (NULL);
	while (s[start + i] != '\0' && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '/';
	i++;
	sub[i] = '\0';
	return (sub);
}

// Conta o numero de pipes
int	count_pipes(char *input)
{
	int	i;
	int	pipe_no;

	i = 0;
	pipe_no = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = skip_quotes(input, i, input[i]);
		else if (input[i] == '|')
			pipe_no++;
		i++;
	}
	return (pipe_no);
}

void	pipe_cleanup(void)
{
	int	i;

	i = 0;
	while (i < com_info()->pipe_no)
	{
		close(com_info()->pip[i][0]);
		close(com_info()->pip[i][1]);
		i++;
	}
	free(com_info()->pip);
}

// Processa o $.
// Se for $? retorna o exit value.
// Se input[i][0] for $ e tiver mais que 1 char e tiver apenas 1 $,
// altera o valor com a funcao change_val.
// Se tiver 1 ou mais $ altera o valor com a funcao change_val2.

// >> 8 & 255
char	**check_ds(char **input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_strncmp(input[i], "$?", 3))
			input[i] = ft_itoa((com_info()->exit_value >> 8 & 255));
		else if (input[i][0] == '$' && ft_strlen(input[i]) > 1
			&& count_ds(input[i]) == 1)
			input[i] = ft_strdup(change_val(input[i]));
		else if (count_ds(input[i]) >= 1 && (ft_strlen(input[i]) > 1))
			input[i] = change_val2(input[i], 0, 0);
		i++;
	}
	return (input);
}

// Altera o valor da variavel quando tiver apenas 1 $.
char	*change_val(char *input)
{
	t_env_lst	*temp;
	t_env_lst	*temp2;
	char		*name;

	name = input;
	temp = com_info()->vars;
	name++;
	while (temp)
	{
		if (!ft_strncmp(ft_strjoin(name, "="), temp->name, ft_strlen(name) + 1))
			return (temp->value);
		temp = temp->next;
	}
	temp2 = com_info()->env_lst;
	while (temp2)
	{
		if (!ft_strncmp(ft_strjoin(name, "=")
				, temp2->name, ft_strlen(name) + 1))
			return (temp2->value);
		temp2 = temp2->next;
	}
	return ("");
}

// Salta as aspas simples.
int	skip_quotes_ds(char *input, int i, char c)
{
	i++;
	while (input[i] && input[i] != c)
		i++;
	return (i);
}

// Altera o valor da variavel quando tiver um ou mais $.
// Se tiver só 1 $, é porque está no meio da string.
char	*change_val2(char *input, int i, int j)
{
	char		*name;

	while (input[i])
	{
		if (input[i] == '\'')
			i = skip_quotes_ds(input, i, '\'');
		if (input[i] == '$')
		{
			while (is_valid(input[++i]))
				j++;
			name = ft_substr(input, (i - j - 1), (j + 1));
			if (cds(name, com_info()->env_lst) || cds(name, com_info()->vars))
				input = create_new(input, i, j, name);
			else
				input = remove_ds(input, ft_strlen(name));
			i = 0;
			j = 0;
		}
		else
			i++;
	}
	return (input);
}

// Conta o numero de $ na string
int	count_ds(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		i++;
	}
	return (count);
}

int	is_valid(char c)
{
	if (c == ' ' || c == '\n' || c == '\v' || c == '\0'
		|| c == '\r' || c == '\t' || c == '\f' || c == '$'
		|| c == '\'')
		return (0);
	return (1);
}

char	*create_new(char *input, int i, int j, char *name)
{
	char	*new;
	char	*val;
	int		k;
	int		l;
	int		size;

	k = 0;
	l = 0;
	val = ft_strdup(change_val(name));
	size = ((ft_strlen(input) - j) + ft_strlen(val));
	new = malloc(sizeof(char) * size);
	if (!new)
		return (NULL);
	while (k < (i - j - 1))
	{
		new[k] = input[k];
		k++;
	}
	while (val[l])
		new[k++] = val[l++];
	while (new[k] || k < size)
		new[k++] = input[i++];
	new[k] = '\0';
	return (new);
}

// Remove $
char	*remove_ds(char *input, int size)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(input) - size));
	if (!new)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$' && i < size)
		{
			i++;
			while (is_valid(input[i]) && input[i])
				i++;
			while (input[i])
				new[j++] = input[i++];
		}
		else
			new[j++] = input[i++];
	}
	new[j] = '\0';
	return (new);
}

// Check If Dollar Sign Exists
int	cds(char *input, t_env_lst *temp)
{
	input++;
	while (temp)
	{
		if (!ft_strncmp(ft_strjoin(input, "="), temp->name, ft_strlen(input)))
			return (1);
		temp = temp->next;
	}
	return (0);
}

// Processa aspas duplas
char	**process_quotes(char **input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (find_quote(input[i]) % 2 == 0 && find_quote(input[i]) != 0)
			input[i] = remove_quotes(input[i]);
		i++;
	}
	return (input);
}

// Processa aspas simples
char	**process_peliculas(char **input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (find_pelicula(input[i]) % 2 == 0 && find_pelicula(input[i]) != 0)
			input[i] = remove_peliculas(input[i]);
		i++;
	}
	return (input);
}

char	*remove_quotes(char *input)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(input) - 2));
	if (!new)
		return (NULL);
	while (input[i])
	{
		if (input[i] != '"')
		{
			new[j] = input[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	return (new);
}

char	*remove_peliculas(char *input)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(input) - 2));
	if (!new)
		return (NULL);
	while (input[i])
	{
		if (input[i] != '\'')
		{
			new[j] = input[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	return (new);
}

int	find_quote(char *str)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '"')
			quote++;
		i++;
	}
	return (quote);
}

int	find_pelicula(char *str)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			quote++;
		i++;
	}
	return (quote);
}

void	do_heredoc(char ***mat_array, int i)
{
	pid_t	pid;

	(void)i;
	pid = fork();
	if (pid == 0)
	{
		com_info()->hereflag = 1;
		heredoc(mat_array[1][0]);
		// if (mat_array[i + 2])
		// {
		// 	printf("mat_array[%d] = %s\n", i, mat_array[i][0]);
		// 	commands(mat_array[i + 2], com_info()->env, 1);
		// }
	}
	else
		waitpid(pid, &com_info()->exit_value, 0);
	//unlink(".heredoc");
}

int	heredoc(char *limiter)
{
	int		fd;
	char	*tmp;

	fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		perror("minishell: error opening heredoc file");
	while (1)
	{
		tmp = readline("> ");
		if (ft_strncmp(tmp, limiter, ft_strlen(limiter)) == 0)
			break ;
		ft_putstr_fd(tmp, fd);
		free(tmp);
	}
	free(tmp);
	close(fd);
	fd = open(".heredoc", O_RDONLY);
	if (fd < 0)
	{
		unlink(".heredoc");
		perror("minishell: error opening heredoc file");
	}
	return (fd);
}

void	do_redir(char **before, char **after)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		//if (com_info()->hereflag == 0)
		//{
			redirections(after, com_info()->redir_type);
			if (com_info()->hereflag == 0)
				commands(before, com_info()->env, 1);
			else
				com_info()->hereflag = 0;
			exit(0);
		//}
		// else
		// {
		// 	redirections(after, com_info()->redir_type);
		// 	com_info()->hereflag = 0;
		// }
	}
	else
		waitpid(pid, &com_info()->exit_value, 0);
	//unlink(".heredoc");
}

void	options(char ***new, int i)
{
	if (check_redir_type(new[i][0]) <= 2)
		close(open(new[i + 1][0], O_CREAT|O_WRONLY, 0644));
	else if (check_redir_type(new[i][0]) == 4)
	{
		if (check_file_access(new[i + 1][0]))
			return ;
		else
		{	
			com_info()->redir_type = check_redir_type(new[i][0]);
			do_redir(new[0], new[i + 1]);
		}
	}
	else
	{
		com_info()->hereflag = 1;
		do_heredoc(new, i + 1);
	}
}

// Executa as redireções.
// Se i for impar, é o tipo de redireção.
// Se i for par, é o arquivo.
void	execute_redir(char **input)
{
	char	***new;
	int		i;

	if (input[0][0] == '<' && input[0][1] == '<')
		i = 0;
	else
		i = 1;
	new = split_redir(input);
	if (com_info()->redir_no > 1)
	{
		while (i < com_info()->redir_no * 2 - 2)
		{
			options(new, i + com_info()->hereflag);
			i += 2;
		}
	}
	com_info()->redir_type = check_redir_type(new[i][0]);
	if (com_info()->redir_type != 3)
	{
		
		do_redir(new[0 + com_info()->hereflag], new[i + 1 + com_info()->hereflag]);
	}
	else
	{
		do_heredoc(new, i);
		com_info()->hereflag = 1;
	}
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

// Faz as redireções e as duplicações de file descriptors
// O que se faz quando tem vários argumentos a seguir ao redirecionador?
void	redirections(char **arquivo, int type)
{
	int		fd;
	char	*file;

	file = arquivo[0];
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

// Função para fazer o split dos redirecionamentos
// Faz a primeira alocação de memória para a matriz
char	***split_redir(char **input)
{
	char	***new;
	int		matlen;

	if (!input)
		return (NULL);
	matlen = ft_matmeasures(input);
	new = (char ***)malloc(sizeof(char ***) * (matlen + 1));
	if (!new)
		return (NULL);
	split_all(input, new, matlen);
	new[matlen] = NULL;
	return (new);
}

// Faz a segunda alocação de memória para a matriz
int	split_all(char **input, char ***new, int matlen)
{
	int		i;
	int		j;
	int		nb_words;

	i = 0;
	j = 0;
	while (j < matlen)
	{
		nb_words = count_second_word(input, i);
		new[j] = malloc(sizeof(char **) * (nb_words + 1));
		if (!new[j])
			return (0);
		fill_word(input, i, nb_words, new[j]);
		i += nb_words;
		j++;
	}
	new[matlen] = NULL;
	return (0);
}

// Preenche a matriz com as palavras
void	fill_word(char **input, int i, int nb_words, char **new)
{
	int	j;

	j = 0;
	while (j < nb_words)
	{
		new[j] = ft_strdup(input[i]);
		if (!new[j])
			return ;
		j++;
		i++;
	}
	new[j] = NULL;
}

// Conta o número de char** que serão alocadas na matriz
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
				if (input[i][j + 1] == input[i][j])
					j++;
				count += 2;
			}
			j++;
		}
		i++;
	}
	return (count);
}

// Conta o número de palavras que serão alocadas na matriz
int	count_second_word(char **input, int i)
{
	int	count;

	count = 0;
	if (input[i] && (input[i][0] == '>' || input[i][0] == '<'))
		return (1);
	while (input[i] && input[i][0] != '>' && input[i][0] != '<')
	{
		count++;
		i++;
	}
	return (count);
}

// Verifica se o char está contido no set
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

// Retorna o tamanho da string até o próximo redirect
int	get_size(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '>' && input[i] != '<')
		i++;
	return (i);
}

// Imprime a matriz
void	print_matrix_redir(char ***matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (matrix[i])
	{
		j = 0;
		while (matrix[i][j])
		{
			printf("matrix[%i][%i]: %s\n", i, j, matrix[i][j]);
			j++;
		}
		i++;
	}
}

int	verify_redir_2(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && input[i + 1] == '>' && (input[i + 2] == '>'
				|| input[i + 2] == '<' || input[i + 2] == '|'))
		{
			ft_error("minishell: syntax error near\
				unexpected token `newline'\n");
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

t_env_lst	*ft_lstlast(t_env_lst *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	lst_add_back(t_env_lst **lst, t_env_lst *new)
{
	t_env_lst	*last;

	if (!lst)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
	new->prev = last;
}

t_env_lst	*new_node(char *env)
{
	t_env_lst	*env_node;

	env_node = malloc(sizeof(t_env_lst));
	if (!env_node)
		return (NULL);
	env_node->name = ft_substr(env, 0, ft_strchr(env, '=') + 1);
	env_node->value = ft_substr(env, ft_strchr(env, '=') + 1, ft_strlen(env));
	env_node->next = NULL;
	env_node->prev = NULL;
	return (env_node);
}

t_env_lst	*env_to_lst(char **env)
{
	int			i;
	t_env_lst	*env_lst;

	i = 0;
	env_lst = NULL;
	while (env[i])
	{
		lst_add_back(&env_lst, new_node(env[i]));
		i++;
	}
	return (env_lst);
}

void	free_env(t_env_lst **env)
{
	while ((*env)->next)
	{
		free((*env)->name);
		free((*env)->value);
		(*env)->name = NULL;
		(*env)->value = NULL;
		*env = (*env)->next;
	}
}

// Função para criar o prompt
char	*print_info(void)
{
	char	cwd[1024];
	char	*str;
	char	*username;
	char	*dir;

	dir = getcwd(cwd, sizeof(cwd));
	username = get_cenas_do_env("USER=");
	str = ft_strjoin(username, com_info()->color);
	str = ft_strjoin(str, dir);
	str = ft_strjoin(str, "$ \033[0m");
	return (str);
}

// Função para imprimir o diretório atual
char	*print_dir(void)
{
	char	cwd[1024];

	return (getcwd(cwd, sizeof(cwd)));
}

// Se o primeiro caracter for igual a '=':
// Erro

// Enquanto input[i] for diferente de NULL:
// Se input[i] tiver um '=':
// Chama a função exported_vars_utils
// Se não:
// Sai do ciclo
void	exported_vars(char **input)
{
	int	i;

	i = 0;
	if (input[0][i] == '=')
	{
		ft_error("%s not found", input[1]);
		return ;
	}
	while (input[i])
	{
		if (ft_strchr(input[i], '='))
			exported_vars_utils(input[i]);
		else
			break ;
		i++;
	}
	return ;
}

// Se já existir a variável no exported, mas não existir na lista de variáveis:
// Altera o valor no exported e adiciona à lista de variáveis

// Se já existir a variável na lista de variáveis:
// Altera o valor na lista de variáveis

// Se não existir a variável em lado nenhum:
// Adiciona à lista de variáveis
void	exported_vars_utils(char *input)
{
	if (check_if_exists(input, com_info()->env_lst)
		&& !check_if_exists(input, com_info()->vars))
	{
		change_value(input, com_info()->env_lst);
		lst_add_back(&com_info()->vars, new_node(input));
	}
	else if (check_if_exists(input, com_info()->vars))
		change_value(input, com_info()->vars);
	else
		lst_add_back(&com_info()->vars, new_node(input));
}

// Find Equal Sign
int	find_es(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

// Conta os argumentos passados
// Atribui o valor no com_info()->nb_args
int	count_args(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		i++;
	com_info()->nb_args = i;
	return (i);
}

// Função que retorna o nome da variável
char	*get_name(char *str)
{
	int		len;
	int		i;
	char	*name;

	len = 0;
	i = 0;
	while (str[len] != '=')
		len++;
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (0);
	while (i < len)
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

int	find_quotes(const char *str, int i, int type)
{
	int	st;

	st = i;
	i++;
	if (type == 34)
	{
		while (str[i] && str[i] != 34)
			i++;
		if (str[i] == 34)
			return (i);
	}
	else
	{
		while (str[i] && str[i] != 39)
			i++;
		if (str[i] == 39)
			return (i);
	}
	return (st);
}

int	ft_wordcount(const char *str, char c)
{
	int		words;
	size_t	i;

	i = 0;
	words = 0;
	while (str[i])
	{
		if (str[i] == 34)
			i = find_quotes(str, i, 34);
		if (str[i] == 39)
			i = find_quotes(str, i, 39);
		if (ft_ispipe(str[i], c) == 0 && ft_ispipe(str[i + 1], c) == 1)
			words++;
		i++;
	}
	return (words);
}

// Norminette
static int	split_it(char const *str, char c, int st, char **ns)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] != '\0' && str[i] == c)
		{
			i++;
			st++;
		}
		if (str[i] == 34)
			i = find_quotes(str, i, 34);
		if (str[i] == 39)
			i = find_quotes(str, i, 39);
		if (str[i] == '\0')
			break ;
		if (ft_ispipe(str[i], c) == 0 && ft_ispipe(str[i + 1], c) == 1)
		{
			while (str[st] && (ft_space(str[st]) || ft_ispipe(str[st], c)))
				st++;
			ns[j] = ft_substr(str, st, (i - st) + 1);
			if (!ns[j++])
				return (0);
			st = i + 1;
		}
		i++;
	}
	ns[j] = NULL;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char	**ns;
	size_t	st;
	int		matlen;

	st = 0;
	if (!s)
		return (NULL);
	matlen = ft_wordcount(s, c);
	ns = malloc(sizeof(char *) * (matlen + 1));
	if (!ns)
		return (NULL);
	if (!split_it(s, c, st, ns))
		return (freematrix(ns, matlen));
	return (ns);
}

void	*freematrix(char **ns, int msize)
{
	while (msize--)
		free(ns[msize]);
	free(ns);
	return (NULL);
}

int	ft_space(char s)
{
	if (s == '\n' || s == '\t' || s == '\v'
		|| s == '\f' || s == '\r' || s == ' ')
		return (1);
	else
		return (0);
}

int	ft_ispipe(char s, char c)
{
	if (c == s || s == '\0')
		return (1);
	else
		return (0);
}

t_args	*add_mat_node(char *args)
{
	t_args	*new_node;

	new_node = malloc(sizeof(t_args));
	if (!new_node)
		return (NULL);
	new_node->arg = ft_split(args, ' ');
	new_node->next = NULL;
	return (new_node);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	val;
	int	sinal;

	i = 0;
	val = 0;
	sinal = 1;
	while ((str[i] == '\n') || (str[i] == '\r') || (str[i] == '\t')
		|| (str[i] == '\v') || (str[i] == '\f') || (str[i] == ' '))
		i++;
	if ((str[i] == '-') || (str[i] == '+'))
	{
		if (str[i] == '-')
			sinal *= -1;
		i++;
	}
	while ((str[i] >= 48) && (str[i] <= 57))
	{	
		val *= 10;
		val += str[i] - 48;
		i++;
	}
	return (sinal * val);
}

char	*check(char c, va_list arg)
{
	if (c == 'c')
		return (ft_putchar_fde(va_arg(arg, int)));
	else if (c == 's')
		return (ft_putstr_fde(va_arg(arg, char *)));
	else
		return (NULL);
}

char	*ft_main(va_list arg, char *c)
{
	char	*error;
	char	*begin;
	char	*middle;
	char	*end;
	int		i;
	
	i = 0;
	error = NULL;
	while (c[i])
	{
		if (c[i] == '%')
		{
			begin = ft_substr(c, 0, i);
			end = ft_substr(c, i + 2, ft_strlen(c));
			i++;
			if (ft_strchr("cs", c[i]))
			{
				middle = check(c[i], arg);
				error = ft_strjoin(begin, middle);
				error = ft_strjoin(error, end);
			}
			else
				return (error);
		};
		i++;
	}
	return (error);
}

void	ft_error(char *err, ...)
{
	va_list	arg;
	char	*error;

	if (!err)
		return ;
	va_start(arg, err);
	error = ft_main(arg, (char *)err);
	va_end(arg);
	error = ft_strjoin("\033[0;31m", error),
	error = ft_strjoin(error, "\033[0m");
	perror(error);
	//free(error);
}

char	*ft_putchar_fde(char c)
{
	char	*str;

	str = malloc(sizeof(char) * 2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

char	*ft_putstr_fde(char *s)
{
	char	*str;
	int		i;

	i = 0;
	str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

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

int	size_of_number(long nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		nb = nb * -1;
		len++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int number)
{
	char	*str;
	long	nb;
	int		len;

	nb = number;
	len = size_of_number(nb);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (nb == 0)
		str[0] = 48;
	if (nb < 0)
	{
		str[0] = '-';
		nb = nb * -1;
	}
	while (nb > 0)
	{
		str[len] = 48 + (nb % 10);
		nb = nb / 10;
		len--;
	}
	return (str);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*dr;
	const char	*sr;
	size_t		i;

	sr = src;
	dr = dst;
	i = 0;
	if (!dst && !src)
		return (NULL);
	if (dr < sr)
	{
		while (i < len)
		{
			dr[i] = sr[i];
			i++;
		}
	}
	else
	{
		while (len--)
			dr[len] = sr[len];
	}
	return (dst);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			write(fd, "-", 1);
			n = -n;
		}
		if (n >= 10)
		{
			ft_putnbr_fd((n / 10), fd);
			ft_putnbr_fd((n % 10), fd);
		}
		else
			ft_putchar_fd((n + '0'), fd);
	}	
}

void	ft_putstr_fd(char *s, int fd)
{
	unsigned int	i;

	i = 0;
	if (!s || !fd)
		return ;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char )c && s[i] != '\0')
		i++;
	if (s[i] == (char )c)
		return (i);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strdup(const char *s1)
{
	char	*p;

	p = malloc(ft_strlen(s1) + 1);
	if (!p)
		return (NULL);
	ft_memmove(p, s1, ft_strlen(s1) + 1);
	return (p);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc(sizeof(char) * ((ft_strlen(s1) + ft_strlen(s2) + 1)));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
	{
		str[i++] = s1[j++];
	}
	j = 0;
	while (s2[j])
	{
		str[i++] = s2[j++];
	}
	str[i] = '\0';
	return (str);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return ((int)i);
}

char	*ft_strljoin(char const *s1, char const *s2, unsigned int len)
{
	char			*ns;
	unsigned int	i;
	unsigned int	j;

	if (!s2)
		return (NULL);
	if (!s1)
		return (ft_substr(s2, 0, len));
	ns = malloc((ft_strlen(s1) + len) * sizeof(char) + 1);
	if (!ns)
		return (NULL);
	i = -1;
	while (s1[++i])
		ns[i] = s1[i];
	j = -1;
	while (s2[++j] && j < len)
		ns[i + j] = s2[j];
	ns[i + j] = '\0';
	return (ns);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*sub;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	if (!len || ((unsigned int)ft_strlen(s) < start))
		return (ft_strdup(""));
	if (len > (size_t)ft_strlen(s))
	{
		len = ft_strlen(s);
		return (ft_substr(s, start, len));
	}
	sub = malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	while (s[start + i] != '\0' && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

static char	*add_leftover(char **storage)
{
	char	*return_of_the_string;
	int		i;
	char	*intern;

	i = 0;
	while ((*storage)[i] != '\n' && (*storage)[i] != '\0')
		i++;
	return_of_the_string = ft_substr(*storage, 0, i + 1);
	intern = NULL;
	if ((*storage)[i] == '\n' && (*storage)[i + 1] != '\0')
	{
		intern = ft_substr(*storage, i + 1, ft_strlen(*storage) - i);
		free(*storage);
		*storage = ft_strdup(intern);
		free(intern);
		intern = NULL;
	}
	else
	{
		free(*storage);
		*storage = NULL;
	}
	return (return_of_the_string);
}

static char	*update_stored(char **storage, char *string)
{
	int		i;
	char	*return_of_the_string;
	char	*intern;

	i = 0;
	while ((*storage)[i] != '\n' && (*storage)[i] != '\0')
		i++;
	intern = ft_strljoin(string, *storage, i + 1);
	free(string);
	return_of_the_string = ft_strdup(intern);
	free(intern);
	if ((*storage)[i] == '\n' && (*storage)[i + 1] != '\0')
	{
		intern = ft_substr(*storage, i + 1, ft_strlen(*storage) - i);
		free(*storage);
		*storage = ft_strdup(intern);
		free(intern);
		intern = NULL;
	}
	else
	{
		free(*storage);
		*storage = NULL;
	}
	return (return_of_the_string);
}

static char	*reading(int fd, char **storage)
{
	char		*temp;
	char		buffer[BUFFER_SIZE + 1];
	int			chomp;

	chomp = read(fd, buffer, BUFFER_SIZE);
	if (chomp <= 0)
	{
		if (*storage != NULL)
			free(*storage);
		*storage = NULL;
		return (NULL);
	}
	buffer[chomp] = '\0';
	*storage = ft_strdup(buffer);
	while (chomp > 0 && !ft_strchr(buffer, '\n'))
	{
		chomp = read(fd, buffer, BUFFER_SIZE);
		buffer[chomp] = '\0';
		temp = ft_strljoin(*storage, buffer, ft_strlen(buffer));
		free(*storage);
		*storage = ft_strdup(temp);
		free(temp);
	}
	return (*storage);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*string;

	if (fd < 0 || fd > 1024 || BUFFER_SIZE < 1)
		return (NULL);
	string = NULL;
	if (storage)
	{
		string = add_leftover(&storage);
		if (ft_strchr(string, '\n'))
			return (string);
	}
	storage = reading(fd, &storage);
	if (!storage && !string)
		return (NULL);
	if (storage)
		string = update_stored(&storage, string);
	return (string);
}

void	lst_add_front(t_args **lst, t_args *new)
{
	if (*lst)
		new->next = *lst;
	*lst = new;
}

int	strict_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}
