/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:26:31 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/25 11:44:23 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

/* const char	*redir_type_to_str(t_redir_type type) */
/* { */
/* 	if (type == REDIR_INPUT) */
/* 		return ("REDIR_INPUT (<)"); */
/* 	if (type == REDIR_OUTPUT) */
/* 		return ("REDIR_OUTPUT (>)"); */
/* 	if (type == REDIR_APPEND) */
/* 		return ("REDIR_APPEND (>>)"); */
/* 	if (type == REDIR_HEREDOC) */
/* 		return ("REDIR_HEREDOC (<<)"); */
/* 	return ("UNKNOWN"); */
/* } */

/* void	print_redirections(t_redir *redir) */
/* { */
/* 	int	i; */

/* 	i = 0; */
/* 	while (redir) */
/* 	{ */
/* 		printf("      [%d] type: %s | arg: \"%s\" | quote: %s\n", i++, */
/* 			redir_type_to_str(redir->type), */
/* 			redir->redir_arg ? redir->redir_arg : "(null)", */
/* 			redir->has_quote ? "YES" : "NO"); */
/* 		redir = redir->next; */
/* 	} */
/* } */

/* void	print_command(t_cmd *cmd) */
/* { */
/* 	int	i; */
/* 	int	j; */

/* 	i = 0; */
/* 	while (cmd) */
/* 	{ */
/* 		printf("┌─ Command [%d]\n", i++); */
/* 		printf("│  cmd  : %s\n", cmd->comand ? cmd->comand : "(null)"); */
/* 		printf("│  args : "); */
/* 		if (!cmd->args) */
/* 			printf("(null)\n"); */
/* 		else */
/* 		{ */
/* 			j = 0; */
/* 			printf("["); */
/* 			while (cmd->args[j]) */
/* 			{ */
/* 				printf("%s", cmd->args[j]); */
/* 				if (cmd->args[j + 1]) */
/* 					printf(", "); */
/* 				j++; */
/* 			} */
/* 			printf("%sNULL]\n", j > 0 ? ", " : ""); */
/* 		} */
/* 		printf("│  redirs:\n"); */
/* 		if (!cmd->redir) */
/* 			printf("│    (none)\n"); */
/* 		else */
/* 			print_redirections(cmd->redir); */
/* 		printf("└─────────────────────────\n"); */
/* 		cmd = cmd->next; */
/* 	} */
/* } */
