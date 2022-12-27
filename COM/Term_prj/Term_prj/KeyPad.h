/*
 * KeyPad.h
 *
 * Created: 2022-11-14 오후 3:50:48
 *  Author: kkb99
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char number[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}; // LCD에 숫자를 표기할 수 있는 16진수

unsigned char* keyScan()
{
	LCD_Clear();
	static unsigned char sel[] = {0, 0, 0, 0, 0, 0};
	unsigned char sel_length = 0;
	unsigned char mode = 0;
	unsigned char print_LCD[] = {'\0', '\0', '\0', '\0', '\0', '\0'};
	unsigned char change = 0;
	
	LCD_Pos(0, 0);
	LCD_Str("input APT NUM");
	
	// 000 동 000 호 입력
	// 숫자 6개 입력받고 확인 버튼 이전까지 동작
	while(1){
		//키 1번만 입력받기 위해 while 문 사용
		PORTC=0xe0; _delay_ms(5); // 1번째 줄 선택
		while(PINC&0x01){if(sel_length!=6){sel[sel_length]=1;change=1;}}
		while(PINC&0x02){if(sel_length!=6){sel[sel_length]=2;change=1;}}
		while(PINC&0x04){if(sel_length!=6){sel[sel_length]=3;change=1;}}
		//
		PORTC=0xd0; _delay_ms(5); // 2번째 줄 선택
		while(PINC&0x01){if(sel_length!=6){sel[sel_length]=4;change=1;}}
		while(PINC&0x02){if(sel_length!=6){sel[sel_length]=5;change=1;}}
		while(PINC&0x04){if(sel_length!=6){sel[sel_length]=6;change=1;}}
		//
		PORTC=0xb0; _delay_ms(5); // 3번째 줄 선택
		while(PINC&0x01){if(sel_length!=6){sel[sel_length]=7;change=1;}}
		while(PINC&0x02){if(sel_length!=6){sel[sel_length]=8;change=1;}}
		while(PINC&0x04){if(sel_length!=6){sel[sel_length]=9;change=1;}}
		//
		PORTC=0x70; _delay_ms(5); // 4번째 줄 선택
		while(PINC&0x01){if(sel_length){mode = 1;change=1;}} // 입력 숫자 하나 지우기
		while(PINC&0x02){if(sel_length!=6){sel[sel_length]=0;change=1;}}
		while(PINC&0x04){if(sel_length==6)mode = 2;} // 입력완료 버튼, 입력 받은 동/호 수 반환
		
		if(mode == 1){
			sel_length--;
		}
		if(mode == 2){
			return sel; // 입력받은 값 반환
		}
		
		if(change){
			if(!mode)sel_length++;
			
			for(int i=0;i<sel_length;i++){
				print_LCD[i] = number[sel[i]];
			}
			
			LCD_Clear();
			LCD_Pos(0, 0);
			LCD_Str("input APT NUM");
			LCD_Pos(1, 0);
			for(int i=0;i<sel_length;i++){
				LCD_Char(print_LCD[i]);
				if(i==2){
					// LCD에 '동' 표시
					LCD_Data(0x00);
				}
				else if(i==5){
					// LCD에 '호' 표시
					LCD_Data(0x01);
				}
			}
			change = 0;
			mode = 0;
		}
	}
	
}


#endif /* KEYPAD_H_ */