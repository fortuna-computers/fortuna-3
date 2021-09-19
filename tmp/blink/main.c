#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

int _write(int file, char *ptr, int len);

static void gpio_setup()
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);          // enable GPIO clock
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);

    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);   // set GPIO pin as output push-pull

	// UART TX pino PA9 (GPIO_USART1_TX)
	gpio_set_mode(
		GPIOA,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		GPIO_USART1_TX);

	usart_set_baudrate(USART1, 38400);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_enable(USART1);
}

static inline void uart_putc (char ch)
{
	usart_send_blocking(USART1, ch);
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(USART1, '\r');
			}
			usart_send_blocking(USART1, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}

int main()
{
    int i;

    gpio_setup();

	printf("\nHello world!\n");

	char c = 'A';
    while (1) {
        gpio_toggle(GPIOC, GPIO13);   // LED on/off
        for (i = 0; i < 800000; i++)  // busy wait
            __asm__("nop");

        uart_putc(c++);	
        if (c > 'Z')
            c = 'A';
    }

    return 0;
}
