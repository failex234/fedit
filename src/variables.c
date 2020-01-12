#include "fedit.h"
#include "variables.h"

int check_if_var_exists(char *var_name)
{
    if (!vars)
        return 0;

    for (int i = 0; i < vars_len; i++)
    {
        if (!strcmp(vars[i].var_name, var_name))
            return 1;
    }
    return 0;
}

char *get_all_vars()
{
    if (!vars)
        return "";

    char *allvars;
    int curr_len = 0;

    for (int i = 0; i < vars_len; i++)
    {
        int var_len_temp = strlen(vars[i].var_name);

        allvars = realloc(allvars, curr_len + var_len_temp + 1);
        strcat(allvars, vars[i].var_name);
    }
    return allvars;
}

char *get_var(char *var_name)
{
    if (!vars)
        return "";
    char *var_val = NULL;

    for (int i = 0; i < vars_len; i++)
    {
        if (!strcmp(vars[i].var_name, var_name))
        {
            int len = strlen(vars[i].var_val);
            var_val = malloc(len + 1);
            memcpy(var_val, vars[i].var_val, len + 1);
        }
    }
    if (!var_val)
    {
        var_val = "";
    }

    return var_val;
}

void set_var(char *var_name, char *var_value)
{
    int newvar = 0;
    if (!check_if_var_exists(var_name))
    {
        vars = realloc(vars, ++vars_len);
        newvar = 1;
    }

    for (int i = 0; i < vars_len; i++)
    {
        //refactor
        var_t *curr = &vars[i];
        if (newvar && !curr->var_name)
        {
            int name_len = strlen(var_name);
            int val_len = strlen(var_value);
            memcpy(curr->var_name, var_name, name_len + 1);
            memcpy(curr->var_val, var_value, val_len + 1);
            break;
        }

        if (!strcmp(curr->var_name, var_name))
        {
            int name_len = strlen(var_name);
            int val_len = strlen(var_value);
            memcpy(curr->var_name, var_name, name_len + 1);
            memcpy(curr->var_val, var_value, val_len + 1);
        }
    }
}