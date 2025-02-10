/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 06:02:41 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/10 16:47:09 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env(char **env, char **cmd)
{
	int		i;
	int		j;
	char	*env_path;

	i = 0;
	j = 0;
	while (env[i] != NULL)
	{
		j = ft_strncmp(env[i], "PATH=", 5);
		if (j == 0)
		{
			env_path = ft_strdup(env[i] + 5);
			return (env_path);
		}
		i++;
	}
	free_tab(cmd);
	exit(EXIT_FAILURE);
}

char	**split_env_path(char **cmd, char **env)
{
	char	*path;
	char	**cmd_files;
	char	**files;
	char	*cmd_join;
	int		i;

	i = 0;
	path = get_env(env, cmd);
	files = ft_split(path, ':');
	cmd_files = malloc(sizeof(char *) * (tab_size(files) + 1));
	if (!cmd_files)
		exit(EXIT_FAILURE);
	cmd_join = ft_strjoin("/", cmd[0]);
	while (files[i] != NULL)
	{
		cmd_files[i] = ft_strjoin(files[i], cmd_join);
		i++;
	}
	cmd_files[i] = NULL;
	free(path);
	free(cmd_join);
	free_tab(files);
	return (cmd_files);
}

pid_t	child(char **argv, int pipefd[2], char **env)
{
	int		fd;
	char	**cmd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", 2);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		fd = open_file(argv[1], 2, pipefd);
		dup2(fd, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		close(fd);
		cmd = ft_split(argv[2], ' ');
		absolute_path(cmd, env);
		exec_cmd(cmd, env);
	}
	close(pipefd[1]);
	return (pid);
}

pid_t	parent(char **argv, int pipefd[2], char **env)
{
	int		fd;
	char	**cmd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", 2);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		fd = open_file(argv[4], 1, pipefd);
		dup2(fd, STDOUT_FILENO);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		close(fd);
		cmd = ft_split(argv[3], ' ');
		absolute_path(cmd, env);
		exec_cmd(cmd, env);
	}
	close(pipefd[0]);
	return (pid);
}

int	main(int argc, char **argv, char **env)
{
	int		pipefd[2];
	pid_t	wait_pid[2];

	if (argc != 5)
	{
		ft_putstr_fd("Wrong format :file1 cmd1 cmd2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("pipe error\n", 2);
		exit(EXIT_FAILURE);
	}
	wait_pid[0] = child(argv, pipefd, env);
	wait_pid[1] = parent(argv, pipefd, env);
	waitpid(wait_pid[0], NULL, 0);
	waitpid(wait_pid[1], NULL, 0);
	return (0);
}
