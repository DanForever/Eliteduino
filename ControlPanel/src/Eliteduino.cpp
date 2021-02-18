#include "Eliteduino.h"

#include "Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "controls/Matrix.h"

Eliteduino::Controls::Matrix gMatrix;

void Eliteduino::SetupButtons( PCCommunications* comms )
{
	gMatrix.Initialize( BUTTON_ROWS, BUTTON_COLUMNS, BUTTON_COLUMN_COUNT, BUTTON_ROW_COUNT, DEBOUNCE_INTERVAL, comms );
}

void Eliteduino::UpdateButtons()
{
	gMatrix.Update();
}

#endif // ELITEDUINO_BUTTONS
