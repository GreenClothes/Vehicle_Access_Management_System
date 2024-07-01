/*
 * UART.h
 *
 * Created: 2022-11-14 P 3:47:07
 *  Author: kybin.kim
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define REC_BUFF_MAX_LENGTH 100

unsigned char RecBuff[REC_BUFF_MAX_LENGTH];
unsigned char RecBuffindex;
unsigned char RecFlg;
unsigned char RecBuff_estLength = REC_BUFF_MAX_LENGTH;
char STR_forLCD[20]; // LCD 출력 문자 저장 위함

void putch_USART1(char data) {
	while( !(UCSR1A & (1<<UDRE1)));
	UDR1 = data;
}
void puts_USART1(char *str){  // 문자열 출력 루틴
	while( *str != 0){  // 문자의 마지막에는 ‘\0’이 들어가 있으므로
		putch_USART1(*str);  // ‘\0’이 나올 때까지 출력한다.
		str++;
	}
}
void sendBuff_USART1(char *str,int length){  // 문자열 출력 루틴
	while (length--){
		putch_USART1(*str);  // ‘\0’이 나올 때까지 출력한다.
		str++;
	}
}
void Init_USART1_IntCon(void){
	// ① RXCIE1=1(수신 인터럽트 허가), RXEN0=1(수신 허가), TXEN0 = 1(송신 허가)
	UCSR1B = (1<<RXCIE1)| (1<<RXEN1) | (1 <<TXEN1);
	UBRR1H = 0x00; // ② 57600bps 보오레이트 설정
	UBRR1L = 0x0F; // ② 57600bps 보오레이트 설정
	sei(); // ③ 인터럽트 동작 시작(전체 인터럽트 허가)
}
ISR(USART1_RX_vect){// 인터럽트 루틴에서의 데이터 수신
	RecBuff[RecBuffindex] = UDR1;
	RecBuffindex++;
	if(RecBuffindex > 4){  // 데이터 길이정보가 수신된 경우 패킷 길이 갱신
		RecBuff_estLength = (RecBuff[4] + 6); // 패킷길이는 데이터 길이+6
	}
	if(RecBuffindex == RecBuff_estLength){          // 수신된 데이터의 순서가 패킷 길이와 같으면 패킷 수신완료
		RecFlg = 1; // 수신 완료 플래그 활성화
	}
}

void resPacket(char type, char rw, unsigned char length, unsigned char *resData){
	unsigned char rescBuff[REC_BUFF_MAX_LENGTH];
	unsigned char resBuffLength = 0;
	int i=0;
	rescBuff[resBuffLength++] = 0xff;
	rescBuff[resBuffLength++] = 0x01;
	rescBuff[resBuffLength++] = type;
	rescBuff[resBuffLength++] = rw;
	rescBuff[resBuffLength++] = length;
	for(i = 0 ; i<length ; i++){
		rescBuff[resBuffLength++] = resData[i];
	}
	sendBuff_USART1(rescBuff,resBuffLength);
}

// type/////////////////////////////////
// 1 : motor, 2 : LCD, 3 : switch & LED
////////////////////////////////////////

int parsingPacket(char *recBuff, int length){ // 패킷을 파싱하는 함수
	//unsigned char *keyscan_num;
	
	int check_length = 0;
	// 패킷검사 1. 만약 시작 데이터가 정해진 데이터(0xFF)가 아닌 경우 종료.
	if(recBuff[0] != 0xFF) return 2;
	// 패킷검사 2. 장치의 이름이 1이 아닌 경우 종료.
	if(recBuff[1] != 0x10) return 3;
	switch(recBuff[3])
	{
		case 0x01:  // type 1 : motor
		
		/////////////motor control code///////////////////
		
		check_length = 5;
		LCD_Clear();
		LCD_Pos(0, 0);
		LCD_Data(0x02);
		LCD_Data(0x03);
		LCD_Pos(1, 0);
		LCD_Data(0x04);
		LCD_Data(0x05);
		
		SSound(Do);
		SSound(Re);
		SSound(Mi);
		MOTOR_backward_motion();
		
		_delay_ms(10000);
		
		MOTOR_forward_motion();
		LCD_Clear();
		
		break;
		case 0x02:  // type 2 : switch & Keypad
		
		////////////switch & Keypad code//////////////////////
		LCD_Pos(0, 0);
		LCD_Str("no detection");
		PORTB |= 0x80; // LED ON
		
		int i = 0;
		while(1){ // switch 입력 대기
			while((PINE & 0x20) == 0x20){
				PORTB &= ~(0x80);
				break;
			}
			if((PORTB & 0x80) == 0x00) break;
			i++;
			_delay_ms(5);
			if(i==2000){
				PORTB &= ~(0x80);
				LCD_Clear();
				return 1;
			}
		}
		LCD_Pos(0, 0);
		LCD_Str("input APT NUM");
		
		keyscan_num = keyScan();

		resPacket(0x02, 0x01, 0x06, keyscan_num);
		packet_type = 0x02;
		LCD_Clear();
		
		SSound(Do);
		SSound(Re);
		SSound(Mi);
		
		MOTOR_backward_motion();
		
		_delay_ms(10000);
		
		MOTOR_forward_motion();
		break;
		case 0x03:  // type 3 : only Keypad
		
		////////////only Keypad control code/////////////
		LCD_Pos(0, 0);
		LCD_Str("input APT NUM");
		
		keyscan_num = keyScan();
		resPacket(0x02, 0x01, 0x06, keyscan_num);
		packet_type = 0x03;
		LCD_Clear();
		
		SSound(Do);
		SSound(Re);
		SSound(Mi);
		MOTOR_backward_motion();
		
		_delay_ms(10000);
		
		MOTOR_forward_motion();
		break;
		case 0x04: // type 4 : data re send
		if (packet_type == 0x01){
			resPacket(0x01, 0x01, 0x01, &blank_Data);
			LCD_Clear();
			LCD_Str("Re send");
		}
		else if (packet_type == 0x02){
			resPacket(0x02, 0x01, 0x06, keyscan_num);
		}
		else if (packet_type == 0x03){
			resPacket(0x02, 0x01, 0x06, keyscan_num);
		}
		break;
	}
	
	return 1;
}


#endif /* UART_H_ */