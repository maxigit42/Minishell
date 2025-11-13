
#include "minishell.h"

// he sustituido la funcion de check_quotes por esta
int quotes_closed(char *s)
{
	static int	q_s;
	static int q_d;

	if (!s)
		return (1);
	q_s = 0;
	q_d = 0;
	while (*s)
	{
		if (*s == '\'' && !q_d)
			q_s = !q_s;
		if (*s == '\"' && !q_s)
			q_d = !q_d;
		s++;
	}
	if (q_s || q_d)
		return (0);
	else
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

t_parse_token *split_with_quotes(char *str)
{
    t_parse_token *tokens;
    int count = count_tokens(str);
    int i = 0, j = 0;

	// quite la verificacion q hacias aqui por que ya esta seguro

    tokens = malloc(sizeof(t_parse_token) * (count + 1));
    if (!tokens)
        return NULL;

    while (str[i])
    {
        while (str[i] == ' ' || str[i] == '\t')
            i++;

        if (!str[i])
            break;

        int len = token_len(&str[i]);
        tokens[j].in_single_quote = 0;
        tokens[j].in_double_quote = 0;

        // Determinar si el token está entre comillas
        if (str[i] == '\'')
            tokens[j].in_single_quote = 1;
        else if (str[i] == '"')
            tokens[j].in_double_quote = 1;

        tokens[j].str = extract_token(&str[i], len);
        i += len;
        j++;
    }

    tokens[j].str = NULL;
    return tokens;
}


