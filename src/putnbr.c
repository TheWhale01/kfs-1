#include "julo.h"
#include "libft.h"

size_t putnbr_base(terminal_t* terminal, int nb, const char *base) {
    size_t len = 0;
    size_t base_len = ft_strlen(base);
    size_t nbr;

    base_len = ft_strlen(base);
    if (nb < 0)  {
        nbr = nb * -1;
        len++;
        vga_putchar(terminal, '-');
    }
    else
        nbr = nb;
    if (nbr / base_len)
        len += putnbr_base(terminal, nbr / base_len, base);
    len++;
    vga_putchar(terminal, base[nbr % base_len]);
    return len;
}

size_t putnbr_base_u(terminal_t *terminal, size_t nb, const char *base) {
    size_t len = 0;
    size_t base_len = ft_strlen(base);

    if (nb / base_len)
        len = putnbr_base(terminal, nb / base_len, base);
    vga_putchar(terminal, base[nb % base_len]);
    return ++len;
}
