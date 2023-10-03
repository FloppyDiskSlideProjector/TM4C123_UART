#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))
#define RCGCUART (*((volatile unsigned long *) 0x400FE618))

#define GPIO_PORTA_LOCK (*((volatile unsigned long *) 0x40004520))
#define GPIO_PORTA_CR (*((volatile unsigned long *) 0x40004524))
#define GPIO_PORTA_PDR (*((volatile unsigned long *) 0x40004514))

#define GPIO_PORTA_EN (*((volatile unsigned long *) 0x4000451C))
#define GPIO_PORTA_DIR (*((volatile unsigned long *) 0x40004400))
#define GPIO_PORTA_DATA (*((volatile unsigned long *) 0x40004030))

#define GPIOAFSEL (*((volatile unsigned long *) 0x40004420))

#define UARTDR (*((volatile unsigned long *) 0x4000C000))
#define UARTIBRD (*((volatile unsigned long *) 0x4000C024))
#define UARTFBRD (*((volatile unsigned long *) 0x4000C028))
#define UARTCTL (*((volatile unsigned long *) 0x4000C030))
#define UARTLCRH (*((volatile unsigned long *) 0x4000C02C))


void stateP(void);
void stateN(void);
char UART_Rx();
void UART_Tx(char data);


int main(void)
{
    //setup


    RCGCUART = 0x01;
    RCGCGPIO = 0x01;


    GPIOAFSEL = 0x01;
    UARTCTL = 1;
    UARTIBRD = 104;
    UARTFBRD = 11;
    UARTCTL = 0x201;
    UARTLCRH = 0x60;


    //clock, lock, cr, pur, enable, direction

    GPIO_PORTA_LOCK = 0x4C4F434B;
    GPIO_PORTA_CR = 0x01;
    GPIO_PORTA_PDR = 0x08;

    //GPIO_PORTA_EN = 0x0C;
    GPIO_PORTA_EN = 0x0F;
    GPIO_PORTA_DIR = 0x04;


    char state = 'n';

    //loop
    while(1){
        if(UART_Rx()=='p'){
            state = 'p';
        }else if(UART_Rx()=='n'){
            state = 'n';
        }

        if(state == 'p'){
            stateP();
        }else if(state == 'n'){
            stateN();
        }
    }
}

void stateP(void){
    if(GPIO_PORTA_DATA & 0x08){
        GPIO_PORTA_DATA = 0x04;
    }else{
        GPIO_PORTA_DATA = 0x00;
    }
}

void stateN(void){
    if(GPIO_PORTA_DATA & 0x08){
            GPIO_PORTA_DATA = 0x00;
        }else{
            GPIO_PORTA_DATA = 0x04;
        }
}

char UART_Rx(void){
    char data;
    //while((UARTFBRD & 0x10) != 0);
    data = UARTDR;
    return ((unsigned char) data);
}

void UART_Tx(char data){
    UARTDR = data;
}

