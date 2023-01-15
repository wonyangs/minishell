/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subst_env_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongmin <jeongmin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 17:42:37 by jeongmin          #+#    #+#             */
/*   Updated: 2023/01/15 14:28:03 by jeongmin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "make_tree.h"
#include "minishell.h"

extern t_global	g_var;

// substitute_env_lst

static t_error	subst_env_str(t_token *token)
{
	char	*new;
	char	*value;

	new = NULL;
	if (ft_strcmp(token->str, "$?") == 0)
		new = ft_itoa(g_var.status);
	else
	{
		value = search_key_value(&(g_var.envp), token->str);
		if (value)
			new = ft_strdup(value);
		else
			new = ft_strdup("");
	}
	if (!new)
		return (ERROR);
	free(token->str);
	token->str = new;
	return (SCS);
}

static t_error	make_env_lst(char *line, int *arr, t_list **lst)
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

static t_error	strjoin_env_lst(t_list *lst, t_token *token)
{
	char	*new;
	char	*cache;

	cache = ft_strdup("");
	if (!cache)
		return (ERROR);
	while (lst)
	{
		new = ft_strjoin(cache, ((t_token *)(lst->content))->str);
		free(cache);
		if (!new)
			return (ERROR);
		cache = new;
		lst = lst->next;
	}
	free(token->str);
	token->str = cache;
	return (SCS);
}

t_error	subst_env_lst(t_token *token, int *arr, t_list **head)
{
	t_list	*lst;

	if (make_env_lst(token->str, arr, head) == ERROR)
		return (ERROR);
	lst = (*head)->next;
	while (lst)
	{
		if (!is_this_symbol(lst->content, T_ENVP))
		{
			lst = lst->next;
			continue ;
		}
		if (subst_env_str(lst->content) == ERROR)
			return (ERROR);
		lst = lst->next;
	}
	if (strjoin_env_lst((*head)->next, token) == ERROR)
		return (ERROR);
	return (SCS);
}
