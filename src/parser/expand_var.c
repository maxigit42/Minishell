#include "minishell.h"

static int get_var_name_len(char *str)
{
    int len;

    len = 0;
    if(str[len] == '?')
        return(1);
    
    if (!ft_isalpha(str[len]) && str[len] != '_')
        return (0); 
    while(str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
    return(len);
}

static char *get_var_name(char *str, int len)
{
    char *name;

    if(len <= 0)
        return(NULL);
    name = malloc(len + 1);
    if(!name)
        return(NULL);
    ft_strlcpy(name, str, len + 1); // si da error quitar el + 1
    return(name);
}

static int handle_special_dollar(char *str, char *result, int j)
{
    if(str[1] == '\0' || str[1] == ' ' || str[1] == '\t')
    {
        result[j++] = '$';
        return(j);
    }
    if (ft_isdigit(str[1]))
    {
        /* $1, $2, etc. → vacío en minishell (no hay parámetros posicionales) */
        return (j);
    }
    if (str[1] == '@' || str[1] == '*' || str[1] == '#')
    {
        /* $@, $*, $# → vacío en minishell */
        return (j);
    }
    /* Otro carácter especial → $ literal */
    result[j++] = '$';
    return (j);
}

static int calculate_expanded_len(char *str, t_env *env, int exit_status)
{
    int len;
    char quote;
    int i;

    len = 0;
    quote = 0;
    i = 0;
    while(str[i])
    {
        if((str[i] == '\'' || str[i] == '"') && !quote)
        {
            quote = str[i];
            i++;
            continue;
        }
        if(str[i] == quote)
        {
            quote = 0;
            i++;
            continue;
        }
        if(str[i] == '$' && quote != '\'')
        {
            int var_len = get_var_name_len(&str[i + 1]);
            if(var_len == 0)
            {
                len++;
                i++;
            }
            else if(str[i + 1] == '?')
            {
                char *exit_str = ft_itoa(exit_status);
                len += ft_strlen(exit_str);
                free(exit_str);
                i += 2;
            }
            else
			{
				char *var_name = get_var_name(&str[i + 1], var_len);
				if (var_name)
                {
                    char *value = get_env_value_list(env, var_name);
                    if(value)
					    len += ft_strlen(value);
				    free(var_name);
                }
                i += var_len + 1;
			}
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

static int expand_var(char *str, char *result, int j, t_data *data)
{
    int var_len;
    char *var_name;
    char *value;

    var_len = get_var_name_len(&str[1]);
    if(var_len == 0)
        return(handle_special_dollar(str, result, j));
    if(str[1] == '?')
    {
        char *exit_var = ft_itoa(data->exit_status);
        if(!exit_var)
            return(j);
        ft_strlcpy(&result[j], exit_var, ft_strlen(exit_var) + 1);
        j += ft_strlen(exit_var);
        free(exit_var);
        return(j);
    }
    var_name = get_var_name(&str[1], var_len);
    if(!var_name)
        return(j);
    value = get_env_value_list(data->env, var_name);
    if(value)
    {
        ft_strlcpy(&result[j], value, ft_strlen(value) + 1);
        j += ft_strlen(value);
    }
    free(var_name);
    return(j);
}

char *expand_variables(char *str, t_data *data)
{
    char *result;
    int i;
    int j;
    char quote;
    int len;

    if(!str)
        return(NULL);
    len = calculate_expanded_len(str, data->env, data->exit_status);
    result = malloc(len + 1);
    if(!result)
        return(NULL);
    i = 0;
    j = 0;
    quote = 0;
    while(str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && !quote)
		{
            quote = str[i];
            i++;
            continue;
        }	
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
            continue;
		}
        if(str[i] == '$' && quote != '\'')
        {
            int var_len = get_var_name_len(&str[i + 1]);
            j = expand_var(&str[i], result, j, data);
            i += var_len + 1;
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return(result);
}

char **process_tokens(char **tokens, t_data *data)
{
    char **processed;
    int i;

    if(!tokens)
        return(NULL);
    i = 0;
    while(tokens[i])
        i++;
    processed = malloc(sizeof(char *) * (i + 1));
    if(!processed)
    {
        free_split(tokens);
        return(NULL);
    }
    i = 0;
    while(tokens[i])
    {
        processed[i] = expand_variables(tokens[i], data);
        if(!processed[i])
        {
            while(--i >= 0)
                free(processed[i]);
            free(processed);
            free_split(tokens);
            return(NULL);
        }
        i++;
    }
    processed[i] = NULL;
    free_split(tokens);
    return(processed);
}
