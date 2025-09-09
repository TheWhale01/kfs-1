#include "libft.h"

void	*ft_memmove(void *dest, void *src, size_t n)
{
	char	*src_c;
	char	*dst_c;

	src_c = src;
	dst_c = dest;
	if (dest < src)
		ft_memcpy(dest, src, n);
	else
		while ((int)--n >= 0)
			*(dst_c + n) = *(src_c + n);
	return (dest);
}
