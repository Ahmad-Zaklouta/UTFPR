; main.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; 15/03/2018
; Este programa espera o usu�rio apertar a chave USR_SW1 e/ou a chave USR_SW2.
; Caso o usu�rio pressione a chave USR_SW1, acender� o LED2. Caso o usu�rio pressione 
; a chave USR_SW2, acender� o LED1. Caso as duas chaves sejam pressionadas, os dois 
; LEDs acendem.

; -------------------------------------------------------------------------------
        THUMB                        ; Instru��es do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declara��es EQU - Defines
;<NOME>         EQU <VALOR>
; ========================
; Defini��es de Valores
BIT0	EQU 2_0001
BIT1	EQU 2_0010

; -------------------------------------------------------------------------------
; �rea de Dados - Declara��es de vari�veis
		AREA  DATA, ALIGN=2
		; Se alguma vari�vel for chamada em outro arquivo
		;EXPORT  <var> [DATA,SIZE=<tam>]   ; Permite chamar a vari�vel <var> a 
		                                   ; partir de outro arquivo
;<var>	SPACE <tam>                        ; Declara uma vari�vel de nome <var>
                                           ; de <tam> bytes a partir da primeira 
                                           ; posi��o da RAM		

; -------------------------------------------------------------------------------
; �rea de C�digo - Tudo abaixo da diretiva a seguir ser� armazenado na mem�ria de 
;                  c�digo
        AREA    |.text|, CODE, READONLY, ALIGN=2

		; Se alguma fun��o do arquivo for chamada em outro arquivo	
        EXPORT Start                ; Permite chamar a fun��o Start a partir de 
			                        ; outro arquivo. No caso startup.s
									
		; Se chamar alguma fun��o externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; fun��o <func>
		IMPORT  GPIO_Init
        IMPORT  PortN_Output
        IMPORT  PortJ_Input

; -------------------------------------------------------------------------------
; Fun��o main()
Start  			
	BL GPIO_Init                 ;Chama a subrotina que inicializa os GPIO

MainLoop
	BL PortJ_Input				 ;Chama a subrotina que l� o estado das chaves e coloca o resultado em R0
Verifica_Nenhuma
	CMP	R0, #2_00000011			 ;Verifica se nenhuma chave est� pressionada
	BNE Verifica_SW1			 ;Se o teste viu que tem pelo menos alguma chave pressionada pula
	MOV R0, #0                   ;N�o acender nenhum LED
	BL PortN_Output			 	 ;Chamar a fun��o para n�o acender nenhum LED
	B MainLoop					 ;Se o teste viu que nenhuma chave est� pressionada, volta para o la�o principal
Verifica_SW1	
	CMP R0, #2_00000010			 ;Verifica se somente a chave SW1 est� pressionada
	BNE Verifica_SW2             ;Se o teste falhou, pula
	MOV R0, #BIT0				 ;Setar o par�metro de entrada da fun��o como o BIT0
	BL PortN_Output				 ;Chamar a fun��o para setar o LED1
	B MainLoop                   ;Volta para o la�o principal
Verifica_SW2	
	CMP R0, #2_00000001			 ;Verifica se somente a chave SW2 est� pressionada
	BNE Verifica_Ambas           ;Se o teste falhou, pula
	MOV R0, #BIT1				 ;Setar o par�metro de entrada da fun��o como o BIT2
	BL PortN_Output				 ;Chamar a fun��o para setar o LED2
	B MainLoop                   ;Volta para o la�o principal	
Verifica_Ambas
	CMP R0, #2_00000000			 ;Verifica se ambas as chaves est�o pressionadas
	BNE MainLoop          		 ;Se o teste falhou, pula
	MOV R0, #BIT0				 ;Setar o par�metro de entrada da fun��o como o BIT0
	ORR R0, #BIT1				 ;e o BIT1
	BL PortN_Output		  	 	 ;Chamar a fun��o para acender os dois LEDs
	B MainLoop                   ;Volta para o la�o principal	


    ALIGN                        ;Garante que o fim da se��o est� alinhada 
    END                          ;Fim do arquivo
