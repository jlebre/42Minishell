/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlebre <jlebre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 15:59:44 by jlebre            #+#    #+#             */
/*   Updated: 2022/10/25 15:59:44 by jlebre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdarg.h>
/////////////////////////////////////////////////
# include <signal.h>
/////////////////////////////////////////////////
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
/////////////////////////////////////////////////
# include <dirent.h>
/////////////////////////////////////////////////
# include <readline/readline.h>
# include <readline/history.h>
///////////////////////////////////////////////// PIPEX
# include <fcntl.h>
# include <errno.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_env_lst
{
	char				*name;
	char				*value;
	struct s_env_lst	*next;
	struct s_env_lst	*prev;
}	t_env_lst;

typedef struct s_command
{
	int					fd[2];
	pid_t				pid;
	int					status;
	char				*cmd;
	char 				*path;
	char				**args;
	int					exit_value;
	t_env_lst			*env_lst;
	t_env_lst			*vars;
	int					nb_args;
	char				*color;
}   t_command;

//COMMAND INFO
t_command				*com_info(void);

//INIT SHELL
void					init_shell(char **env);
void					recieve(int sig);

//ENV_TO_LIST
t_env_lst				*env_to_lst(char **env);
t_env_lst				*new_node(char *env);
t_env_lst				*ft_lstlast(t_env_lst *lst);
void					lst_add_back(t_env_lst **lst, t_env_lst *new);
char    				**lst_to_env();
void					change_value(char *str);
void					change_color(char **input);

//FREE ENV
void					free_env(t_env_lst **env);

//PRINT DIR
char					*print_info(void);
char					*print_dir(void);
//PROCESS INPUT
void					process_input(char *input, char **env);
int						count_args(char **matrix);

//COMMANDS
void					commands(char **input, char **env);
void					env_commands(char **input, char **env);
char					*find_path(char *cmd, t_env_lst *env_lst);

//ECHO
void					ft_echo(char **input);

//CD
void					ft_cd(char **input, char **env);

//ENV
void					ft_env(char **input);
void					ft_unset(char **input);
void					*ft_export(char **input);
void					print_exported(char **input);
int						check_if_exists(char *str);
void					change_value(char *str);
void					ft_pwd(void);
void					ft_exit(char **input);
int						ft_isdigit(char *str);

//FT_ERROR
void					ft_error(char *err);

//SHELL_SPLIT_UTILS
int						find_quotes(const char *str, int i, int type);
void					*freematrix(char **ns, int msize);

//UTILS_PIPE
int						ft_strichr(const char *s, int start, int c);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
char					*join_strings(char *path, int j, char *cmd);
char					*ft_substring(char const *s, unsigned int start, size_t len);

//UTILS
char					*ft_strjoin(char const *s1, char const *s2);
int						ft_strlen(const char *str);
char					*ft_substr(char const *s, unsigned int start, size_t len);
char					*ft_strdup(const char *s1);
void					*ft_memmove(void *dst, const void *src, size_t len);

//UTILS2
char					**ft_split(const char *s, char c);
int						ft_strcmp(char *s1, char *s2);
int 					strict_cmp(char *s1, char *s2);
void					ft_clear(void);
int						ft_strchr(const char *s, int c);
int						ft_atoi(const char *str);

//NORMAL COLORS
int						black(char *str);
int						red(char *str);
int						green(char *str);
int						yellow(char *str);
int						blue(char *str);
int						purple(char *str);
int						cyan(char *str);
int						white(char *str);

//BOLD
int						bold_black(char *str);
int						bold_red(char *str);
int						bold_green(char *str);
int						bold_yellow(char *str);
int						bold_blue(char *str);
int						bold_purple(char *str);
int						bold_cyan(char *str);
int						bold_white(char *str);

#endif