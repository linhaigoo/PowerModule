#include "System_Init.h"
void System_Init()
{
	LED_Init();
#ifdef USE_COM_LCD
	screenComInit();
#endif
	
#if defined(USE_COM_BOARD) || defined(USE_485_COM) || defined(USE_BLUE_TEETH)
	COM_USART_Init(BOARD_COM);
#endif

}
