/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:43:39 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/10 16:45:51 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

void	absolute_path(char **cmd, char **env, t_pipe pipex)
{
	if (cmd[0] == NULL)
	{
		free_tab(cmd);
		free(pipex.wait_pid);
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

void	open_error(void)
{
	ft_putstr_fd("error with open\n", 2);
	exit(EXIT_FAILURE);
}

int	open_file(char *argv, int check, t_pipe pipex)
{
	int	fd;

	fd = 0;
	if (check == 1)
	{
		fd = open(argv, O_CREAT | O_TRUNC | O_WRONLY, 0777);
		if (fd == -1)
		{
			close(pipex.infile);
			open_error();
		}
	}
	else if (check == 2)
	{
		fd = open(argv, O_RDONLY, 0777);
		if (fd == -1)
			open_error();
	}
	else if (check == 3)
	{
		fd = open(argv, O_CREAT | O_APPEND | O_WRONLY, 0777);
		if (fd == -1)
			open_error();
	}
	return (fd);
}

void	exec_cmd(char **cmd, char **env, t_pipe pipex)
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
				free(pipex.wait_pid);
				ft_putstr_fd("execve failed\n", 2);
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
	free_tab(cmd);
	free_tab(cmd_files);
	free(pipex.wait_pid);
	ft_putstr_fd("command not found\n", 2);
	exit(EXIT_FAILURE);
}
