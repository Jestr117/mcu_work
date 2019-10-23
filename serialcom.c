#include <xc.h>
#include <pic18f4321.h>
#include <stddef.h>

unsigned char input();
void initialization();
void output(unsigned char);
void calculate(char);

unsigned char decoder[10] = {
    0x7E, 0x30, 
    0x6D, 0x79,
    0x33, 0x5B,
    0x5F, 0x70,
    0x7F, 0x73 };
unsigned char message;

void initialization()
{
    TXSTA = 0x24;
    SPBRG = 103;
    ADCON1 = 0x0F;

    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 0;
    TRISA = 0;
    TRISD = 0;
    RCSTA =0x90;
}

unsigned char input()
{
    if(PIRbits.RC1IF)
    {
        return RCREG1;
    }
    else
    {
        return 0;
    }
    
}

void output(unsigned char input)
{
    while(!PIRbits.TX1IF);
    TXREG1 = input;
}

void calculate(char data)
{
    
    unsigned char ten, one;
    ten = data / 10;
    one = data  % 10;
    PORTA = decoder[ten];
    PORTD = decoder[one];
}

void main(void)
{
    initialization();
    while(1)
    {
         message  =  input();         //receive data from serial
        calculate(message);
        output(72); // send data to serial comm
      
    }
   
    return;
}