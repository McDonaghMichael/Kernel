// Add these at the top of your file (after the includes)
static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    asm volatile("inb %1, %0" 
                 : "=a"(data) 
                 : "Nd"(port));
    return data;
}

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1"
                 :
                 : "a"(data), "Nd"(port));
}


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

#define KEYBOARD_DATA_PORT 0x60 // Memory address for reading keyboard data
#define KEYBOARD_STATUS_PORT 0x64 // Memory address for the keyboards controller status register

void k_clear_screen();
void k_printf(char* str, unsigned char color);
void print_char(char c, unsigned char color);
unsigned char k_fetch();
unsigned char k_getc();

/**
Function that returns the length of a string
 */
int strlen(const char* str) {

    // Variable starting with length 0
    int len = 0;

    // While the index exists, increment
    while(str[len]) len++;

    return len;
}

/**
The function below is for comparing two strings,
 */
int strcmp(const char* s1, const char* s2) {

    // While the first string is NOT NULL and the second string matches the first
    // The loop will stop when we hit a null terminator or the chars no longer equal
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}


unsigned char k_fetch(void) {

    unsigned char status;
    
    // Wait for keyboard to have data
    while(1) {
        status = inb(KEYBOARD_STATUS_PORT);

        /**
        This is first time ive seen bitwise operation, we are checking if 0x01,
        which in binary is 0 0 0 0 0 0 0 1, compares each bit of status to these bits,
        so if the 1 bit in status in binary form is also 1, there is data, if so, output it to
        the data port buffer
         */
        if(status & 0x01) { 
            return inb(KEYBOARD_DATA_PORT);
        }
    }
}

unsigned char k_getc(void) {

    unsigned char scancode = k_fetch();


    /**
    Here we check if the keyboard input is released! And if it is released, return
    nothing! As if we didnt do this, it could go straight to checking the value in
    our list and can be issue!!
     */
    if(scancode & 0x80) {
        return 0;
    }
    
    switch(scancode) {
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case 0x0C: return '-';
        case 0x0D: return '=';
        case 0x0E: return '\b'; 
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1A: return '[';
        case 0x1B: return ']';
        case 0x1C: return '\n';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x27: return ';';
        case 0x28: return '\'';
        case 0x29: return '`';
        case 0x2B: return '\\';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        case 0x33: return ',';
        case 0x34: return '.';
        case 0x35: return '/';
        case 0x39: return ' ';
        
        default: return 0;
    }
}


void k_clear_screen()
{
	char *vidmem = VIDEO_MEMORY;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=LIGHT_GRAY_TXT;
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

void k_shell(void) {

    char input[256]; // Here we will store what the user types

    int index = 0;
    
    k_printf("\n> ", LIGHT_RED_TXT);
    
    while(1) { // Continuous input loop!

        unsigned char c = k_getc(); // Here is essentially checking for the input
        
        // If 0, either not in our ascii list or invalid, so skip
        if(c == 0) continue; 
        
        // Enter is pressed!
        if(c == '\n') {

            input[index] = '\0'; // Adds null terminator to string

            k_printf("\n", LIGHT_GRAY_TXT);
            
            // Here we can run our commands!
            if(strcmp(input, "help") == 0) {
                k_printf("\n\nAvailable commands:\n", CYAN_TXT);
                k_printf("  help    - Show this help\n", LIGHT_GRAY_TXT);
                k_printf("  clear   - Clear screen\n", LIGHT_GRAY_TXT);
                k_printf("  echo    - Print text\n", LIGHT_GRAY_TXT);
            } 
            else if(strcmp(input, "clear") == 0) {
                k_clear_screen();
                cursor_x = 0;
                cursor_y = 0;
            }
            else if(strlen(input) >= 4 && input[0] == 'e' && input[1] == 'c' && 
                    input[2] == 'h' && input[3] == 'o' && input[4] == ' ') {

                k_printf(&input[5], LIGHT_WHITE_TXT);
                k_printf("\n", 0x07);
            }
            else if(strlen(input) > 0) {
                k_printf("Unknown command: '", RED_TXT);
                k_printf(input, LIGHT_RED_TXT);
                k_printf("'\n", RED_TXT);
            }
            
            k_printf("> ", LIGHT_RED_TXT);
            index = 0;
        }
        else if(c == '\b') {
            // Backspace
            if(index > 0) {
                index--;
                cursor_x--;
                print_char(' ', LIGHT_RED_TXT);
                cursor_x--;
            }
        }
        else if(index < 255) {
            input[index++] = c;
            print_char(c, LIGHT_WHITE_TXT);
        }
    }
}

void k_main(void) {
    k_clear_screen();
    
    k_printf("========================================\n", LIGHT_GRAY_TXT);
    k_printf("        Kernel v0.0.1 - Booted!\n", LIGHT_CYAN_TXT);
    k_printf("========================================\n\n", LIGHT_GRAY_TXT);
    
    k_shell();
}