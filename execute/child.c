/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wonyang <wonyang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 23:33:05 by wonyang           #+#    #+#             */
/*   Updated: 2023/01/12 18:38:06 by wonyang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ds_envp.h"
#include "execute.h"
#include "libft.h"

extern t_envp	g_envp;

static t_error	child_execute(t_tnode *cmd_node)
{
	char	**cmd_argv;
	char	*path;

	path = NULL;
	cmd_argv = make_argv(cmd_node);
	if (!cmd_argv)
		return (ERROR);
	if (make_cmd_path(cmd_argv[0], &path, g_envp.arr) == ERROR
		|| apply_redirections(cmd_node) == ERROR
		|| execve(path, cmd_argv, g_envp.arr) == FAIL)
	{
		free(path);
		ft_freesplit(cmd_argv);
		return (ERROR);
	}
	return (SCS);
}
static pid_t	fork_child(t_tnode *cmd_node, int *before_fd)
{
	int		fd[2];
	pid_t	child_pid;
	int		tmp;

	if (pipe(fd) == -1)
		return (0);
	tmp = *before_fd;
	*before_fd = fd[0];
	child_pid = fork();
	if (child_pid == -1)
		return (0);
	if (child_pid == 0)
	{
		if (close(fd[0]) == -1
			|| ft_dup2(tmp, STDIN_FILENO) == ERROR
			|| ft_dup2(fd[1], STDOUT_FILENO) == ERROR
			|| child_execute(cmd_node) == ERROR)
			return (0);
	}
	if (close(tmp) == -1 || close(fd[1]) == -1)
		return (0);
	return (child_pid);
}

static pid_t	last_fork_child(t_tnode *cmd_node, int before_fd)
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid == -1)
		return (0);
	if (child_pid == 0)
	{
		if (before_fd != STDIN_FILENO
			&& ft_dup2(before_fd, STDIN_FILENO) == ERROR)
			return (0);
		if (child_execute(cmd_node) == ERROR)
			return (0);
	}
	if (close(before_fd) == -1)
		return (0);
	return (child_pid);
}

t_error	create_childs(t_tnode **cmd_list, pid_t *pid_list)
{
	int	before_fd;
	int	i;

	before_fd = STDIN_FILENO;
	i = 0;
	while (cmd_list[i + 1])
	{
		pid_list[i] = fork_child(cmd_list[i], &before_fd);
		if (pid_list[i] == 0)
			return (ERROR);
		i++;
	}
	pid_list[i] = last_fork_child(cmd_list[i], before_fd);
	if (pid_list[i] == 0)
		return (ERROR);
	return (SCS);
}
