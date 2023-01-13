/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_first_step.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongmin <jeongmin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 22:41:40 by jeongmin          #+#    #+#             */
/*   Updated: 2023/01/13 23:37:11 by jeongmin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "make_tree.h"
#include "minishell.h"

// substitute_env

extern t_global	g_var;

static void	print_arr(int *arr, char *line)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(line))
	{
		printf(" %d", arr[i]);
		i++;
	}
	printf("\n");
}

static void	print_lst(t_list *lst)
{
	t_token	*token;

	while (lst)
	{
		token = (t_token *)(lst->content);
		printf("[%d] [%s]\n", token->type, token->str);
		lst = lst->next;
	}
}

static int	check_cust_idx(char *line)
{
	int			i;
	const char	symbol[5][3] = {"\"", "\'", "$?", "$", " "};
	const int	cust_idx[] = {50, 52, 81, 70, 60};

	i = 0;
	while (i < 5)
	{
		if (ft_strncmp(line, (char *)(symbol[i]), ft_strlen(symbol[i])) == 0)
			return (cust_idx[i]);
		i++;
	}
	return (0);
}

static t_error	make_list(char *line, int *arr, t_list **lst)
{
	int		cust_idx;
	size_t	len;
	char	*start;

	start = line;
	while (*start)
	{
		cust_idx = arr[start - line];
		if (cust_idx == T_DOLLER * 10 + 1)
		{
			start++;
			continue ;
		}
		if (cust_idx == T_ENVP * 10 || cust_idx == T_WORD)
			len = count_len(line, arr, start - line);
		else
			len = cust_idx % 10 % 2 + 1;
		if (make_token(start, cust_idx / 10, len, lst) == ERROR)
			return (ERROR);
		start += len;
	}
	return (SCS);
}

t_error	env_first_step(t_token *token)
{
	int		*arr;
	t_list	*lst;

	arr = init_arr(ft_strlen(token->str));
	lst = ft_lstnew(NULL);
	if (!arr || !lst)
	{
		free(arr);
		free(lst);
		return (ERROR);
	}
	fill_arr(token->str, arr, &check_cust_idx);
	handling_envp(arr, ft_strlen(token->str));
	handling_quote(token->str, arr, '\"');
	handling_quote(token->str, arr, '\'');
	if (make_list(token->str, arr, &lst) == ERROR)
	{
		free(arr);
		ft_lstclear(&lst, &del_t_token);
		return (ERROR);
	}
	print_arr(arr, token->str);
	print_lst(lst->next);
	return (SCS);
}
