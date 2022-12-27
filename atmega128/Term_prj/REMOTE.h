/*
 * REMOTE.h
 *
 * Created: 2022-12-18 오후 3:05:12
 *  Author: kkb99
 */ 


#ifndef REMOTE_H_
#define REMOTE_H_

uint8_t Flg_TimeOver;
uint16_t Pulse_Width[100];
uint16_t Pulse_WidthIndex;
uint8_t INSPECTION_command = 24;
unsigned char OPEN = 0;

void Timer_Init(){
	TIMSK |= (1<<TOIE2);
}
void Timer_TurnOn(){
	TCCR2 = 0x00;       // Timer 초기화
	TCNT2 = 0;          // Timer Counter 초기화
	TCCR2 = (1<<CS22) | (0<<CS21)| (1<<CS20);  // 1/1024
	// 1Cnt =  69.444.. us , OVF interval : 17.77.. ms
	TIMSK = (1<<TOIE2); // OVF 감지 설정
}
void Timer_TurnOff(){
	TCCR2 = 0x00;   // preScaler 초기화로 타이머 정지
}
ISR(TIMER2_OVF_vect){
	Timer_TurnOff();
	Flg_TimeOver = 1;
}
ISR(INT4_vect){
	Pulse_Width[Pulse_WidthIndex] = TCNT2;
	Pulse_WidthIndex++;
	Timer_TurnOn();
}
void EXTI_Init()    // PE4 - REMOTE IN
{
	EIMSK = 0x10;
	EICRB = (1<<ISC41) | (1<<ISC40);    // Riding Edge
	sei();
}

uint32_t GetCode_Raw(
uint16_t *pTimeArray
, uint16_t arraySize
){
	int i=0;
	uint32_t rawCode = 0;
	uint8_t address = 0;
	uint8_t commnad = 0;
	
	if(arraySize < 32)
	return -1;
	
	for(i=0; i<arraySize ; i++){
		if(pTimeArray[i] < 15){  // 1ms 이하의 pulse는 무시
			continue;
			}else if(pTimeArray[i] < 20){  // 1.388 ms 이하의 Pulse는 0으로 설정
			rawCode = rawCode<<1;
			}else if(pTimeArray[i] < 35){  // 2.43 ms 이하의 Pulse는 1으로 설정
			rawCode = rawCode<<1;
			rawCode |= 0x01;
		}
	}
	return rawCode;
}

uint32_t GetCode(
uint16_t *pTimeArray
, uint16_t arraySize
, uint8_t *pRetAddress
, uint8_t *pRetCommand
){
	int i=0;
	uint32_t rawCode = 0;
	uint32_t rawCode_inv = 0;
	uint8_t commnad = 0;
	
	rawCode = GetCode_Raw(pTimeArray,arraySize);
	if(rawCode == -1)
	return 0;
	
	rawCode_inv = ~rawCode;
	
	if( ((rawCode>>24)&0xFF) != ((rawCode_inv>>16)&0xFF) )
	return 0;
	
	if( ((rawCode>>8)&0xFF) != ((rawCode_inv>>0)&0xFF) )
	return 0;
	
	*pRetAddress  = (rawCode>>24)&0xFF;
	*pRetCommand  = (rawCode>>8)&0xFF;
	return rawCode;
}

void Check_remote()
{
	char str[100];
	int i=0;
	uint32_t remotecontroller_rawData;
	uint8_t remotecontroller_address;
	uint8_t remotecontroller_command;
	
	if(Flg_TimeOver)
	{
		remotecontroller_rawData = GetCode(
		Pulse_Width
		, Pulse_WidthIndex
		, &remotecontroller_address
		, &remotecontroller_command
		);
		
		if(remotecontroller_rawData != 0){
			if(remotecontroller_command == INSPECTION_command){
				LCD_Pos(0, 0);
				if(OPEN == 0){
					LCD_Str("INSPECTION...");
					SSound(Sol);
					SSound(La);
					SSound(Si);
					MOTOR_backward_motion();
					OPEN = 1;
				}
				else if(OPEN == 1){
					LCD_Clear();
					LCD_Str("Complete");
					SSound(Si);
					SSound(La);
					SSound(Sol);
					MOTOR_forward_motion();
					OPEN = 0;
					LCD_Clear();
				}
			}
		}
		
		// 리모콘 Pulse 정보 초기화
		Pulse_WidthIndex = 0;
		memset(Pulse_Width,0,sizeof(Pulse_Width));
		
		Flg_TimeOver = 0;
	}
}

#endif /* REMOTE_H_ */