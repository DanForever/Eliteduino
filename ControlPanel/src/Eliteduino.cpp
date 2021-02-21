#include "Eliteduino.h"

#include "Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "controls/Matrix.h"
#include "controls/BindingManager.h"

Eliteduino::Controls::Matrix gMatrix;
Eliteduino::Controls::BindingsManager gBindingsManager;

void Eliteduino::SetupButtons( PCCommunications* comms )
{
	gMatrix.Initialize( BUTTON_ROWS, BUTTON_COLUMNS, BUTTON_ROW_COUNT, BUTTON_COLUMN_COUNT, DEBOUNCE_INTERVAL, comms );
	gBindingsManager.Initialize( BUTTON_ROW_COUNT, BUTTON_COLUMN_COUNT );

	const uint8_t buttonCount = BUTTON_ROW_COUNT * BUTTON_COLUMN_COUNT;
	for ( uint8_t i = 0; i < buttonCount; ++i )
	{
		const auto* binding = gBindingsManager.GetBinding( i );
		gMatrix.SetBinding( i, binding );
	}
}

void Eliteduino::UpdateButtons()
{
	gMatrix.Update();
}

#endif // ELITEDUINO_BUTTONS
