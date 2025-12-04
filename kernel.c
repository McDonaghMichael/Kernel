
#define BLACK_TXT 0X00
#define BLUE_TXT 0X01
#define GREEN_TXT 0X02
#define CYAN_TXT 0X03
#define RED_TXT 0X04
#define MAGENTA_TXT 0X05
#define BROWN_TXT 0x06
#define LIGHT_GRAY_TXT 0x07
#define DARK_GRAY_TXT 0x08
#define LIGHT_BLUE_TXT 0x09
#define LIGHT_GREEN_TXT 0x0A
#define LIGHT_CYAN_TXT 0x0B
#define LIGHT_RED_TXT 0x0C
#define LIGHT_MAGENTA_TXT 0x0D
#define LIGHT_YELLOW_TXT 0x0E
#define LIGHT_WHITE_TXT 0x0F

#define BLACK_BG_WHITE_TXT 0X0F
#define WHITE_BG_BLACK_TXT 0XF0
#define BLUE_BG_WHITE_TXT 0x1F
// qemu-system-i386 -cdrom iso/os.iso

int cursor_x = 0;
int cursor_y = 0;

#define VIDEO_MEMORY ((char *) 0xb8000)


void k_clear_screen();
void k_printf(char* str, unsigned char color);
void print_char(char c, unsigned char color);

void k_main() 
{
	k_clear_screen();

	k_printf("Hello, world! Welcome to my kernel.\n",WHITE_BG_BLACK_TXT);
    k_printf("This is NOT a VIRUS, trust me!", LIGHT_CYAN_TXT);
};



void k_clear_screen()
{
	char *vidmem = VIDEO_MEMORY;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=MAGENTA_TXT;
		i++;
	};
};

/* k_printf : the message and the line # */
void k_printf(char* str, unsigned char color)
{
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i], color);
        i++;
    }
};



void print_char(char c, unsigned char color)
{

    if(c == '\n'){
        cursor_x = 0;
        cursor_y++;
        return;
    }

    int offset = (cursor_y * 80 + cursor_x) * 2;
    VIDEO_MEMORY[offset] = c;
    VIDEO_MEMORY[offset + 1] = color;

    cursor_x++;

    if(cursor_x >= 80){
        cursor_x = 0;
        cursor_y++;
    }

};