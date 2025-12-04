
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

void design();
void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line, unsigned char color);

void k_main() 
{
	k_clear_screen();
    design();
	k_printf("Hello, world! Welcome to my kernel.", 1, WHITE_BG_BLACK_TXT);
    k_printf("This is NOT a VIRUS, trust me!", 2, LIGHT_CYAN_TXT);
};

void design()
{
    int i = 0;

    char test[11] = "";

    while(i < 10){
        test[i] = '-';
        i++;
    }
    test[10] = '\0';

    k_printf(test, 0, LIGHT_WHITE_TXT);
};


void k_clear_screen()
{
	char *vidmem = (char *) 0xb8000;
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
unsigned int k_printf(char *message, unsigned int line, unsigned char color)
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;

	i=(line*80*2);

	while(*message!=0)
	{
		if(*message=='\n')
		{
			line++;
			i=(line*80*2);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]=color;
			i++;
		};
	};

	return(1);
}