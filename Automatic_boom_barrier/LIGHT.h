/*
 * LIGHT.h
 *
 * Created: 2022-11-21 PM 3:20:24
 *  Author: kybin.kim
 */ 


#ifndef LIGHT_H_
#define LIGHT_H_

int Check_Light()
{
	unsigned char adc_low, adc_high;
	unsigned short value = 0;
	ADCSRA |= 0x40;
	
	while((ADCSRA&0x10)!=0x10); // ADC 변환 완료 검사
	
	adc_low = ADCL; // 변환된 low 값 읽기
	adc_high = ADCH; // 변환된 high 값 읽기
	
	value = (adc_high<<8) | adc_low;
	
	if(value > 870){
		return 1;
	}
	else{
		return 0;
	}
}

void Init_LIGHT()
{
	DDRF &= 0xfe;
	ADMUX = 0x40;
	ADCSRA = 0x87;
}



#endif /* LIGHT_H_ */