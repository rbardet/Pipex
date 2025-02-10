/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 04:21:04 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/10 16:42:24 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "../LIBFT_V2/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

void	absolute_path(char **cmd, char **env);
int		tab_size(char **tab);
pid_t	child(char **argv, int pipefd[2], char **env);
pid_t	parent(char **argv, int pipefd[2], char **env);
char	*get_env(char **env, char **cmd);
char	**split_env_path(char **cmd, char **env);
void	exec_cmd(char **cmd, char **env);
int		open_file(char *argv, int check, int pipefd[2]);
void	open_error(int pipefd[2]);

#endif
