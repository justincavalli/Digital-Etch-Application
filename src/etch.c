//
// COMP-GENG 421 - Justin Cavalli
//
// Etch A Sketch Emulator module
//

#include "main.h"
#include "adc.h"
#include "etch.h"
#include "lcd.h"
#include "nav.h"
#include "rgb.h"
#include "accel.h"


#define ETCH_ST_INIT		0		// state to initialize the system
#define ETCH_ST_CURSOR_1X1	1		// state to move the 1X1 point without drawing (cursor)
#define ETCH_ST_STYLUS_1X1	2		// state to move the 1X1 point and draw (stylus)
#define ETCH_ST_CURSOR_2X2	3		// state to move the 2X2 point without drawing (cursor)
#define ETCH_ST_STYLUS_2X2	4		// state to move the 2X2 point and draw (stylus)

#define POSX_TH			124		// X position threshold to switch between cursor and stylus
#define POSX_HYST		2		// X position threshold hysteresis

#define HOR_DIV			((ADC_RDG_MAX + 1) / LCD_PIXEL_WIDTH)	// PosX ADC reading divisor
#define VER_DIV			((ADC_RDG_MAX + 1) / LCD_PIXEL_HEIGHT)	// PosY ADC reading divisor

#define THIN_OBJECT		LCD_OBJ_POINT_1X1	// 1X1 object to be rendered
#define THICK_OBJECT	LCD_OBJ_POINT_2X2	// 2X2 object to be rendered

void EtchClock(void)
{
	static int EtchState = ETCH_ST_INIT;
	static int PrevPosX;
	static int PrevPosY;

	int CurPosX = -1;	// ensure not equal to initial position
	int CurPosY = -1;	// ensure not equal to initial position

	Bool NavUp = FALSE;
	Bool NavDown = FALSE;
	Bool NavLeft = FALSE;
	Bool NavRight = FALSE;

	// Get the current X,Y position, invert Y so clockwise rotation of right knob => up
	CurPosX = AdcLeftReadingGet() / HOR_DIV;
	CurPosY = (LCD_PIXEL_HEIGHT - 1 ) - (AdcRightReadingGet() / VER_DIV);

	// Check the nav switch directional buttons
	if (NavUpIsPressed())
	{
		NavClearUpIsPressed();
		NavUp = TRUE;
	}
	if (NavDownIsPressed())
	{
		NavClearDownIsPressed();
		NavDown = TRUE;
	}
	if (NavLeftIsPressed())
	{
		NavClearLeftIsPressed();
		NavLeft = TRUE;
	}
	if (NavRightIsPressed())
	{
		NavClearRightIsPressed();
		NavRight = TRUE;
	}

	// Check the nav switch select button
	if (NavSelectIsPressed())
	{
		LcdReverse(TRUE);
	}
	else
	{
		LcdReverse(FALSE);
	}


	// Check if the board has been shaken
	if(AccelIsShaken())
	{
		RGB_GRN_DIS();
		LcdClear();
		EtchState = ETCH_ST_INIT;
	}


	switch (EtchState)
	{
	case ETCH_ST_INIT:
		LcdRenderObject(THIN_OBJECT, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the cursor in its initial position
		RGB_RED_ENA();
		EtchState = ETCH_ST_CURSOR_1X1;
		break;

	case ETCH_ST_CURSOR_1X1:
		LcdRenderObject(THIN_OBJECT, PrevPosX, PrevPosY, LCD_MODE_CURSOR);		// erase the previous cursor
		RGB_RED_ENA();
		if (NavRight)
		{
			LcdRenderObject(THICK_OBJECT, CurPosX, CurPosY, LCD_MODE_CURSOR);	// render the 2X2 cursor to be erased in ETCH_ST_CURSOR_2X2
			EtchState = ETCH_ST_CURSOR_2X2;
		}
		else if (NavDown)
		{
			RGB_RED_DIS();														// disable the LED changing from cursor to stylus
			EtchState = ETCH_ST_STYLUS_1X1;
		}
		else
		{
			LcdRenderObject(THIN_OBJECT, CurPosX, CurPosY, LCD_MODE_CURSOR);	// render the 1X1 cursor
		}
		break;

	case ETCH_ST_STYLUS_1X1:
		LcdRenderObject(THIN_OBJECT, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the 1X1 stylus
		RGB_GRN_ENA();
		if (NavRight)
		{
			EtchState = ETCH_ST_STYLUS_2X2;
		}
		if (NavUp)
		{
			RGB_GRN_DIS();														// disable the LED changing from stylus to cursor
			EtchState = ETCH_ST_CURSOR_1X1;
		}
		break;

	case ETCH_ST_CURSOR_2X2:
		LcdRenderObject(THICK_OBJECT, PrevPosX, PrevPosY, LCD_MODE_CURSOR);		// erase the previous cursor
		RGB_RED_ENA();

		if (NavLeft)
		{
			LcdRenderObject(THIN_OBJECT, CurPosX, CurPosY, LCD_MODE_CURSOR);	// render the 1X1 cursor to be erased in ETCH_ST_CURSOR_1X1
			EtchState = ETCH_ST_CURSOR_1X1;
		}
		else if (NavDown)
		{
			RGB_RED_DIS();														// disable the LED changing from cursor to stylus
			EtchState = ETCH_ST_STYLUS_2X2;
		}
		else
		{
			LcdRenderObject(THICK_OBJECT, CurPosX, CurPosY, LCD_MODE_CURSOR);	// render the 2X2 cursor
		}
		break;

	case ETCH_ST_STYLUS_2X2:
		LcdRenderObject(THICK_OBJECT, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the 2X2 stylus
		RGB_GRN_ENA();
		if (NavLeft)
		{
			EtchState = ETCH_ST_STYLUS_1X1;
		}
		if (NavUp)
		{
			RGB_GRN_DIS();														// disable the LED changing from stylus to cursor
			EtchState = ETCH_ST_CURSOR_2X2;
		}
		break;
	}

	// Save the current position
	PrevPosX = CurPosX;
	PrevPosY = CurPosY;
}
