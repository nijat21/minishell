/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_ops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 19:25:21 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/25 03:02:04 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

static char	**nonquote_var(char **args, char **var_args)
{
	char	**res;

	if (!var_args)
	{
		ft_putstr_fd("nonqute_var: !var_args\n", STDERR_FILENO);
		ft_free_arr(args);
		return (NULL);
	}
	if (!ft_arrlen(var_args))
		return (ft_arrconcat_free(args, var_args));
	args = add_str_to_last_arg(args, var_args[0]);
	if (!args)
	{
		ft_free_arr(var_args);
		return (args);
	}
	res = ft_arrconcat(args, var_args + 1);
	ft_free_arr(args);
	ft_free_arr(var_args);
	return (res);
}

static char	**seg_expand_split(t_seg *seg, t_all *all, char **args)
{
	char	*exp;
	char	**var_args;

	exp = expand_var(seg->val, all);
	if (!exp)
	{
		ft_putstr_fd("seg_expand_split: !exp\n", STDERR_FILENO);
		return (NULL);
	}
	if (!seg->has_quote)
	{
		var_args = ft_split(exp, ' ');
		free(exp);
		args = nonquote_var(args, var_args);
		if (!args)
			ft_putstr_fd("seg_expand_split: !args\n", STDERR_FILENO);
		return (args);
	}
	var_args = add_str_to_last_arg(args, exp);
	free(exp);
	return (var_args);
}

static char	**seg_to_args(t_seg *seg, t_all *all)
{
	char	**args;
	char	**new_args;

	args = NULL;
	while (seg)
	{
		if (!(seg->val))
		{
			ft_putstr_fd("seg_to_args: !seg->value\n", 2);
			ft_free_arr(args);
			return (NULL);
		}
		if (seg->expand)
			new_args = seg_expand_split(seg, all, args);
		else
			new_args = add_str_to_last_arg(args, seg->val);
		if (!new_args)
		{
			ft_putstr_fd("seg_to_args: !args\n", 2);
			return (NULL);
		}
		args = new_args;
		seg = seg->next;
	}
	return (args);
}

char	**word_tokens_to_args(t_token **tk, t_all *all)
{
	char	**args;
	char	**new_args;

	args = NULL;
	while ((*tk) && (*tk)->type == WORD)
	{
		new_args = seg_to_args((*tk)->seg_list, all);
		if (!new_args)
		{
			ft_putstr_fd("word_tokens_to_args: !new_str\n", STDERR_FILENO);
			return (NULL);
		}
		args = ft_arrconcat_free(args, new_args);
		if (!args)
		{
			ft_putstr_fd("word_tokens_to_args: !args\n", STDERR_FILENO);
			return (NULL);
		}
		if (!(*tk)->next || ((*tk)->next && (*tk)->next->type != WORD))
			break ;
		(*tk) = (*tk)->next;
	}
	return (args);
}
