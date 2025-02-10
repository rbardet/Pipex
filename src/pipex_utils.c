/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:43:39 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/10 16:47:37 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	open_error(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
	ft_putstr_fd("error with open\n", 2);
	exit(EXIT_FAILURE);
}

void	absolute_path(char **cmd, char **env)
{
	if (cmd[0] == NULL)
	{
		free_tab(cmd);
		exit(EXIT_FAILURE);
	}
	if (strchr(cmd[0], '/'))
	{
		if (access(cmd[0], X_OK) == 0)
		{
			if (execve(cmd[0], cmd, env) == -1)
			{
				free_tab(cmd);
				ft_putstr_fd("execve failed\n", 2);
				exit(EXIT_FAILURE);
			}
		}
	}
}

int	open_file(char *argv, int check, int pipefd[2])
{
	int	fd;

	fd = 0;
	if (check == 1)
	{
		fd = open(argv, O_CREAT | O_TRUNC | O_WRONLY, 0777);
		if (fd == -1)
			open_error(pipefd);
	}
	else if (check == 2)
	{
		fd = open(argv, O_RDONLY, 0777);
		if (fd == -1)
			open_error(pipefd);
	}
	return (fd);
}

void	exec_cmd(char **cmd, char **env)
{
	int		i;
	char	**cmd_files;

	i = 0;
	cmd_files = split_env_path(cmd, env);
	while (cmd_files[i] != NULL)
	{
		if (access(cmd_files[i], X_OK) == 0)
		{
			if (execve(cmd_files[i], cmd, env) == -1)
			{
				free_tab(cmd_files);
				free_tab(cmd);
				ft_putstr_fd("execve failed\n", 2);
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
	free_tab(cmd);
	free_tab(cmd_files);
	ft_putstr_fd("command not found", 2);
	exit(EXIT_FAILURE);
}
