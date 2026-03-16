// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   create_redir_lst.c                                 :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/03/04 09:14:01 by olacerda          #+#    #+#             */
// /*   Updated: 2026/03/11 13:20:48 by olacerda         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include <data.h>

int	is_redirection(char *string)
{
	if (!string)
		return (-1);
	if (string_compare(string, ">") == 0)
		return (REDIR_OUTPUT);
	else if (string_compare(string, "<") == 0)
		return (REDIR_INPUT);
	else if (string_compare(string, ">>") == 0)
		return (REDIR_APPEND);
	else if (string_compare(string, "<<") == 0)
		return (REDIR_HEREDOC);
	return (0);
}
