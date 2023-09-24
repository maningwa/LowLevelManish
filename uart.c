// Define the UART register offsets
#define UART_DR_OFFSET 0x00
#define UART_FR_OFFSET 0x18
#define UART_IBRD_OFFSET 0x24
#define UART_FBRD_OFFSET 0x28
#define UART_LCRH_OFFSET 0x2C
#define UART_CR_OFFSET 0x30

// Define the UART flags
#define UART_FR_TXFF (1 << 5) // Transmit FIFO full
#define UART_FR_RXFE (1 << 4) // Receive FIFO empty

// Define the system clock frequency
#define SYSTEM_CLOCK_FREQ 16000000 // 16 MHz

// Define the baud rate
#define BAUD_RATE 9600

void uart_init()
{
    // Enable UART peripheral clock
    *((volatile unsigned int*)0x400FE618) |= (1 << 0);

    // Disable UART
    *((volatile unsigned int*)0x4000C030) &= ~(1 << 0);

    // Configure the baud rate
    unsigned int baud_divisor = ((SYSTEM_CLOCK_FREQ / (16 * BAUD_RATE)) + 0.5);
    *((volatile unsigned int*)(0x4000C000 + UART_IBRD_OFFSET)) = baud_divisor / 65536;
    *((volatile unsigned int*)(0x4000C000 + UART_FBRD_OFFSET)) = baud_divisor % 65536;

    // Configure the UART line control
    *((volatile unsigned int*)(0x4000C000 + UART_LCRH_OFFSET)) = (1 << 4) | (1 << 5);

    // Enable UART
    *((volatile unsigned int*)0x4000C030) |= (1 << 0);
}

void uart_putc(char c)
{
    // Wait for the transmit FIFO to become empty
    while (*((volatile unsigned int*)(0x4000C000 + UART_FR_OFFSET)) & UART_FR_TXFF);

    // Transmit the character
    *((volatile unsigned int*)(0x4000C000 + UART_DR_OFFSET)) = c;
}

char uart_getc()
{
    // Wait for the receive FIFO to become non-empty
    while (*((volatile unsigned int*)(0x4000C000 + UART_FR_OFFSET)) & UART_FR_RXFE);

    // Receive the character
    return (char)(*((volatile unsigned int*)(0x4000C000 + UART_DR_OFFSET)));
}

int main()
{
    // Initialize the UART
    uart_init();

    // Send a test message
    char* message = "Hello, world!\n";
    while (*message) {
        uart_putc(*message++);
    }

    // Receive characters and echo them back
    while (1) {
        char c = uart_getc();
        uart_putc(c);
    }

    return 0;
}
