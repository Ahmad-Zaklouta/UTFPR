// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Controle do motor de passo
// Giovanni de Rosso Unruh
// Anderson Kmetiuk

#include <stdint.h>

#include "tm4c1294ncpdt.h"
																													//.Q.P.N.M.L.K.J.H.G.F.E.D.C.B.A.
																													//.0.0.0.1.1.1.1.0.0.0.0.0.0.0.0.
#define GPIO_PORTEFJKLMP (0x0F00)  //INICIALIZA PORTA	

void SysTick_Wait1ms(uint32_t delay);
// -------------------------------------------------------------------------------
// Fun��o GPIO_Init
// Inicializa os portas
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: N�o tem
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R |= GPIO_PORTEFJKLMP;
	//1b.   ap�s isso verificar no PRGPIO se a porta est� pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTEFJKLMP) ) != (GPIO_PORTEFJKLMP) ){};
	
	// 2. Limpar o AMSEL para desabilitar a anal�gica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTK_AMSEL_R = 0x00;
	GPIO_PORTL_AMSEL_R = 0x00;
	GPIO_PORTM_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTK_PCTL_R = 0x00;
	GPIO_PORTL_PCTL_R = 0x00;
	GPIO_PORTM_PCTL_R = 0x00;
		
	// 4. DIR para 0 se for entrada, 1 se for sa�da
	GPIO_PORTJ_AHB_DIR_R = 0x00; //input em 1,0
	GPIO_PORTK_DIR_R = 0xFF;		//output 7,6,5,4,3,2,1,0
	GPIO_PORTL_DIR_R = 0x00;		//input 3,2,1,0
	GPIO_PORTM_DIR_R = 0xF7;		//output 7,6,5,4,2,1,0
	
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun��o alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTK_AFSEL_R = 0x00;
	GPIO_PORTL_AFSEL_R = 0x00;
	GPIO_PORTM_AFSEL_R = 0x00;
	
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x03;	//input em 1,0
	GPIO_PORTK_DEN_R = 0xFF;	//pk 7,6,5,4,3,2,1,0
	GPIO_PORTL_DEN_R = 0x0F;	//input 3,2,1,0
	GPIO_PORTM_DEN_R = 0xF7;	//pm 7,6,5,4,2,1,0
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03;   //PINO 1,0	
	GPIO_PORTL_PUR_R = 0x0F;			//PINO 3,2,1,0

}	
// -------------------------------------------------------------------------------
// Fun��o PortJ_Input
// L� os valores de entrada do port J
uint32_t PortJ_Input(void)
{
	return GPIO_PORTJ_AHB_DATA_R;
}
// -------------------------------------------------------------------------------
// Fun��o PortL_Input
// L� os valores de entrada do port L
uint32_t PortL_Input(void)
{
	return GPIO_PORTL_DATA_R;
}
// -------------------------------------------------------------------------------
// Fun��o PortK_Output
void PortK_Output(uint32_t valor)
{
		uint32_t temp;
    //para uma escrita amig�vel nos bits 0 e 1
    temp = GPIO_PORTK_DATA_R & 0x00;	
    //agora vamos fazer o OR com o valor recebido na fun��o
    temp = temp | valor;
    GPIO_PORTK_DATA_R = valor; 
}
// -------------------------------------------------------------------------------
// Fun��o PortM_Output
void PortM_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amig�vel nos bits 0 e 1
    temp = GPIO_PORTM_DATA_R & 0x08;
    //agora vamos fazer o OR com o valor recebido na fun��o
    temp = temp | valor;
    GPIO_PORTM_DATA_R = temp; 
}

// -------------------------------------------------------------------------------
void Config_Interrupt_J(void)
{
		int temp;
	
		temp = 0x00 | GPIO_PORTJ_AHB_IM_R;
		GPIO_PORTJ_AHB_IM_R = temp;
		
		temp = 0x00 | GPIO_PORTJ_AHB_IS_R;
		GPIO_PORTJ_AHB_IS_R = temp;
		
		temp = 0x00;
		temp = temp | GPIO_PORTJ_AHB_IBE_R;
		GPIO_PORTJ_AHB_IBE_R = temp;
		
		temp = 0x00;
		temp = temp | GPIO_PORTJ_AHB_IEV_R;
		GPIO_PORTJ_AHB_IEV_R = temp;
	
		temp = 0x01;
		temp = temp | GPIO_PORTJ_AHB_ICR_R;
		GPIO_PORTJ_AHB_ICR_R = temp;	
	
		temp = 0x01;
		temp = temp | GPIO_PORTJ_AHB_IM_R;
		GPIO_PORTJ_AHB_IM_R = temp;
	
		temp = 0xA0000000;
		temp = temp | NVIC_PRI12_R;
		NVIC_PRI12_R = temp;
		
		temp = 0x00080000;
		temp = temp | NVIC_EN1_R;
		NVIC_EN1_R = temp;			
}
