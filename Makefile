CC=gcc
CASM=nasm
ASM_FLAGS=-felf32
NAME=kfs1.bin
NAME_ISO=$(subst .bin,.iso, $(NAME))
OBJ_DIR=obj/
DEP_DIR=dep/
SRC_DIR=src/
INCLUDES=includes/
ISO_DIR=./bin
BOOT_DIR=$(addsuffix /boot, $(ISO_DIR))
GRUB_DIR=$(addsuffix /grub, $(BOOT_DIR))
GRUB_CONFIG=grub.cfg

BOOT_FILE=$(addprefix $(SRC_DIR), boot.asm)
BOOT_FILE_OBJ=$(addprefix $(OBJ_DIR), boot.o)
LINKER_FILE=$(addprefix $(SRC_DIR), linker.ld)

SRCS= $(addprefix $(SRC_DIR), main.c)
DEP= $(patsubst $(SRC_DIR)%.c, $(DEP_DIR)%.d, $(SRCS))
OBJS= $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))

CFLAGS=-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs \
	-std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -I $(INCLUDES)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@mkdir -p $(subst $(OBJ_DIR), $(DEP_DIR), $(dir $@))
	$(CC) $(CFLAGS) -MMD -MP -MF $(patsubst $(OBJ_DIR)%.o, $(DEP_DIR)%.d, $@) -c $< -o $@

$(NAME): $(OBJS)
	echo $(NAME_ISO)
	echo $(GRUB_DIR)
	$(CASM) $(ASM_FLAGS) $(BOOT_FILE) -o $(BOOT_FILE_OBJ)
	$(CC) -T $(LINKER_FILE) -o $(NAME) $(CFLAGS) $(OBJS) $(BOOT_FILE_OBJ)
	mkdir -p $(GRUB_DIR)
	cp $(NAME) $(BOOT_DIR)
	cp $(GRUB_CONFIG) $(GRUB_DIR)
	docker compose up --build

all: $(NAME)

run:
	qemu-system-i386 -cdrom kfs1.iso

run_kernel:
	qemu-system-i386 -kernel kfs1.bin

sanitize: fclean
sanitize: CFLAGS=-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib \
	-nodefaultlibs -std=gnu99 -ffreestanding -O2 -Wall -Wextra
sanitize: $(NAME)

clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR) $(ISO_DIR)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME_ISO)
	docker compose down

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re sanitize run run_kernel
