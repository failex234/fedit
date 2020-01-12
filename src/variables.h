#ifndef FLX_VARS_H
#define FLX_VARS_H

typedef struct var_t
{
    char var_name[16];
    char var_val[16];
} var_t;

var_t *vars;
size_t vars_len;
#endif