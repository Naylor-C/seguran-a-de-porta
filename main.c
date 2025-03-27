#include <xc.h>
#include <stdio.h>
#include <string.h>

// Configurações do PIC (ajuste conforme seu modelo)
#pragma config FOSC = HS        // Oscilador
#pragma config WDTE = OFF       // Watchdog desligado
#pragma config PWRTE = ON       // Power-up Timer
#pragma config BOREN = ON       // Brown-out Reset

#define _XTAL_FREQ 20000000     // Frequência do cristal

// Pinos (ajuste conforme seu hardware)
#define RELAY_PIN RD0
#define TX_PIN RC6
#define RX_PIN RC7

// Protótipos
void UART_Init(unsigned long baud);
void UART_Write(char data);
char UART_Read();

void main(void) {
    TRISD0 = 0;         // Configura RD0 como saída (rele)
    UART_Init(9600);     // Inicializa UART para Bluetooth
    
    while(1) {
        if(UART_Data_Ready()) {  // Se recebeu dado
            char cmd = UART_Read();
            
            if(cmd == 'A') {     // Comando para abrir
                RELAY_PIN = 1;
                __delay_ms(1000); // Tempo de ativação
                RELAY_PIN = 0;
                UART_Write('1');  // Confirma abertura
            }
            else if(cmd == 'F') { // Comando para fechar
                RELAY_PIN = 1;
                __delay_ms(1000);
                RELAY_PIN = 0;
                UART_Write('0');  // Confirma fechamento
            }
        }
    }
}

void UART_Init(unsigned long baud) {
    // Configuração dos registros UART (ajuste para seu PIC)
    SPBRG = ((_XTAL_FREQ/16)/baud) - 1;
    TXSTAbits.BRGH = 1;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
    TRISC6 = 0;
    TRISC7 = 1;
}

void UART_Write(char data) {
    while(!TXSTAbits.TRMT);
    TXREG = data;
}

char UART_Read() {
    while(!PIR1bits.RCIF);
    return RCREG;
}
