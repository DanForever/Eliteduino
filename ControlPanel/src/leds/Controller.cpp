#include "Controller.h"

#include "Matrix.h"

void Eliteduino::Leds::Controller::Initialize( const LedMatrixConfig& matrix, PCCommunications* comms )
{
	m_matrix = new Matrix();

	m_matrix->Initialize( matrix.Pin.Din, matrix.Pin.Clk, matrix.Pin.Cs, matrix.Dimensions.RowCount, matrix.Dimensions.ColumnCount, comms );
	m_matrix->SetAll( true );
}

void Eliteduino::Leds::Controller::Initialize( const LedPinConfig& pins, PCCommunications* comms )
{
}

void Eliteduino::Leds::Controller::Initialize( const LedMatrixConfig& matrix, const LedPinConfig& pins, PCCommunications* comms )
{
}

void Eliteduino::Leds::Controller::SetBinding( uint8_t index, const Bindings::Binding* binding )
{
}

void Eliteduino::Leds::Controller::Update()
{
}
