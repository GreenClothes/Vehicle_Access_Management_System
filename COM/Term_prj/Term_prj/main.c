/*
 * Term_prj.c
 *
 * Created: 2022-11-14 오후 3:42:45
 * Author : kkb99
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char packet_type = 0x00;
unsigned char blank_Data[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char *keyscan_num;

#include "lcd.h"
#include "PARAM.h"
#include "KeyPad.h"
#include "BUZZER.h"
#include "UART.h"
#include "MOTOR.h"
#include "Distance.h"
#include "LIGHT.h"
#include "REMOTE.h"

void HIGH_LED(int onoff){
	// 전달받는 onoff에 따라 high luminance LED 켜고 끄기
	if(onoff){ 
		// onoff == 1, LED ON
		PORTD |= 0xf0;
	}
	else{ 
		// onoff == 0, LED OFF
		PORTD &= ~(0xf0);
	}
}

void Init_Port(){
	// PORTC for Keypad
	DDRC = 0xf0;
	PORTC = 0x00;
	// PORTD for high luminance LED
	DDRD |= 0xf0;
	PORTD &= 0x0f;
	// PORTE for switch
	DDRE &= ~(0x20);
	//PORTB for LED
	DDRB |= 0x80;
	PORTB &= ~(0x80);
}

int main(void)
{
	Init_USART1_IntCon();
	Init_MOTOR();
	Init_Distance();
	Init_LIGHT();
	LCD_Init();
	CGRAM_Set();
	Init_Port();
	Init_BUZZER();
	EXTI_Init();
	
	// 대기 상태
	unsigned char READY = 1;
	// HIGH LED ON/OFF 제어
	int MAIN_on_off = 0;
    
    while (1) 
    {
		if(READY && Check_Distance() && OPEN == 0){
			LCD_Pos(0, 0);
			LCD_Str("Welcome");
			READY = 0;
			
			// 주변 환경 어두우면 고휘도 LED ON
			if(Check_Light()){
				MAIN_on_off = 1;
				HIGH_LED(MAIN_on_off);
			}
			
			// 차량 번호판 인식 요청
			resPacket(0x01, 0x01, 0x06, &blank_Data);
			packet_type = 0x01;
		}
		
		Check_remote(); // 차단기 점검
		
		unsigned char lRecBuff[100];        // 수신패킷 임시저장용 배열
		unsigned char lRecBuffLength = 0;   // 수신패킷 길이 저장
		int res = 0;
		
		if(RecFlg == 1) // 패킷 수신완료 플래그가 설정된 경우
		{
			// 데이터의 연속 수신을 고려하여, 임시변수에 수신된 데이터 저장
			memcpy(lRecBuff, RecBuff, RecBuff_estLength);
			lRecBuffLength = RecBuff_estLength;
			
			// 임시저장 후 또다시 패킷 수신을 위한 버퍼 및 관련변수 초기화
			RecFlg = 0;
			memset(RecBuff, 0, REC_BUFF_MAX_LENGTH);
			RecBuff_estLength = REC_BUFF_MAX_LENGTH;
			RecBuffindex = 0;
			
			// 수신된 패킷을 파싱하는 함수
			res = parsingPacket(lRecBuff,lRecBuffLength);
			
			if(res == 1){
				// motor 동작 완료 시 대기 상태로 전환
				READY = 1;
				
				// if HIGH LED ON, turn off
				if(MAIN_on_off){
					MAIN_on_off = 0;
					HIGH_LED(MAIN_on_off);
				}
			}
			else if(res == 2){
				READY = 1;
				
				LCD_Clear();
				LCD_Pos(0,0);
				LCD_Str("START error");
			}
			else if(res == 3){
				READY = 1;
				
				LCD_Clear();
				LCD_Pos(0,0);
				LCD_Str("NAME error");
			}
			
		}
    }
}

