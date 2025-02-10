/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:10:07 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/10 16:44:33 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

pid_t	child(char **argv, t_pipe pipex, char **env)
{
	char	**cmd;
	pid_t	pid;

	if (pipe(pipex.pipefd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(pipex.pipefd[0]);
		dup2(pipex.pipefd[1], STDOUT_FILENO);
		close(pipex.pipefd[1]);
		if (pipex.infile > 0)
			close(pipex.infile);
		if (pipex.outfile > 0)
			close(pipex.outfile);
		cmd = ft_split(argv[pipex.child_count], ' ');
		absolute_path(cmd, env, pipex);
		exec_cmd(cmd, env, pipex);
	}
	close(pipex.pipefd[1]);
	dup2(pipex.pipefd[0], STDIN_FILENO);
	close(pipex.pipefd[0]);
	return (pid);
}

pid_t	parent(int argc, char **argv, t_pipe pipex, char **env)
{
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
		if (pipex.outfile > 0)
			close(pipex.outfile);
		if (pipex.infile > 0)
			close(pipex.infile);
		cmd = ft_split(argv[argc - 2], ' ');
		absolute_path(cmd, env, pipex);
		exec_cmd(cmd, env, pipex);
	}
	return (pid);
}

void	handle_pipe(t_pipe pipex, int argc, char **argv, char **env)
{
	int		pid_nb;
	int		i;

	pid_nb = 0;
	i = pipex.child_count;
	pipex.wait_pid = malloc(sizeof(int) * (argc - (pipex.child_count)));
	while (pipex.child_count < argc - i)
	{
		pipex.wait_pid[pid_nb] = child(argv, pipex, env);
		pipex.child_count++;
		pid_nb++;
	}
	dup2(pipex.outfile, STDOUT_FILENO);
	pipex.wait_pid[pid_nb] = parent(argc, argv, pipex, env);
	while (pid_nb != -1)
	{
		waitpid(pipex.wait_pid[pid_nb], NULL, 0);
		pid_nb--;
	}
	if (pipex.outfile > 0)
		close(pipex.outfile);
	if (pipex.infile > 0)
		close(pipex.infile);
	free(pipex.wait_pid);
}

int	main(int argc, char **argv, char **env)
{
	t_pipe	pipex;

	pipex.infile = -1;
	pipex.outfile = -1;
	if (argc < 5)
	{
		ft_putstr_fd("Wrong format :file1 cmd1 cmd2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
	{
		if (argc < 6)
			exit(EXIT_FAILURE);
		check_first_arg(argv[3], env);
		pipex = handle_here_doc(argv, argc);
	}
	else
	{
		pipex.child_count = 2;
		pipex.infile = open_file(argv[1], 2, pipex);
		pipex.outfile = open_file(argv[argc - 1], 1, pipex);
		dup2(pipex.infile, STDIN_FILENO);
	}
	handle_pipe(pipex, argc, argv, env);
	return (0);
}
