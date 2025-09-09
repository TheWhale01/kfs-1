#pragma once

#include <stddef.h>

int			ft_tolower(int c);
int			ft_toupper(int c);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
int			ft_min(int n, int m);
int			ft_max(int n, int m);
int			ft_atoi(const char *nptr);
int			ft_memcmp(void *s1, void *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);

void		ft_swap(int *a, int *b);
void		ft_bzero(void *s, size_t n);
void		ft_striteri(char *s, void (*f)(unsigned int, char *));

char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strnstr(const char *haystack, const char *needle, size_t n);

void		*ft_memset(void *s, int c, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memmove(void *dest, void *src, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);

size_t      abs(int n);
size_t		ft_strlen(char const *str);
size_t		ft_strlcat(char *dst, char *src, size_t size);
size_t		ft_strlcpy(char *dest, char *src, size_t size);

long long	ft_atoll(const char *nptr);
