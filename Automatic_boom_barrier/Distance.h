/*
 * Distance.h
 *
 * Created: 2022-11-21 PM 12:57:39
 *  Author: kybin.kim
 */ 


#ifndef DISTANCE_H_
#define DISTANCE_H_

#define TRIG 6 // trig (PE6)
#define ECHO 7 // Echo (PE7)
#define SOUND_VELOCITY 340UL // 소리 속도(m/s)



unsigned char Check_Distance()
{
	unsigned int distance; // 거리
	TCCR1B = 0x03; // Timer/Counter1 클럭 4.34us (64분주)
	
	PORTE &= ~(1<<TRIG); // Trig -> LOW
	_delay_us(10); // 10us 유지
	PORTE |= (1<<TRIG); // Trig -> HIGH, 거리 측정 시작
	_delay_us(10);
	PORTE &= ~(1<<TRIG); // Trig -> LOW, 거리 측정 끝
	
	while(!(PINE & (1<<ECHO))); // Echo -> HIGH 까지 대기
	TCNT1 = 0x0000; // Timer/Counter1 값 초기화
	
	while(PINE & (1<<ECHO)); // Echo -> LOW 까지 대기
	TCCR1B = 0x00; // Timer/Counter1 클럭 정지(클럭 입력 차단)
	
	distance = (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4.34/2)/1000); // 거리 = 속도 X 시간 (단위 mm)
	
	if(distance < 500){
		// 50cm 이하로 접근 시 1 반환(접근)
		return 1;
	}
	else{
		// 50cm 이내에 인식 없을 시 0 반환(객체X)
		return 0;
	}
}

void Init_Distance()
{
	DDRE = (DDRE |(1<<TRIG) & ~(1<<ECHO)); // TRIG -> output, ECHO -> input
	PORTE &= ~(1<<TRIG); // TRIG -> LOW
}

#endif /* DISTANCE_H_ */