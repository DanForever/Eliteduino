#include "../Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "Matrix.h"

#include "Button.h"

#include <Arduino.h>

inline uint8_t Eliteduino::Controls::Matrix::Index( uint8_t row, uint8_t column )
{
	return row + ( column * m_rowCount );
}

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
			const uint8_t index = Index( row, column );
			Button& button = m_buttons[ index ];

			const uint8_t rowPin = m_rows[ row ];
			button.Initialize( rowPin, LOW, comms );
		}

		DeactivateColumn( column );
	}
}

void Eliteduino::Controls::Matrix::SetPressedState( uint8_t row, uint8_t column, uint8_t state )
{
}

void Eliteduino::Controls::Matrix::Update()
{
	for ( uint8_t column = 0; column < m_columnCount; ++column )
	{
		ActivateColumn( column );

		for ( uint8_t row = 0; row < m_rowCount; ++row )
		{
			const uint8_t index = Index( row, column );
			Button& button = m_buttons[ index ];

//			Serial.print( "Row: " );
//			Serial.print( row );
//
//			Serial.print( " / Col: " );
//			Serial.print( column );
//
//			Serial.print( " / Idx: " );
//			Serial.println( index );

//			if ( row == 0 && column == 0 )
//			{
//				Serial.print( "Button 0 state: " );
//				Serial.println( button.Raw() );
//			}

			button.Update();
		}

		DeactivateColumn( column );
	}
}

#endif // ELITEDUINO_BUTTONS
