void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	index;

	index = -1;
	while (s[++index])
		f(index, &s[index]);
}
