#include "Screen.hpp"

screen::screen(uint8_t id, Graphics_Context *i_pContext,Graphics_Rectangle *i_pLand,Graphics_Rectangle *i_pSky)
{
	m_u8TaskID=id;
	m_pMyContext=i_pContext;
	m_pLand=i_pLand;
	m_pSky=i_pSky;
	m_bNewOrientation=true;
	m_bCurOrientation=true;
	m_i16CurrentLevel=(*i_pLand).yMin;
	m_i16NewLevel=0;
}

	// **********************************
	// screen run function
	// plots change in the horizon
	// rotates screen if required
	// enables adc14 interrupt
	// **********************************

uint8_t screen::run(void)
{

	if(m_i16CurrentLevel>m_i16NewLevel) // Logic to draw sky or land, only the change in the horizon is drawn.
		{(*m_pSky).yMax=m_i16CurrentLevel;
		 (*m_pSky).yMin=m_i16NewLevel;
		 Graphics_fillRectangleOnDisplay((*m_pMyContext).display,
		 	            		m_pSky, GRAPHICS_COLOR_BLUE);}
	else
		{(*m_pLand).yMax=m_i16NewLevel;
		 (*m_pLand).yMin=m_i16CurrentLevel;
		 Graphics_fillRectangleOnDisplay((*m_pMyContext).display,
		 		 	            		m_pLand, 0XA347);}
	if(m_bNewOrientation!=m_bCurOrientation)//Logic to rotate
		{if(m_bNewOrientation==true)
			{Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);}
		else
			{Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);}
		}
	m_i16CurrentLevel=m_i16NewLevel;
	m_bCurOrientation=m_bNewOrientation;
	MAP_Interrupt_enableInterrupt(INT_ADC14);// reenable ADC interrupt
    return(NO_ERR);
}
	// **********************************
	// screen read Message function
	// Reads two messages:
	// 	-One to change the horzion
	// 	-One to rotate the screen
	// **********************************
uint8_t screen::readMessage(uint8_t i_i8Source, uint64_t i_i64Data)
{
	switch(i_i8Source)
	{
			case 0:  //horizon update
					m_i16NewLevel=0.0194*i_i64Data-93.12;//Linear function to plot according to the reading from the accelerometer


			case 1:  if(i_i64Data>8000) //screen rotation
						{m_bNewOrientation=true;}
					else
						{m_bNewOrientation=false;}

			default: break;

	}
	return (NO_ERR);
}
