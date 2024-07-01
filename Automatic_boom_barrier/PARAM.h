/*
 * PARAM.h
 *
 * Created: 2022-11-14 오후 3:54:14
 *  Author: kkb99
 */ 


#ifndef PARAM_H_
#define PARAM_H_

unsigned char ho[] = {0x04, 0x1f, 0x04, 0x0a, 0x04, 0x04, 0x04, 0x1f};

unsigned char dong[] = {0x1f, 0x10, 0x1f, 0x04, 0x1f, 0x04, 0x0a, 0x04};

unsigned char deu[] = {0x00, 0x1f, 0x10, 0x10, 0x1f, 0x00, 0x1f, 0x00};
unsigned char ng[] = {0x00, 0x04, 0x0a, 0x11, 0x11, 0x0a, 0x04, 0x00};
unsigned char ro[] = {0x1f, 0x01, 0x1f, 0x10, 0x1f, 0x04, 0x1f, 0x00};
unsigned char g[] = {0x00, 0x1f, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00};
	
	
void CGRAM_Set()
{

	LCD_Delay(1);
	LCD_Comm(0x40);
	LCD_Delay(1);
	
	for(int j=0;j<8;j++){
		LCD_Data(dong[j]);
		LCD_Delay(1);
	}
	
	LCD_Comm(0x48);
	LCD_Delay(1);
	
	for(int j=0;j<8;j++){
		LCD_Data(ho[j]);
		LCD_Delay(1);
	}
	
	LCD_Comm(0x50);
	LCD_Delay(1);
	
	for(int j=0;j<8;j++){
		LCD_Data(deu[j]);
		LCD_Delay(1);
	}
	
	LCD_Comm(0x58);
	LCD_Delay(1);
	
	for(int j=0;j<8;j++){
		LCD_Data(ro[j]);
		LCD_Delay(1);
	}
	
	LCD_Comm(0x60);
	LCD_Delay(1);
	
	for(int j=0;j<8;j++){
		LCD_Data(ng[j]);
		LCD_Delay(1);
	}
	
	LCD_Comm(0x68);
	LCD_Delay(1);
	
	for(int j=0;j<8;j++){
		LCD_Data(g[j]);
		LCD_Delay(1);
	}
}

#endif /* PARAM_H_ */