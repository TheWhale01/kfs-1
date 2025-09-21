CC=./opt/cross/bin/i386-elf-gcc
AR=ar rc
CASM=nasm
LIB_ASM=asm.a
OBJ_DIR=obj/
DEP_DIR=dep/
SRC_DIR=src/
ISO_DIR=bin
LIBFT_DIR=./src/libft
INCLUDES=includes/
GRUB_CONFIG=grub.cfg
LINKER_FILE=linker.ld
ASM_FLAGS=-f elf32
NAME=julo.elf
NAME_ISO=$(subst .elf,.iso,$(NAME))
BOOT_DIR=$(addsuffix /boot,$(ISO_DIR))
GRUB_DIR=$(addsuffix /grub,$(BOOT_DIR))

ASM_SRCS=$(addprefix $(SRC_DIR), boot.s)
ASM_OBJS=$(patsubst $(SRC_DIR)%.s, $(OBJ_DIR)%.o, $(ASM_SRCS))

SRCS= $(addprefix $(SRC_DIR), main.c vga.c printk.c cursor.c printaddr.c putnbr.c \
	inb.c outb.c)
DEP= $(patsubst $(SRC_DIR)%.c, $(DEP_DIR)%.d, $(SRCS))
OBJS= $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))

CFLAGS=-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs \
	-std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -I $(INCLUDES) -g

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@mkdir -p $(subst $(OBJ_DIR), $(DEP_DIR), $(dir $@))
	$(CC) $(CFLAGS) -MMD -MP -MF $(patsubst $(OBJ_DIR)%.o, $(DEP_DIR)%.d, $@) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.s
	$(CASM) $(ASM_FLAGS) $< -o $@

$(NAME): $(OBJS) $(ASM_OBJS)
	mkdir -p $(GRUB_DIR)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) -T $(LINKER_FILE) -o $(NAME) $(CFLAGS) $(OBJS) $(ASM_OBJS) $(LIBFT_DIR)/bin/libft.a
	cp $(GRUB_CONFIG) $(GRUB_DIR)
	cp $(NAME) $(BOOT_DIR)
	grub-mkrescue -o $(NAME_ISO) $(ISO_DIR)

all: $(NAME)

run:
	qemu-system-i386 -cdrom $(NAME_ISO)

run_kernel:
	qemu-system-i386 -kernel $(NAME)

run_debug:
	qemu-system-i386 -cdrom $(NAME_ISO) -s -S

sanitize: fclean
sanitize: CFLAGS=-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib \
	-nodefaultlibs -std=gnu99 -ffreestanding -O2 -Wall -Wextra -g
sanitize: $(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR) $(DEP_DIR) $(ISO_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -rf $(NAME)
	rm -rf $(NAME_ISO)
	# docker compose down

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re sanitize run run_kernel
