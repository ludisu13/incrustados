#include "Screen.hpp"

screen::screen(uint8_t id, Graphics_Context *context,Graphics_Rectangle *iLand,Graphics_Rectangle *iSky)
{
	m_u8TaskID=id;
	mycontext=context;
	Land=iLand;
	Sky=iSky;
	newOrientation=true;
	curOrientation=true;
	currentLevel=(*Land).yMin;
	newLevel=0;
}

uint8_t screen::run(void)
{
	if(currentLevel>newLevel)
		{(*Sky).yMax=currentLevel;
		 (*Sky).yMin=newLevel;
		 Graphics_fillRectangleOnDisplay((*mycontext).display,
		 	            		Sky, GRAPHICS_COLOR_BLUE);}
	else
		{(*Land).yMax=newLevel;
		 (*Land).yMin=currentLevel;
		 Graphics_fillRectangleOnDisplay((*mycontext).display,
		 		 	            		Land, 0XA347);}
	if(newOrientation!=curOrientation)
		{if(newOrientation==true)
			{Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);}
		else
			{Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);}
		}
	currentLevel=newLevel;
	curOrientation=newOrientation;
	MAP_Interrupt_enableInterrupt(INT_ADC14);
    return(NO_ERR);
}

uint8_t screen::readMessage(uint8_t source, uint64_t data)
{
	switch(source)
	{
			case 0:
					newLevel=/*-0.0194*data+221.12;/*/0.0194*data-93.12;
	        		TIMER_A0->CCR[2]=-0.85*data+62280;// actualizacion

			case 1:  if(data>8000)
						{newOrientation=true;}//rotacion
					else
						{newOrientation=false;}

			default: break;

	}
	return (NO_ERR);
}
