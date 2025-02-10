/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 02:07:10 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/10 16:44:54 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// check absolute path for the first arg and also if the arg is NULL
static int	absolute_path_first_arg(char **cmd)
{
	if (cmd[0] == NULL)
	{
		free_tab(cmd);
		ft_putstr_fd("command not found\n", 2);
		exit(EXIT_FAILURE);
	}
	if (access(cmd[0], X_OK) == 0)
	{
		free_tab(cmd);
		return (0);
	}
	return (1);
}

// check if the first arg is a valid command for the here_doc
void	check_first_arg(char *argv, char **env)
{
	char	**cmd;
	char	**cmd_files;
	int		i;

	i = 0;
	cmd = ft_split(argv, ' ');
	if (absolute_path_first_arg(cmd) == 0)
		return ;
	cmd_files = split_env_path(cmd, env);
	while (cmd_files[i] != NULL)
	{
		if (access(cmd_files[i], X_OK) == 0)
		{
			free_tab(cmd);
			free_tab(cmd_files);
			return ;
		}
		i++;
	}
	free_tab(cmd);
	free_tab(cmd_files);
	ft_putstr_fd("command not found\n", 2);
	exit(EXIT_FAILURE);
}

t_pipe	handle_here_doc(char **argv, int argc)
{
	int		pipe_fd[2];
	pid_t	pid;
	t_pipe	pipex;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	pipex.child_count = 3;
	if (pid == 0)
		here_doc_exec(argv, pipe_fd);
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		wait(NULL);
	}
	pipex.infile = -1;
	pipex.outfile = open_file(argv[argc - 1], 3, pipex);
	return (pipex);
}

// here_doc loop that play until the LIMITER is typed in the terminal
void	here_doc_exec(char **argv, int pipe_fd[2])
{
	char	*str;
	char	*limiter;

	close(pipe_fd[0]);
	limiter = ft_strjoin(argv[2], "\n");
	while (1)
	{
		str = get_next_line(0);
		if (str == NULL)
			break ;
		if (ft_strncmp(str, limiter, ft_strlen(str)) == 0)
		{
			free(str);
			free(limiter);
			get_next_line(-1);
			close(pipe_fd[1]);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(str, pipe_fd[1]);
		free(str);
	}
	return ;
}
