/*
 * BUZZER.h
 *
 * Created: 2022-12-18 오후 2:52:53
 *  Author: kkb99
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#define Do 1908 // 262Hz (3817us) 1908us
#define Re 1700 // 294Hz (3401us) 1701us
#define Mi 1515 // 330Hz (3030us) 1515us
#define Fa 1432 // 349Hz (2865us) 1433us
#define Sol 1275 // 370Hz (2703us) 1351us
#define La 1136 // 440Hz (2273us) 1136us
#define Si 1012 // 494Hz (2024us) 1012us

void myDelay_us(unsigned int delay)
{
	int i;
	for(i=0; i<delay; i++)
	{
		_delay_us(1);
	}
}

void SSound(int time)
{
	int i, tim;
	tim = 50000 / time;
	// 음계마다 같은 시간동안 울리도록 tim 변수 사용
	// ex) time = 10, tim = 50000/10 = 5000
	// loopDelay : 10 x 2 = 20 us
	// Function delay : 20us * 5000 = 0.1Sec
	for(i=0; i<tim; i++) {
		PORTG &= ~(1<<PG4); //buzzer off, PORTG의 4번 핀 off(out 0)
		
		myDelay_us(time);
		PORTG |= (1<<PG4); //buzzer on, PORTG의 4번 핀 on(out 1)
		myDelay_us(time);
	}
}

void Init_BUZZER()
{
	DDRG |= (1<<PG4); // 부저와 연결되는 PORTG.4를 출력으로 설정
	PORTG |= (1<<PG4); // 교육용 보드의 BUZZER는 회로가 Active-Low 로 되어있음으로
}

#endif /* BUZZER_H_ */