#include "../Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "Matrix.h"

#include "Button.h"
#include "Utility.h"

#include <Arduino.h>
#include "..\leds\Matrix.h"

inline void Eliteduino::Controls::Matrix::ActivateColumn( uint8_t column )
{
	const uint8_t columnPin = m_columns[ column ];

	pinMode( columnPin, OUTPUT );
	digitalWrite( columnPin, LOW );
}

inline void Eliteduino::Controls::Matrix::DeactivateColumn( uint8_t column )
{
	const uint8_t columnPin = m_columns[ column ];

	pinMode( columnPin, INPUT );
}

void Eliteduino::Controls::Matrix::Initialize( const uint8_t* rows, const uint8_t* columns, uint8_t numRows, uint8_t numCols, int debounceInterval, PCCommunications* comms )
{
	m_rows = rows;
	m_columns = columns;
	m_rowCount = numRows;
	m_columnCount = numCols;

	const int buttonCount = numRows * numCols;

	m_buttons = new Button[ buttonCount ];

	// Eliteduino is designed as column major, so you go through each item in column 1 before moving onto column 2
	for ( uint8_t column = 0; column < m_columnCount; ++column )
	{
		ActivateColumn( column );

		for ( uint8_t row = 0; row < m_rowCount; ++row )
		{
			const uint8_t index = ButtonIndex( row, column, m_rowCount );
			Button& button = m_buttons[ index ];

			const uint8_t rowPin = m_rows[ row ];
			button.Initialize( rowPin, LOW, comms );
		}

		DeactivateColumn( column );
	}
}

void Eliteduino::Controls::Matrix::SetNormallyOpen( uint8_t index, bool isOpen )
{
	Button& button = m_buttons[ index ];
	button.SetNormallyOpen( isOpen );
}

void Eliteduino::Controls::Matrix::SetNormallyOpen( uint8_t row, uint8_t column, bool isOpen )
{
	const uint8_t index = ButtonIndex( row, column, m_rowCount );
	SetNormallyOpen( index, isOpen );
}

void Eliteduino::Controls::Matrix::SetBinding( uint8_t index, const Bindings::Binding* binding )
{
	Button& button = m_buttons[ index ];
	button.SetBinding( binding );
}

void Eliteduino::Controls::Matrix::SetBinding( uint8_t row, uint8_t column, const Bindings::Binding* binding )
{
	const uint8_t index = ButtonIndex( row, column, m_rowCount );
	SetBinding( index, binding );
}

void Eliteduino::Controls::Matrix::Update()
{
	for ( uint8_t column = 0; column < m_columnCount; ++column )
	{
		ActivateColumn( column );

		for ( uint8_t row = 0; row < m_rowCount; ++row )
		{
			const uint8_t index = ButtonIndex( row, column, m_rowCount );
			Button& button = m_buttons[ index ];

			button.Update();
		}

		DeactivateColumn( column );
	}
}

#endif // ELITEDUINO_BUTTONS
