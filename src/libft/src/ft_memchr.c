#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	to_find;
	unsigned char	*tab;

	i = 0;
	tab = (unsigned char *)s;
	to_find = (unsigned char)c;
	while (i < n)
		if (tab[i++] == to_find)
			return (&tab[i - 1]);
	return (0);
}
