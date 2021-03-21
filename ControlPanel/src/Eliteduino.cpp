#include "Eliteduino.h"

#include "Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "controls/Matrix.h"
#include "bindings/BindingManager.h"
#include "leds/Controller.h"
#include "debug/Debug.h"

Eliteduino::Controls::Matrix gButtonMatrix;
Eliteduino::Leds::Controller gLedController;
Eliteduino::Bindings::BindingsManager gBindingsManager;

void Eliteduino::SetupButtons( PCCommunications* comms )
{
	gBindingsManager.Initialize( BUTTON_ROW_COUNT, BUTTON_COLUMN_COUNT );

#if defined(ELITEDUINO_LEDMATRIX)
	Eliteduino::Leds::LedMatrixConfig ledMatrixConfig =
	{
		{
			LED_CONTROLLER_PIN_DIN,
			LED_CONTROLLER_PIN_CLK,
			LED_CONTROLLER_PIN_CS
		},
		{
			LED_CONTROLLER_ROW_COUNT,
			LED_CONTROLLER_COLUMN_COUNT
		}
	};

	gLedController.Initialize( ledMatrixConfig, comms );

	for ( uint8_t column = 0; column < BUTTON_COLUMN_COUNT; ++column )
	{
		for ( uint8_t row = 0; row < BUTTON_ROW_COUNT; ++row )
		{
			const auto* binding = gBindingsManager.GetBinding( row, column );
			gLedController.SetBinding( Leds::MatrixAddress { row, column }, binding );
		}
	}
#endif

	gButtonMatrix.Initialize( BUTTON_ROWS, BUTTON_COLUMNS, BUTTON_ROW_COUNT, BUTTON_COLUMN_COUNT, DEBOUNCE_INTERVAL, comms );


	for ( uint8_t i = 0; i < gBindingsManager.GetBindingCount(); ++i )
	{
		const auto* binding = gBindingsManager.GetBinding( i );
		gButtonMatrix.SetBinding( i, binding );
	}
}

void Eliteduino::UpdateButtons()
{
	gButtonMatrix.Update();
	gLedController.Update();
}

#endif // ELITEDUINO_BUTTONS
