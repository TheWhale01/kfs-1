#include <stddef.h>

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*tab;

	i = 0;
	tab = (char *)s;
	while (i < n)
		tab[i++] = '\0';
}
