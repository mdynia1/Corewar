/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_funcs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdynia <mdynia@student.unit.ua>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 01:06:00 by mdynia            #+#    #+#             */
/*   Updated: 2018/12/07 01:06:01 by mdynia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static t_op	g_op_tab[17] =
{
	{"live", 1, {T_DIR}, 1, 10, "alive", 0, 0},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"et (and  r1, r2, r3   r1&r2 -> r3", 1, 0},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 0},
	{"zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 1},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 1},
	{"fork", 1, {T_DIR}, 12, 800, "fork", 0, 1},
	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 1},
	{"lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1},
	{"aff", 1, {T_REG}, 16, 2, "aff", 1, 0},
	{0, 0, {0}, 0, 0, 0, 0, 0}
};

int			label_name_is_duplicate(t_binfile *file, char *label_name)
{
	t_lable	*tmp;

	if (file->labels_list)
	{
		tmp = file->labels_list;
		while (tmp)
		{
			if (tmp->label_name)
			{
				if (!(ft_strcmp(tmp->label_name, label_name)))
					return (1);
			}
			tmp = tmp->next;
		}
	}
	return (0);
}

int			label_name_is_valid(t_binfile *file, t_lable *label, char *str)
{
	int		i;
	char	*label_name;

	i = 0;
	if (!(label_name = (char *)ft_memalloc(sizeof(char) * ft_strlen(str) + 1)))
		return (0);
	while (str[i] && str[i] != LABEL_CHAR)
	{
		if (!(ft_strchr(LABEL_CHARS, str[i])))
			return (0);
		label_name[i] = str[i];
		i++;
	}
	label_name[i] = LABEL_CHAR;
	label_name[i + 1] = '\0';
	if (label_name_is_duplicate(file, label_name))
	{
		ft_strdel(&label_name);
		ft_printf("%s\n", "Duplicate label!");
		return (0);
	}
	label->label_name = label_name;
	return (1);
}

int			arguments_add_validator(int size, char *arg)
{
	if (size == T_DIR)
	{
		if (!(arg + 1) || (!(ft_isdigit(arg[1])) && arg[1] != '-')
			|| !all_digits(arg + 1))
			return (0);
	}
	else if (size == T_REG)
	{
		if (!(arg + 1) || !(ft_isdigit(arg[1])) || !all_digits(arg + 1)
			|| ft_atoi(arg + 1) >= 100)
			return (0);
	}
	else if (size == T_IND)
	{
		if (!all_digits(arg))
			return (0);
	}
	return (1);
}

int			arguments_lbl_validator(t_binfile *file, t_t *token, char *arg)
{
	char	*str;

	str = ft_strstr(arg, file->z) ? ft_strjoin(arg + 2, file->w)
	: ft_strjoin(arg + 1, file->w);
	if (!(ft_strstr(file->f_contents, str)))
	{
		ft_strdel(&str);
		return (ft_strstr(arg, file->z) ?
			error_message_label(file, token, arg + 2, arg)
			: error_message_label(file, token, arg + 1, arg));
	}
	ft_strdel(&str);
	return (1);
}

int			arguments_validator(t_binfile *file, t_t *token, char *arg, int i)
{
	int		size;
	int		t;

	t = g_op_tab[token->c_name].param_types[i];
	size = argument_adder2(arg) == 3 ? 4 : argument_adder2(arg);
	if (size == t || t == 7 || (size < t && t - size != size &&
		(t - size == T_REG || t - size == T_DIR || t - size == T_IND)))
	{
		if (ft_strstr(arg, file->z) || ft_strchr(arg, LABEL_CHAR))
		{
			if (!arguments_lbl_validator(file, token, arg))
				return (0);
		}
		else if ((size == T_DIR || size == T_REG || size == T_IND) &&
			(!(arguments_add_validator(size, arg))))
			return (error_message(file, arg, token->line_num));
		return (1);
	}
	return (error_invalid_arg_type(token, i, size));
}
