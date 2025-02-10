/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 04:21:04 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/10 16:43:28 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include "../LIBFT_V2/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

typedef struct t_pipe
{
	int			pipefd[2];
	int			child_count;
	int			infile;
	int			outfile;
	pid_t		*wait_pid;
}				t_pipe;

void	here_doc_exec(char **argv, int pipe_fd[2]);
t_pipe	handle_here_doc(char **argv, int argc);
void	absolute_path(char **cmd, char **env, t_pipe pipex);
pid_t	child(char **argv, t_pipe pipex, char **env);
pid_t	parent(int argc, char **argv, t_pipe pipex, char **env);
char	*get_env(char **env, char **cmd);
char	**split_env_path(char **cmd, char **env);
void	exec_cmd(char **cmd, char **env, t_pipe pipex);
int		open_file(char *argv, int check, t_pipe pipex);
void	open_error(void);
void	handle_pipe(t_pipe pipex, int argc, char **argv, char **env);
void	check_first_arg(char *argv, char **env);

#endif
