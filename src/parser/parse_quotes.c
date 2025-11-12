
#include "minishell.h"

int	check_quotes(char *str)
{
	int		i = 0;
	char	quote = 0;

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (0);
	}
	return (1);
}

static int	count_tokens(char *str)
{
	int		count = 0;
	int		in_token = 0;
	char	in_quote = 0;
	int		i = 0;

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !in_quote)
			in_quote = str[i];
		else if (str[i] == in_quote)
			in_quote = 0;

		if ((str[i] == ' ' || str[i] == '\t') && !in_quote)
			in_token = 0;
		else if (!in_token)
		{
			in_token = 1;
			count++;
		}
		i++;
	}
	return (count);
}

static int	token_len(char *str)
{
	int		i = 0;
	char	quote = 0;

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if ((str[i] == ' ' || str[i] == '\t') && !quote)
			break;
		else
			i++;
	}
	return (i);
}

static char	*extract_token(char *str, int len)
{
	char	*token;
	int		i = 0, j = 0;
	char	quote = 0;

	token = malloc(len + 1);
	if (!token)
		return (NULL);

	while (i < len)
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
		{
			quote = str[i++];
			continue;
		}
		if (str[i] == quote)
		{
			quote = 0;
			i++;
			continue;
		}
		token[j++] = str[i++];
	}
	token[j] = '\0';
	return (token);
}

char	**split_with_quotes(char *str)
{
	char	**tokens;
	int		count, i = 0, j = 0, len;

	if (!check_quotes(str))
		return (NULL);
	count = count_tokens(str);
	tokens = malloc(sizeof(char *) * (count + 1));
	if (!tokens)
		return (NULL);

	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (str[i])
		{
			len = token_len(&str[i]);
			tokens[j] = extract_token(&str[i], len);
			if (!tokens[j])
			{
				while (--j >= 0)
					free(tokens[j]);
				free(tokens);
				return (NULL);
			}
			j++;
			i += len;
		}
	}
	tokens[j] = NULL;
	return (tokens);
}

