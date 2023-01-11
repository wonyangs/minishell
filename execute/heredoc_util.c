/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wonyang <wonyang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 20:42:06 by wonyang           #+#    #+#             */
/*   Updated: 2023/01/11 20:49:19 by wonyang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "ds_tree.h"
#include "token.h"
#include "libft.h"
#include "execute.h"

static char	*make_heredoc_filename(void)
{
	char	*res;
	int		fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (NULL);
	res = (char *)ft_calloc(sizeof(char), 31);
	read(fd, res, 30);
	return (res);
}

static void	heredoc_readline(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

char	*execute_heredoc(t_tnode *node)
{
	char	*delimiter;
	char	*file_name;
	int		fd;

	delimiter = ((t_token *)(node->left->content))->str;
	file_name = make_heredoc_filename();
	if (!file_name)
		return (NULL);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (NULL);
	heredoc_readline(fd, delimiter);
	// todo : update node
	if (close(fd) == -1)
		return (NULL);
	return (file_name);
}
