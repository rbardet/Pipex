/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:49:45 by rbardet-          #+#    #+#             */
/*   Updated: 2024/10/22 14:43:05 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	a;

	a = 0;
	if (n == 0)
		return (0);
	while (s1[a] && s2[a] && a < n - 1 && s1[a] == s2[a])
		a++;
	return ((unsigned char )s1[a] - (unsigned char )s2[a]);
}
// int main(void)
// {
// 	char str[] = "test";
// 	char str2[] = "";
//	printf("%d\n", ft_strncmp(str, str2,06));
//	printf("%d\n", strncmp(str, str2, 0));
//  return 0;
// }
