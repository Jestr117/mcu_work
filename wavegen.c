#include <xc.h>
#include <p18f4321.h>
#pragma config OSC = INTIO2
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOR = OFF

unsigned char ctrl[3], data[3];
unsigned char SPI(unsigned char);

void initialization()
{
    for(unsigned char i = 0; i < 3; i++){
        data[i] = 0;
        ctrl[0] = 1;
        ctrl[1] = 2;
        ctrl[2] = 4;
        OSCCONbits.IRCF1 = 1;
        OSCCONbits.IRCF0 = 1;  // Select the 8MHz freq
    }
    // Port Configurations
    // all PORTC bits are output, except PC4, SDI, input
    TRISC = 0x10;
    // TRISCbits.TRISC4 = 1; // except SDI input
    ADCON1 = 0X0F
    // configure SPI
    SSPSTAT = 0x40;
    SSPADD = 0;
    // enable SPI, idle low, and Fosc / 16
    SSPCON1 = 0x21;
    // configure TMR0
    // Timer0, 16-bit mode, prescaler = 1/4
    T0CON = 0x01;
    TMR0H = 0x0B;
    TMR0L = 0xDC;
    T0CONBbits.TMR0ON = 1; // turn on the timer
    INTCONbits.TMR0IF = 0; // clear the flag
}

unsigned char SPI(unsigned char message)
{
    SSPBUF = message;
    while(!SSPSTATbits.BF); // wait for transmission completion
    return SSPBUF;
}

unsigned char dataSPI;
unsigned int select;
void output()
{   
    // select waveform
    select = 2;
    switch(select){
        case 0:
            PORTCbits.RC2 = 0;
            dataSPI = 0x30 + (data[0]>>4);
            SPI(dataSPI);
            dataSPI = (data[0]<<4);
            SPI(dataSPI);
            PORTCbits.RC2 = 1;
            break;
        case 1:
            PORTCbits.RC2 = 0;
            dataSPI = 0x30 + (data[1]>>4);
            SPI(dataSPI);
            dataSPI = (data[1]<<4)
            SPI(dataSPI);
            PORTCbits.RC2 = 1;
            break;
        case 2:
            PORTCbits.RC2 = 0;
            dataSPI = 0x30 + (data[2]>>4);
            SPI(dataSPI);
            dataSPI = (data[2]<<4);
            SPI(dataSPI)
            PORTCbits.RC2 = 1;
            break;
    }
}

void calculate()
{
  unsigned char sineTable[64] = {128 ,​

    140 , 152 , 165 , 176 , 188 , 199 , 209 , 218 ,​

    226 , 234 , 240 , 246 , 250 , 253 , 255 , 255 ,​

    255 , 253 , 250 , 246 , 240 , 234 , 226 , 218 ,​

    209 , 199 , 188 , 176 , 165 , 152 , 140 , 128 ,​

    115 , 103 , 90 , 79 , 67 , 56 , 46 , 37 ,​

    29 , 21 , 15 , 9 , 5 , 2 , 0 , 0 ,​

    0 , 2 , 5 , 9 , 15 , 21 , 29 , 37 ,​

    46 , 56 , 67 , 79 , 90 , 103 , 115}; ​

//channel A sawtooth ​

data[0]=data[0]+1;   //increment th​

//channel B triangular​

if(data[0] >=128) 
    data[1] = data[1] -2;​
else 
    data[1] = data[1] +2;​

//channel C ​
data[2] = sineTable[data[0]%64];

}

void input()
{}

void main()
{
    initialization();
    while(1)
    {
        input();
        calculate();
        output();
    }
}