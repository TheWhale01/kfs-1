CC=i386-elf-gcc
BIN_DIR=bin/
OBJ_DIR=obj/
SRC_DIR=src/
INCLUDES=../../includes
CFLAGS=-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs \
	-std=gnu99 -ffreestanding -Wall -Wextra -Werror -m32 -I $(INCLUDES) -g
CFILES=$(addprefix $(SRC_DIR), ft_abs.c ft_atoll.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_memchr.c ft_memcpy.c ft_memset.c ft_strcmp.c ft_strlcat.c \
	ft_strlen.c ft_strnstr.c ft_swap.c ft_toupper.c ft_atoi.c ft_bzero.c ft_isalpha.c \
	ft_isdigit.c ft_max.c ft_memcmp.c ft_memmove.c ft_min.c ft_strchr.c ft_striteri.c \
	ft_strlcpy.c ft_strncmp.c ft_strrchr.c ft_tolower.c)
OBJS=$(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(CFILES))
NAME=$(BIN_DIR)libft.a

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@mkdir -p $(dir $@)
	ar rc $(NAME) $(OBJS)

all: $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

re: fclean all

.PHONY: all clean fclean re
