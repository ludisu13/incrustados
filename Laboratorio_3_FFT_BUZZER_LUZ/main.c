//****************************************************************************
//
// main.c - Laboratorio 3
//
//         Jose Pablo Avila,Luis Diego Soto
//         Desplazador de notas musicales controlado por luz
//
//****************************************************************************

#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <arm_math.h>
#include "arm_const_structs.h"
#include <HAL_I2C.h>
#include <HAL_OPT3001.h>

//constantes para la transformada rapida de Fourier
#define TEST_LENGTH_SAMPLES 512
#define SAMPLE_LENGTH 512


/* Variables globales para la FFT */

uint32_t g_u32FftSize = SAMPLE_LENGTH;
uint32_t g_u32IfftFlag = 0;
uint32_t g_u32DoBitReverse = 1;
volatile arm_status g_asStatus;

/* Contexto de graficos */
Graphics_Context g_sContext;
Graphics_Rectangle g_sShader;

#define SMCLK_FREQUENCY     48000000
#define SAMPLE_FREQUENCY    8000
#define FREQ_SLOPE          6000000
#define LUX_DIVISOR 		5000
#define NO_SOUND            20


/* Tabla de control del DMA */
#ifdef ewarm
#pragma data_alignment=256
#else
#pragma DATA_ALIGN(g_u8ControlTable, 256)
#endif
uint8_t g_u8ControlTable[256];


/* Buffers de procesamiento de la fft*/
float g_fHann[SAMPLE_LENGTH];
int16_t g_i16DataArray1[SAMPLE_LENGTH];
int16_t g_i16DataArray2[SAMPLE_LENGTH];
int16_t g_i16DataInput[SAMPLE_LENGTH*2];
int16_t g_i16DataOutput[SAMPLE_LENGTH];

volatile int g_iSwitchData = 0;


/* Configuracion PWM del Timer A */
Timer_A_PWMConfig pwmConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        (SMCLK_FREQUENCY/SAMPLE_FREQUENCY),
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_SET_RESET,
        (SMCLK_FREQUENCY/SAMPLE_FREQUENCY)/2
};
/*Variables de control */
uint8_t g_u8Flag=0;
uint8_t g_u8Flag2=1;
uint32_t color = 0;

void main(void)
{
    /* Se deshabilitan las interrupciones*/
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Voltaje del core VCORE1*/
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Estado de espera para los bancos 0 y 1 de la memoeria flash*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /*Sistema de reloj a 48MHZ */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Inicializacion del interfaz I2C*/
    Init_I2C_GPIO();
    I2C_init();

    /* Se inicializa el sensor de luz*/
    OPT3001_init();
     __delay_cycles(100000);

    /* Se inicializa la pantalla */
    Crystalfontz128x128_Init();

    /* Orientacion de la pantalla */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Contexto de graficos */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

    /* Ambiente grafico de la aplicacion */
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawLineH(&g_sContext, 0, 127, 115);
    Graphics_drawLineV(&g_sContext, 0, 115, 117);
    Graphics_drawLineV(&g_sContext, 16, 115, 116);
    Graphics_drawLineV(&g_sContext, 31, 115, 117);
    Graphics_drawLineV(&g_sContext, 32, 115, 117);
    Graphics_drawLineV(&g_sContext, 48, 115, 116);
    Graphics_drawLineV(&g_sContext, 63, 115, 117);
    Graphics_drawLineV(&g_sContext, 64, 115, 117);
    Graphics_drawLineV(&g_sContext, 80, 115, 116);
    Graphics_drawLineV(&g_sContext, 95, 115, 117);
    Graphics_drawLineV(&g_sContext, 96, 115, 117);
    Graphics_drawLineV(&g_sContext, 112, 115, 116);
    Graphics_drawLineV(&g_sContext, 127, 115, 117);
    g_sShader.xMax = 128;
   	g_sShader.xMin = 30;
   	g_sShader.yMax = 15;
   	g_sShader.yMin = 0;
    Graphics_drawStringCentered(&g_sContext,
                                    "FFT",
                                    AUTO_STRING_LENGTH,
                                    12,
                                    6,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    "0",
                                    AUTO_STRING_LENGTH,
                                    4,
                                    122,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    "1",
                                    AUTO_STRING_LENGTH,
                                    32,
                                    122,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    "2",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    122,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    "3",
                                    AUTO_STRING_LENGTH,
                                    96,
                                    122,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    "4",
                                    AUTO_STRING_LENGTH,
                                    125,
                                    122,
                                    OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawStringCentered(&g_sContext,
                                    "kHz",
                                    AUTO_STRING_LENGTH,
                                    112,
                                    122,
                                    OPAQUE_TEXT);

    /* Ventana hann para al fft */
    int n;
    for (n = 0; n < SAMPLE_LENGTH; n++)
    {
        g_fHann[n] = 0.5 - 0.5 * cosf((2*PI*n)/(SAMPLE_LENGTH-1));
    }

    /* Configuracion del Timer_A1   */
    Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig);

    /* Inicializacion del ADC14 */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE3, false); // gatillo por timer A1

    /*Configuracion del TimerA0 para el buzzer*/
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__8 ;
    TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
    TIMER_A0->CCR[0] =NO_SOUND ;
    TIMER_A0->CCR[4] =NO_SOUND ;// Inicialmente no debe zonar
    TIMER_A0->CCTL[4] |= TIMER_A_CCTLN_OUTMOD_3; //Set/Reset (cuando CCR[0] se pone en 0, cuando CCR[4] se pone en 1)
    P2->DIR |= BIT7; // La salida para el buzzer es el bit 7 del puerto 2
    P2->SEL0 =BIT7;
    P2->SEL1 =0;
    // Mapeo del Timer_A0.4 al pin 2.7 (PWM) output
    PMAP->KEYID = PMAP_KEYID_VAL;
    P2MAP ->PMAP_REGISTER7 |= PMAP_TA0CCR4A;

    /*Configuracion de botones y LEDS */
    P1->DIR &= ~(BIT1) | ~(BIT4); // Entradsas para interrupciones
    P1->DIR |=BIT0;// LED Rojo
    P2->DIR |=BIT1;// LED Verde
    P2->OUT &= ~(BIT1); // puerto en 0
    P1->OUT |= BIT0; // inicialmente el led rojo encendido
    P1->OUT |= BIT1 | BIT4;
    P1->REN |= BIT1 | BIT4;                 // Enable pull-up resistor (P1.1 and P1.4 output high)
    P1->SEL0 &= ~(BIT1) | ~(BIT4);
    P1->SEL1 &= ~(BIT1) | ~(BIT4);
    P1->IFG &= ~(BIT1) | ~(BIT4);                   // Limpiar banderas
    P1->IES |= BIT1 | BIT4;                 // Flanco negativo
    P1->IE |= BIT1 | BIT4;                  // Habilitar interupciones
    NVIC_SetPriority(PORT1_IRQn,2);
    NVIC_EnableIRQ(PORT1_IRQn);

    /* GPIO del microfono (4.3 A10) */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3,
    GPIO_TERTIARY_MODULE_FUNCTION);

    /* Memoria del ADc */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
    ADC_INPUT_A10, false);

    /* Resultado del ADC en binario con signo*/
    ADC14_setResultFormat(ADC_SIGNED_BINARY);

    /* Configuracion del DMA */
    DMA_enableModule();
    DMA_setControlBase(g_u8ControlTable);


    DMA_disableChannelAttribute(DMA_CH7_ADC14,
                                 UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                                 UDMA_ATTR_HIGH_PRIORITY |
                                 UDMA_ATTR_REQMASK);


    /* Indices de control. La fuente es la memoria del ADC y el destino es el arreglo con lso datos.*/
    MAP_DMA_setChannelControl(UDMA_PRI_SELECT | DMA_CH7_ADC14,
        UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH7_ADC14,
        UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[0],
        g_i16DataArray1, SAMPLE_LENGTH);

    MAP_DMA_setChannelControl(UDMA_ALT_SELECT | DMA_CH7_ADC14,
        UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH7_ADC14,
        UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[0],
        g_i16DataArray2, SAMPLE_LENGTH);

    /* Se habilitan las interrupciones */
    MAP_DMA_assignInterrupt(DMA_INT1, 7);
    MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
    MAP_DMA_assignChannel(DMA_CH7_ADC14);
    MAP_DMA_clearInterruptFlag(7);
    MAP_Interrupt_enableMaster();

    /*Se habilitan las conversiones */
    MAP_DMA_enableChannel(7);
    MAP_ADC14_enableConversion();
    /*Variables locales */
    q15_t l_q15MaxValue;
    uint32_t l_u32MaxIndex = 0;
    float l_fMaxFreq ;
    char l_cStringFreq [20];
    float l_fSampledFreq;
    uint32_t l_u32Lux;

    while(1)
    {
        MAP_PCM_gotoLPM0();

        int i = 0;

        /* Se calcula la FFt con el buffer de datos completado */
        if (g_iSwitchData & 1)
        {
            for (i=0; i<512; i++)
            {
                g_i16DataArray1[i] = (int16_t)(g_fHann[i]*g_i16DataArray1[i]);
            }
            arm_rfft_instance_q15 instance;
            g_asStatus = arm_rfft_init_q15(&instance, g_u32FftSize, g_u32IfftFlag, g_u32DoBitReverse);

            arm_rfft_q15(&instance, g_i16DataArray1, g_i16DataInput);
        }
        else
        {
            for (i=0; i<512; i++)
            {
                g_i16DataArray2[i] = (int16_t)(g_fHann[i]*g_i16DataArray2[i]);
            }
            arm_rfft_instance_q15 instance;
            g_asStatus = arm_rfft_init_q15(&instance, g_u32FftSize, g_u32IfftFlag, g_u32DoBitReverse);

            arm_rfft_q15(&instance, g_i16DataArray2, g_i16DataInput);
        }

        /* Se calcula la magnitud de la fft */
        for (i = 0; i < 1024; i+=2)
        {
            g_i16DataOutput[i/2] = (int32_t)(sqrtf((g_i16DataInput[i] * g_i16DataInput[i]) + (g_i16DataInput[i+1] * g_i16DataInput[i+1])));
        }


        /*Se obtiene el indice con la magnitud mayor   */
        arm_max_q15(g_i16DataOutput, g_u32FftSize, &l_q15MaxValue, &l_u32MaxIndex);
        /*Logica de color del grafico */
        if (l_u32MaxIndex <= 64)
            color = ((uint32_t)(0xFF * (l_u32MaxIndex / 64.0f)) << 8) + 0xFF;
        else if (l_u32MaxIndex <= 128)
            color = (0xFF - (uint32_t)(0xFF * ((l_u32MaxIndex-64) / 64.0f))) + 0xFF00;
        else if (l_u32MaxIndex <= 192)
            color = ((uint32_t)(0xFF * ((l_u32MaxIndex-128) / 64.0f)) << 16) + 0xFF00;
        else
            color = ((0xFF - (uint32_t)(0xFF * ((l_u32MaxIndex-192) / 64.0f))) << 8) + 0xFF0000;

        l_fMaxFreq=l_u32MaxIndex*15.79-10.58; //ecuacion lineal de la frecuencia en funcion del indice del arreglo
        if(l_fMaxFreq<=0)
        	l_fMaxFreq=0;
        Graphics_fillRectangleOnDisplay(g_sContext.display,&g_sShader, GRAPHICS_COLOR_WHITE); // se refresca la frecuencia maxima
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        sprintf(l_cStringFreq, "%f", l_fMaxFreq);
        Graphics_drawStringCentered(&g_sContext,(int8_t *)l_cStringFreq,6,55,6,OPAQUE_TEXT);

         if(g_u8Flag==1) // si se debe reproducir
         	 {
        	 if(g_u8Flag2==1)
        	 	 {
        		 l_fSampledFreq=l_fMaxFreq; // si es la primera vez en este estado
        	 	 g_u8Flag2=0U;
        	 	 }
        	 l_u32Lux = OPT3001_getLux()/LUX_DIVISOR;// se obtiene la lectura de luz
        	 if(l_u32Lux==0)
        	 	l_u32Lux=1;
        	TIMER_A0->CCR[0] =FREQ_SLOPE/(l_u32Lux*l_fSampledFreq);  //Periodo
        	TIMER_A0->CCR[4] =FREQ_SLOPE/(2*l_u32Lux*l_fSampledFreq); //Semiperiodo
         	  }
         else
         	 {
        	 TIMER_A0->CCR[0]=NO_SOUND ; // no suena
         	 TIMER_A0->CCR[4]=NO_SOUND ;
         	 }
        /* Se dibuja el grafico de la FFT */
        for (i = 0; i < 256; i+=2)
        {
            int x = min(100, (int)((g_i16DataOutput[i]+g_i16DataOutput[i+1])/8));

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
            Graphics_drawLineV(&g_sContext, i/2, 114-x, 14);
            Graphics_setForegroundColor(&g_sContext, color);
            Graphics_drawLineV(&g_sContext, i/2, 114, 114 - x);
        }
    }
}


/* Subrutina de atencion a interrupciones del DMA */
void DMA_INT1_IRQHandler(void)
{
    /* Se selecciona el buffer del DMA de acuerdo al modo PING POMG del DMA */
    if (DMA_getChannelAttribute(7) & UDMA_ATTR_ALTSELECT)
    {
        DMA_setChannelControl(UDMA_PRI_SELECT | DMA_CH7_ADC14,
            UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
        DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH7_ADC14,
            UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[0],
            g_i16DataArray1, SAMPLE_LENGTH);
        g_iSwitchData = 1;
    }
    else
    {
        DMA_setChannelControl(UDMA_ALT_SELECT | DMA_CH7_ADC14,
            UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
        DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH7_ADC14,
            UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[0],
            g_i16DataArray2, SAMPLE_LENGTH);
        g_iSwitchData = 0;
    }
}
/* Subrutina de atencion a interrupciones del Puerto 1 */
void PORT1_IRQHandler(void)
{



    if(P1->IFG & BIT1){
    		g_u8Flag=1; // Bandera en alto para indicar que se debe pasar al estado de reproduccion
    		P1->OUT &= ~(BIT0);
    		P2->OUT |= BIT1;
   	    	P1->IFG &= ~BIT1;

    }
    else if(P1->IFG & BIT4){

			g_u8Flag=0; // Bandera en bajo para indicar estado de grabacion
			g_u8Flag2=1; // Bandera en alto para indicar que en al siguiente activacion se debe seleccionar una frecuencia maxima
			P1->OUT |= BIT0;
			P2->OUT &= ~(BIT1);
			P1->IFG &= ~BIT4;

    }
    return;
}
