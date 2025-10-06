CC=i386-elf-gcc
AR=ar rc
CASM=nasm
LIB_ASM=asm.a
OBJ_DIR=obj/
SRC_DIR=src/
ISO_DIR=bin
LIBFT_DIR=./src/libft
INCLUDES=includes/
GRUB_CONFIG=grub.cfg
LINKER_FILE=linker.ld
ASM_FLAGS=-f elf32 -g
NAME=julo.elf
NAME_ISO=$(subst .elf,.iso,$(NAME))
BOOT_DIR=$(addsuffix /boot,$(ISO_DIR))
GRUB_DIR=$(addsuffix /grub,$(BOOT_DIR))

ASM_SRCS=$(addprefix $(SRC_DIR), boot.s gdts.s idts.s)
ASM_OBJS=$(patsubst $(SRC_DIR)%.s, $(OBJ_DIR)%.o, $(ASM_SRCS))

SRCS= $(addprefix $(SRC_DIR), main.c terminal.c print.c cursor.c utils.c gdt.c \
	idt.c isr.c keyboard.c shell.c  signal.c timer.c)
DEP= $(patsubst $(SRC_DIR)%.c, $(DEP_DIR)%.d, $(SRCS))
OBJS= $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))

CFLAGS=-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs \
	-std=gnu99 -ffreestanding -Wall -Wextra -m32 -I $(INCLUDES) -g

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.s
	$(CASM) $(ASM_FLAGS) $< -o $@

$(NAME): $(SRC_DIR)isrs.s $(SRC_DIR)isr.c $(ASM_OBJS) $(OBJS)
	mkdir -p $(GRUB_DIR)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) -T $(LINKER_FILE) -o $(NAME) $(CFLAGS) $(OBJS) $(ASM_OBJS) $(LIBFT_DIR)/bin/libft.a
	cp $(GRUB_CONFIG) $(GRUB_DIR)
	cp $(NAME) $(BOOT_DIR)
	grub-mkrescue -o $(NAME_ISO) $(ISO_DIR)

$(SRC_DIR)isr.c $(SRC_DIR)isrs.s:
	bash conf/create_isr.sh $(SRC_DIR)isr.c $(SRC_DIR)isrs.s

all: $(NAME)

run:
	qemu-system-i386 $(NAME_ISO)

run_kernel:
	qemu-system-i386 -kernel $(NAME)

run_debug:
	qemu-system-i386 -s -S $(NAME_ISO)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -rf $(NAME)
	rm -rf $(NAME_ISO)
	rm -f $(SRC_DIR)isr.c $(SRC_DIR)isrs.s
	# docker compose down

re: fclean all

.PHONY: all clean fclean re run run_kernel run_debug
