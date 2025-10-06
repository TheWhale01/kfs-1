#include "keyboard.h"
#include "idt.h"
#include "julo.h"
#include "terminal.h"

const char SCANCODE_INIT_QWERTY[128] = {
    [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',[0x07]='6',[0x08]='7',[0x09]='8',
    [0x0A]='9',[0x0B]='0',[0x0C]='-',[0x0D]='=',
    [0x10]='q',[0x11]='w',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='y',[0x16]='u',[0x17]='i',
    [0x18]='o',[0x19]='p',[0x1A]='[',[0x1B]=']',
    [0x1E]='a',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',[0x24]='j',[0x25]='k',
    [0x26]='l',[0x27]=';',[0x28]='\'',[0x29]='`',
    [0x2C]='z',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',[0x32]='m',[0x33]=',',
    [0x34]='.',[0x35]='/',[0x39]=' ', [0x1C]='\n', [0x0E]='\b', [0x0F]='\t'
};

const char SCANCODE_SHIFT_INIT_QWERTY[128] = {
    [0x02]='!',[0x03]='@',[0x04]='#',[0x05]='$',[0x06]='%',[0x07]='^',[0x08]='&',[0x09]='*',
    [0x0A]='(',[0x0B]=')',[0x0C]='_',[0x0D]='+',
    [0x10]='Q',[0x11]='W',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Y',[0x16]='U',[0x17]='I',
    [0x18]='O',[0x19]='P',[0x1A]='{',[0x1B]='}',
    [0x1E]='A',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',[0x23]='H',[0x24]='J',[0x25]='K',
    [0x26]='L',[0x27]=':',[0x28]='"',[0x29]='~',
    [0x2C]='Z',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',[0x32]='M',[0x33]='<',
    [0x34]='>',[0x35]='?',[0x39]=' ', [0x1C]='\n', [0x0E]='\b', [0x0F]='\t'
};

const char SCANCODE_INIT_AZERTY[128] = {
    [0x02]='&',[0x03]=0x82, /* é */ [0x04]='"',[0x05]='\'',[0x06]='(',
    [0x07]='-',[0x08]=0x8A, /* è */ [0x09]='_',[0x0A]=0x87, /* ç */
    [0x0B]=0x85, /* à */ [0x0C]=')',[0x0D]='=',
    [0x10]='a',[0x11]='z',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='y',
    [0x16]='u',[0x17]='i',[0x18]='o',[0x19]='p',[0x1A]='^',[0x1B]='$',
    [0x1E]='q',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',
    [0x24]='j',[0x25]='k',[0x26]='l',[0x27]='m',[0x28]=0x97, /* ù */
    [0x29]=0xFD, /* ² n'existe pas : approximé par carré supérieur */
    [0x2C]='w',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',
    [0x32]=',',[0x33]=';',[0x34]=':',[0x35]='!',
    [0x39]=' ', [0x1C]='\n', [0x0E]='\b', [0x0F]='\t'
};

const char SCANCODE_SHIFT_INIT_AZERTY[128] = {
    [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',[0x07]='6',
    [0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',[0x0C]=0xF8, /* ° */
    [0x0D]='+',
    [0x10]='A',[0x11]='Z',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Y',
    [0x16]='U',[0x17]='I',[0x18]='O',[0x19]='P',[0x1A]=0x8B, /* ¨ */
    [0x1B]='*',[0x1E]='Q',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',
    [0x23]='H',[0x24]='J',[0x25]='K',[0x26]='L',[0x27]='M',[0x28]='%',
    [0x29]=' ',
    [0x2C]='W',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',
    [0x32]='?',[0x33]='.',[0x34]='/',[0x35]=0x15, /* § */
    [0x39]=' ', [0x1C]='\n', [0x0E]='\b', [0x0F]='\t'
};

keyboard_t keyboard = {
    .shift_on = false, .alt_on = false,
    .scancode = 0,
    .pressed = false
};

void init_keyboard(void) {
    // Register keyboard PS/2 driver
    keyboard.SC_SHIFT = SCANCODE_SHIFT_INIT_QWERTY;
    keyboard.SC_US = SCANCODE_INIT_QWERTY;
    irq_install_handler(1, &keyboard_handler);
}

void keyboard_handler(int_regs_t *regs)
{
	(void)regs;
	int len;
	keyboard.scancode = inb(0x60) & 0x7F;
	keyboard.pressed = !(inb(0x60) & 0x80);
	switch (keyboard.scancode) {
	    case SC_LSHIFT:
		case SC_RSHIFT:
            keyboard.shift_on = keyboard.pressed;
            break;
        case SC_ALT:
            keyboard.alt_on = keyboard.pressed;
            break;
        default:
            break;
	}
	if (keyboard.pressed)
	{
		if (keyboard.alt_on && (keyboard.scancode - 1 >= 1 && keyboard.scancode - 1 <= 4))
			change_screen(keyboard.scancode - 2);
		else if (keyboard.alt_on && keyboard.SC_US[keyboard.scancode] == 'k') {
		    if (keyboard.SC_US[0x02] == '1')
				change_kb_layout(SCANCODE_INIT_AZERTY, SCANCODE_SHIFT_INIT_AZERTY);
			else
                change_kb_layout(SCANCODE_INIT_QWERTY, SCANCODE_SHIFT_INIT_QWERTY);
		}
		else if (!check_echappement(keyboard.SC_US[keyboard.scancode]) && keyboard.SC_US[keyboard.scancode])
		{
			vga_putchar(keyboard.shift_on ? keyboard.SC_SHIFT[keyboard.scancode] : keyboard.SC_US[keyboard.scancode]);
			len = ft_strlen(terminal.CMD_BUFFER[terminal.screen]);
			if (len < VGA_WIDTH)
					terminal.CMD_BUFFER[terminal.screen][len] = keyboard.shift_on ? keyboard.SC_SHIFT[keyboard.scancode] : keyboard.SC_US[keyboard.scancode];
			else
			{
				ft_bzero(terminal.CMD_BUFFER[terminal.screen], VGA_WIDTH);
				len = 0;
			}
		}
	}
}

void change_kb_layout(const char *layout, const char *layout_shift) {
    keyboard.SC_US = layout;
    keyboard.SC_SHIFT = layout_shift;
}
