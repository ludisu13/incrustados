#include "Screen.hpp"

screen::screen(uint8_t id, Graphics_Context *context,Graphics_Rectangle *New,Graphics_Rectangle *Old)
{
	m_u8TaskID=id;
	mycontext=context;
	oldRec=Old;
	newRec=New;
	newOrientation=true;
	curOrientation=true;;
}

uint8_t screen::run(void)
{
	Graphics_fillRectangleOnDisplay((*mycontext).display,oldRec, GRAPHICS_COLOR_BLUE);//cielo
	Graphics_fillRectangleOnDisplay((*mycontext).display,newRec,0xa347);//tierra
	if(newOrientation!=curOrientation)
		{if(newOrientation==true)
			{Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);}
		else
			{Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);}
		}
	curOrientation=newOrientation;
	MAP_Interrupt_enableInterrupt(INT_ADC14);
    return(NO_ERR);
}

uint8_t screen::readMessage(uint8_t source, uint64_t data)
{
	switch(source)
	{
			case 0: (*newRec).yMax= 0.0194*data-93.12;
	        		(*oldRec).yMin =(*newRec).yMax;
	        		TIMER_A0->CCR[2]=-0.85*data+62280;// actualizacion

			case 1:  if(data>8000)
						{newOrientation=true;}//rotacion
					else
						{newOrientation=false;}

			default: break;

	}
	return (NO_ERR);
}
