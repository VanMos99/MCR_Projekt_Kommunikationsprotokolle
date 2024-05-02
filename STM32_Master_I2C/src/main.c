/*
Projektarbeit im Fach Mikrokontrollertechnik.
Gruppe 1: Jan Skvortsov
Der Quellcode stellt eine Verbindung über I2C Bus her. 
Der Quellcode ist für den Meister erstellt

*/



#include <stm32f4xx.h>


#include<delay.h>
#include<exti.h>
#include<gpio.h>
#include<i2c.h>
#include<timer.h>
#include<uart.h>


#ifdef USE_UART_PRINTF
#include <stdio.h>
int _write(int fd, char *ptr, int len)
{
    (void)fd;
    if (fd == 1)
        uart_write(USART2, (uint8_t *)ptr, (uint32_t)len);
    return -1;
}
#define UART_LOG(msg...) printf(msg);
#else
#define UART_LOG(msg...) ;
#endif

//***** Daten *****
uint8_t ALLE_AUS= 0b00;
uint8_t GRUEN_EIN= 0b01;
uint8_t GRUEN_AUS= 0b10;
uint8_t GELB_EIN =0b11;
uint8_t GELB_AUS =0b100;
uint8_t ROT_EIN =0b101;
uint8_t ROT_AUS =0b110;
uint8_t ALLE_AN =0b111;

#define MASTER_ADDR 0x15
#define SlAVE_ADDR 0x12
char DATA =0; 


//***** Test LEDs an dem Master *****
//***** A5 *****
#define LED_ONE_ON  gpio_set_pin(GPIOA,5);
#define LED_ONE_OFF gpio_reset_pin(GPIOA,5);
//***** B2 *****
#define LED_TWO_ON  gpio_set_pin(GPIOB,2);
#define LED_TWO_OFF gpio_reset_pin(GPIOB,2);
//***** C4 *****
#define LED_THREE_ON gpio_set_pin(GPIOC,4);
#define LED_THREE_OFF gpio_reset_pin(GPIOC,4);

char value1 = 1;
char value2 =1;


//***** Flags und Counters *****
int counter_one=0;
int counter_two=0;
int counter_three=0;

bool butt_one=0;
bool butt_one_flag=0;

bool butt_two=0;
bool butt_two_flag=0;

bool butt_three=0;
bool butt_three_flag=0;

bool led_flag=0;

//***** Timer Variablen *****
uint32_t millis();
volatile uint32_t msTicks = 0;

uint32_t timer_butt_one=0;
uint32_t timer_butt_two=0;
uint32_t timer_butt_three=0;

uint32_t timer1=0;
uint32_t timer2=0;
uint32_t timer3=0;



//***** Prototypen *****
void enable_all_clock();
void i2c_Init();


int main(){

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000); // SysTick = 1ms
   
    enable_all_clock();
    i2c_Init();
    uart_setup(USART2, 115200);


     //***** Output *****
    gpio_set_mode(GPIOA,5,OUTPUT);
    gpio_set_mode(GPIOB,2,OUTPUT);
    gpio_set_mode(GPIOC,4,OUTPUT);
    
    

    //***** Input *****
    //***** Taster PC10, PC11, PC12 *****
    gpio_set_mode(GPIOC,10,INPUT);
    gpio_set_mode(GPIOC,11,INPUT);
    gpio_set_mode(GPIOC,12,INPUT);


    UART_LOG("\033[36m Gruppennummer: 1 \033[0m \n");
    UART_LOG("\033[36m Der Mikrokontroller ist der Master! \033[0m \n");


    while (1)
    {   
        
        butt_one=!gpio_read_pin_state(GPIOC,10);
        butt_two=!gpio_read_pin_state(GPIOC,12);
        butt_three=!gpio_read_pin_state(GPIOC,11);
        

        //***** Taster_1 mit Prellungschutz*****
        if (butt_one==1 && butt_one_flag==0 && millis()-timer_butt_one>50 && counter_one==0){
            timer_butt_one=millis();
            butt_one_flag=1;
            LED_ONE_ON;
            i2c_master_transmit(I2C1,SlAVE_ADDR,&ROT_EIN,value1);
            UART_LOG("\033[34m DEBUG: Taste 1 gedrückt! \033[0m \n");
            UART_LOG("\033[35m I2C: Master_transmit \033[0m \n");
            UART_LOG("\033[31m LED: Rot Ein \033[0m \n");
            counter_one=counter_one+1;
            }

        if (butt_one==0 && butt_one_flag==1 && millis()-timer_butt_one>50){
            timer_butt_one=millis();
            butt_one_flag=0;
            UART_LOG("\033[34m DEBUG: Taste 1 losgelassen! \033[0m \n"); 
            }

        if (butt_one==1 && butt_one_flag==0 && millis()-timer_butt_one>50 && counter_one==1){
            timer_butt_one=millis();
            butt_one_flag=1;
            LED_ONE_OFF;
            i2c_master_transmit(I2C1,SlAVE_ADDR,&ROT_AUS,value1);
            UART_LOG("\033[34m DEBUG: Taste 1 gedrückt 2 mal! \033[0m \n");
            UART_LOG("\033[35m I2C: Master_transmit \033[0m \n");
            UART_LOG("\033[31m LED: Rot Aus \033[0m \n");
            counter_one=counter_one-1;
            }

        //***** Taster_2 *****
        if (butt_two ==1 && butt_two_flag==0 && millis()-timer_butt_two>50 && counter_two==0){
            timer_butt_two=millis();
            butt_two_flag=1;
            LED_TWO_ON;
            i2c_master_transmit(I2C1,SlAVE_ADDR,&GELB_EIN,value1);
            UART_LOG("\033[34m DEBUG: Taste 2 gedrückt! \033[0m \n");
            UART_LOG("\033[35m I2C: Master_transmit \033[0m \n");
            UART_LOG("\033[33m LED: Gelb Ein \033[0m \n");
            counter_two=counter_two+1;
            }

        if (butt_two==0 && butt_two_flag==1 && millis()-timer_butt_two>50){
            timer_butt_two=millis();
            butt_two_flag=0;
            UART_LOG("\033[34m DEBUG: Taste 2 losgelassen! \033[0m \n");     
            }

        if (butt_two ==1 && butt_two_flag==0 && millis()-timer_butt_two>50 && counter_two==1){
            timer_butt_two=millis();
            butt_two_flag=1;
            LED_TWO_OFF;
            i2c_master_transmit(I2C1,SlAVE_ADDR,&GELB_AUS,value1);
            UART_LOG("\033[34m DEBUG: Taste 2 gedrückt 2 mal! \033[0m \n");
            UART_LOG("\033[35m I2C: Master_transmit \033[0m \n");
            UART_LOG("\033[33m LED: Gelb Aus \033[0m \n");
            counter_two=counter_two-1;
            }
        //***** Taster_3 *****

        if (butt_three==1 && butt_three_flag==0 && millis()-timer_butt_three>50 && counter_three==0){
            timer_butt_three=millis();
            butt_three_flag=1;
            LED_THREE_ON;
            i2c_master_transmit(I2C1,SlAVE_ADDR,&GRUEN_EIN,value1);
            UART_LOG("\033[34m DEBUG: Taste 3 gedrückt! \033[0m \n");
            UART_LOG("\033[35m I2C: Master_transmit \033[0m \n");
            UART_LOG("\033[32m LED: Grün Ein \033[0m \n");
            counter_three=counter_three+1;
            }

        if (butt_three==0 && butt_three_flag==1 && millis()-timer_butt_three>50){
            timer_butt_three=millis();
            butt_three_flag=0;
            UART_LOG("\033[34m DEBUG: Taste 3 losgelassen! \033[0m \n");     
            }

        if (butt_three ==1 && butt_three_flag==0 && millis()-timer_butt_three>50 && counter_three==1){
            timer_butt_three=millis();
            butt_three_flag=1;
            LED_THREE_OFF;
            i2c_master_transmit(I2C1,SlAVE_ADDR,&GRUEN_AUS,value1);
            UART_LOG("\033[34m DEBUG: Taste 3 gedrückt 2 mal! \033[0m \n");
            UART_LOG("\033[35m I2C: Master_transmit \033[0m \n");
            UART_LOG("\033[32m LED: Grün Aus \033[0m \n");
            counter_three=counter_three-1;
            }
           
            /* Blink für Slave
                if (millis()-timer1 >500){
                    timer1=millis();
                    led_flag=!led_flag;
                    gpio_write_pin_state(GPIOA,5,led_flag);
                    }
            
            
            if(i2c_master_receive(I2C1,SlAVE_ADDR,&ROT_EIN,value2)){
                UART_LOG("\033[32m DEBUG: Habe was empfangen\033[0m \n",&ROT_EIN);
            }
            
            */


          


       }
        
    }
    


 void enable_all_clock(){
        gpio_enable_port_clock(GPIOA);
        gpio_enable_port_clock(GPIOB);
        gpio_enable_port_clock(GPIOC);
        gpio_enable_port_clock(GPIOD);
        gpio_enable_port_clock(GPIOE);
        gpio_enable_port_clock(GPIOH);
    } 

 void i2c_Init(){
        i2c_set_own_address(I2C1,MASTER_ADDR);
        i2c_master_setup(I2C1,I2C_STANDARD);
}   

void SysTick_Handler(void)
{
 ++msTicks;
}

 uint32_t millis(){
    return msTicks;
 }