#include "minishell.h"

// checkea de que no queden comillas abiertas

int check_quotes(char *str)
{
    int i;
    int single;
    int doble;

    single = 0;
    doble = 0;
    i = 0;
    while(str[i])
    {
        if(str[i] == '\'' && doble == 0)
            single = !single;
        else if(str[i] == '"' && single == 0)
            doble = !doble;
        i++;
    }
    if(single || doble)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
        return(0);
    }
    return(1);
}

// cuenta cuantos tokens tiene cada comando 

static int	count_tokens(char *str)
{
	int	count;
	int	in_token;
	int	i;
	int	in_quote;

	count = 0;
	in_token = 0;
	i = 0;
	in_quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !in_quote)
			in_quote = str[i];
		else if (str[i] == in_quote)
			in_quote = 0;
		if (str[i] != ' ' && str[i] != '\t' && !in_token)
		{
			in_token = 1;
			count++;
		}
		else if ((str[i] == ' ' || str[i] == '\t') && !in_quote)
			in_token = 0;
		i++;
	}
	return (count);
}
// un strlen de cada token

static int token_len(char *str)
{
    int i;
    int in_quote;

    in_quote = 0;
    i = 0;
    while(str[i])
    {
        if((str[i] == '\'' || str[i] == '"') && !in_quote)
            in_quote = str[i++];
        else if(str[i] == in_quote)
            in_quote = 0;
        if((str[i] == ' ' || str[i] ==  '\t') && !in_quote)
            break;
        i++;
    }
    return(i);
}
// extre el token

static char	*extract_token(char *str, int len)
{
	char	*token;
	int		i;
	int		j;
	char	quote;

	token = malloc(len + 1);
	if (!token)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (i < len)
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			token[j++] = str[i++];
	}
	token[j] = '\0';
	return (token);
}

char **split_with_quotes(char *str)
{
    char **tokens;
    int count;
    int i;
    int j;
    int len;

    if(!check_quotes(str))
        return(NULL);
    count = count_tokens(str);
    tokens = malloc(sizeof(char *) * (count + 1));
    if(!tokens)
        return(NULL);
    i = 0;
    j = 0;
    while(str[i])
    {
        while(str[i] == ' ' || str[i] == '\t')
            i++;
        if(str[i])
        {
            len = token_len(&str[i]);
            tokens[j++] = extract_token(&str[i], len);
            i += len;
        }
    }
    tokens[j] = NULL;
    return(tokens);
}
