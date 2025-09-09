#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*tab_dest;
	const char	*tab_src;

	tab_src = (char *)src;
	tab_dest = (char *)dest;
	while (n--)
		*(tab_dest++) = *(tab_src++);
	return (dest);
}
