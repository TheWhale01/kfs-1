char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	x;

	i = -1;
	x = c;
	while (s && s[++i])
		if (s[i] == x)
			return ((char *)&s[i]);
	if (s && s[i] == x)
		return ((char *)&s[i]);
	return (0);
}
