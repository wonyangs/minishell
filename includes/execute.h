/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wonyang <wonyang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 20:33:04 by wonyang           #+#    #+#             */
/*   Updated: 2023/01/13 15:04:27 by wonyang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "ds_tree.h"
# include "return.h"

// preprocess.c
t_tnode	**make_cmd_list(t_tnode *root);

// execute.c
t_error	execute_cmds(t_tnode *root);

// child.c
t_error	create_childs(t_tnode **cmd_list, pid_t *pid_list);

// argv.c
char	**make_argv(t_tnode *node);

// redirection.c
t_error	apply_redirections(t_tnode *node);

// path.c
t_error	make_cmd_path(char *cmd_name, char **path, char **envp);

// heredoc.c
t_error	execute_all_heredoc(t_tnode **cmd_list);

// heredoc_util.c
t_error	execute_heredoc(t_tnode *node);

// util.c
t_error	ft_dup2(int fd1, int fd2);

#endif