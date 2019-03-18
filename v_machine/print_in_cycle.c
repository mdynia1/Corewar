/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_in_cycle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <vlikhotk@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 13:54:55 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/11/20 13:55:07 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"

void	intro_print(t_flags *params, t_header bots[MAX_PLAYERS],
	WINDOW **win)
{
	int i;

	i = 0;
	if ((*params).ncurses == 1)
		visual_init(win, params, bots);
	else
	{
		ft_printf("%s\n", "Introducing contestants...");
		while (i < (*params).bots_quantity)
		{
			ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
			bots[i].id + 1, bots[i].prog_size,
			bots[i].prog_name, bots[i].comment);
			i++;
		}
	}
}

/*
** If you want to see more similarities with original
** VM, replace ft_printf phrase to:
** ft_printf("Contestant %d, \"%s\", has won !\n",
** main_cycle.winner_id + 1, main_cycle.winner_name);
*/

void	after_cycle(t_flags *params, t_header bots[MAX_PLAYERS],
	t_cycle main_cycle, WINDOW **win)
{
	int i;

	i = 0;
	while (i < (*params).bots_quantity)
	{
		free(bots[i].exec_part);
		bots[i].exec_part = NULL;
		i++;
	}
	if ((*params).ncurses == 1)
	{
		print_winner(win, &main_cycle);
	}
	if (!(*params).ncurses && !main_cycle.processes)
	{
		if (main_cycle.winner_id >= 0)
			ft_printf("Player %d (%s) won\n",
			main_cycle.winner_id + 1, main_cycle.winner_name);
		else
			ft_printf("%s\n",
				"There is no winner in this game. Friendship won:)");
	}
}

void	print_adv(t_cycle *main_cycle, t_proc *processes, t_flags *params,
	unsigned char *map)
{
	int j;

	j = 0;
	(*main_cycle).indexes[(*processes).current_position % MEM_SIZE][1] =
	NO_CARETKA;
	if (((*params).v_verbosity >> 4) & 1 && (*main_cycle).shift > 0
		&& !(*params).ncurses)
	{
		ft_printf("ADV %d (0x%.4x -> 0x%.4x) ", (*main_cycle).shift,
			(*processes).current_position, (*main_cycle).shift +
			(*processes).current_position);
		while (j < (*main_cycle).shift)
		{
			ft_printf("%.2x ", map[((*processes).current_position
			+ j) % MEM_SIZE]);
			j++;
		}
		ft_printf("\n");
	}
	(*processes).current_position += (*main_cycle).shift;
}

int		external_cycle_pass(t_cycle *main_cycle, unsigned char *map,
	t_flags *params)
{
	if ((*params).ncurses == 1 && (*main_cycle).second_limit > 0)
		usleep((useconds_t)((int)1000000 / (*main_cycle).second_limit));
	(*main_cycle).cycles++;
	if (((*params).d_dumps_memory > 0 && (*main_cycle).cycles ==
		(*params).d_dumps_memory) && !(*params).ncurses)
	{
		print_dump(map);
		return (0);
	}
	return (1);
}

void	print_dump(unsigned char *map)
{
	int i;

	i = 0;
	ft_printf("0x0000 : ");
	while (i < MEM_SIZE)
	{
		ft_printf("%.2x ", map[i]);
		if (++i % 64 == 0 && i < MEM_SIZE)
			ft_printf("\n%#.4x : ", i);
	}
	ft_printf("\n");
}
