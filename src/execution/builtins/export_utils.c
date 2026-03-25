/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:36:08 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/23 16:03:58 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

char	*env_get_value(char *prefix, char **env)
{
	char	*result;
	int		size;

	if (!prefix || !env)
		return (NULL);
	size = 0;
	while (prefix[size])
		size++;
	result = env_find_pointer(prefix, env);
	if (!result)
		return (NULL);
	result += size;
	if (*result == '=')
		result++;
	return (result);
}

char	*env_value_dup(char *prefix, char **env)
{
	char	*string;
	char	*result;
	int		size;
	int		index;

	string = env_get_value(prefix, env);
	if (!string || (string[-1] != '='))
		return (NULL);
	size = 0;
	while (string[size] != '\0')
		size++;
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	index = -1;
	while (string[++index])
		result[index] = string[index];
	result[index] = '\0';
	return (result);
}

char	*env_value_dup_beginning(char *string, int beginning)
{
	char	*result;
	int		size;
	int		index;

	if (!string || beginning < 0 || !string[beginning])
		return (NULL);
	index = beginning;
	size = 0;
	while (string[index])
	{
		index++;
		size++;
	}
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	index = 0;
	while (string[beginning])
	{
		result[index] = string[beginning];
		index++;
		beginning++;
	}
	result[index] = '\0';
	return (result);
}

char	*env_key_dup(char *string, int delimiter)
{
	char	*result;
	int		index;

	if (!string || (delimiter < 0) || string[0] == ' ')
		return (NULL);
	index = 0;
	result = malloc((delimiter + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (string[index] && (index < delimiter))
	{
		result[index] = string[index];
		index++;
	}
	result[index] = '\0';
	return (result);
}

int	parse_export_string(char *string)
{
	int	index;

	if (!string)
		return (0);
	index = 0;
	if (is_numerical(string[index]) || string[index] == '=')
		return (0);
	index++;
	while (string[index] && (string[index] != '='))
	{
		if ((is_alphanumerical_or_underline(string[index]) == false)
			&& (string[index] != '+'))
			return (0);
		if ((string[index] == '+') && (string[index + 1] != '='))
			return (0);
		index++;
	}
	return (1);
}
