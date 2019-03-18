/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 12:45:32 by vlikhotk          #+#    #+#             */
/*   Updated: 2017/11/04 12:45:36 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i])
	{
		if (s1[i] == s2[i])
			i++;
		else
			return (int)((unsigned char)s1[i] - (unsigned char)s2[i]);
	}
	if (!s1[i] && !s2[i])
		return (0);
	return (int)((unsigned char)s1[i] - (unsigned char)s2[i]);
}
