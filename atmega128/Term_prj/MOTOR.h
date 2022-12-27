/*
 * MOTOR.h
 *
 * Created: 2022-11-15 오전 10:02:58
 *  Author: kkb99
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#define TIME_DELAY 131 // TCNT 설정
#define TIME_VALUE 1   // 주기 값 설정 (TCNT 131 사용시 1ms)
const unsigned char forward_data[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};
const unsigned char backward_data[8] = {0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01};

volatile unsigned char step; // 스텝
volatile unsigned short tmpTime; // 주기
volatile unsigned short count; // 스텝 횟수

ISR(TIMER0_OVF_vect)
{
	TCNT0 = TIME_DELAY; // 주기 판단을 위한 초기화
	tmpTime += 1; // 주기 처리
	
	if(tmpTime>=TIME_VALUE) // 주기 판단
	{
		step = (step+1) % 8; // 스텝 상태 (0~7)
		count += 1;  // 총 스텝 수
		tmpTime = 0; // 주기 초기화
	}
}

void MOTOR_forward_motion()
{
	TIMSK |= 0x01; // 타이머 인터럽트 설정
	sei(); // 전역 인터럽트 허가
	TCCR0 = 0x05; // 타이머0 시작 (분주비 : 128)
	
	while(1)
	{
		PORTB = forward_data[step];
		
		if(count >= 1024){
			PORTB = 0x00;
			count = 0;
			tmpTime = 0;
			step = 0;
			
			TIMSK &= ~0x01;
			cli();
			TCCR0 &= ~0x05;
			sei();
			break;
		}
	}
}

void MOTOR_backward_motion()
{
	TIMSK |= 0x01; // 타이머 인터럽트 설정
	sei(); // 전역 인터럽트 허가
	TCCR0 = 0x05; // 타이머0 시작 (분주비 : 128)
	
	while(1)
	{
		PORTB = backward_data[step];
		
		if(count >= 1024){
			PORTB = 0x00;
			count = 0;
			tmpTime = 0;
			step = 0;
			
			TIMSK &= ~0x01;
			cli();
			TCCR0 &= ~0x05;
			sei();
			break;
		}
	}
}

void Init_MOTOR()
{
	count = 0;
	DDRB |= 0x0f;
	PORTB &= ~0x0f;
	TCNT0 = TIME_DELAY;
	//TIMSK |= 0x01; // 타이머 인터럽트 설정
	//sei(); // 전역 인터럽트 허가
	//TCCR0 = 0x05; // 타이머0 시작 (분주비 : 128)
}


#endif /* MOTOR_H_ */