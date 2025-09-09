#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				i;
	unsigned char	x;

	x = c;
	i = ft_strlen(s);
	if (x == 0)
		return ((char *)&s[i]);
	while (--i >= 0)
		if (s[i] == x)
			return ((char *)&s[i]);
	return (0);
}
