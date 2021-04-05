#include <Arduino.h>
#include <EEPROM.h>

#include "BindingManager.h"
#include "Binding.h"

#include "../debug/Debug.h"
#include "../MatrixUtility.h"

#include "../Devices.h"
#ifdef ELITEDUINO_DEVICE_PROMICRO_16BUTTONSONLY
#include <HID-Project.h>
namespace
{
	Eliteduino::Bindings::Binding* ForceConfigureDansBindings()
	{
		using namespace Eliteduino;

		Bindings::Binding* bindings = new Bindings::Binding[ 16 ];

		bindings[ 0 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 0 ].PhysicalType = ePhysicalControlType::Toggle;
		bindings[ 0 ].Value = KEY_L;

		bindings[ 1 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 1 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 1 ].Value = KEY_HOME;

		bindings[ 2 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 2 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 2 ].ControlRole = eControlRole::SystemMap;
		bindings[ 2 ].Value = KEY_F3;

		bindings[ 3 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 3 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 3 ].ControlRole = eControlRole::GalaxyMap;
		bindings[ 3 ].Value = KEY_F4;

		bindings[ 4 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 4 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 4 ].Value = KEY_LEFT_BRACE;

		bindings[ 5 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 5 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 5 ].Value = KEY_H;

		bindings[ 6 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 6 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 6 ].Value = KEY_B;

		bindings[ 7 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 7 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 7 ].Value = KEY_Z;

		bindings[ 8 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 8 ].PhysicalType = ePhysicalControlType::Toggle;
		bindings[ 8 ].Value = KEY_INSERT;

		bindings[ 9 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 9 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 9 ].Value = KEY_DELETE;

		bindings[ 10 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 10 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 10 ].Value = KEY_END;

		bindings[ 11 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 11 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 11 ].Value = KEY_G;

		bindings[ 12 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 12 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 12 ].Value = KEY_O;

		bindings[ 13 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 13 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 13 ].ControlRole = eControlRole::Heatsink;
		bindings[ 13 ].Value = KEY_V;

		bindings[ 14 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 14 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 14 ].ControlRole = eControlRole::FrameshiftJump;
		bindings[ 14 ].Value = KEY_J;

		bindings[ 15 ].VirtualType = eVirtualControlType::Keyboard;
		bindings[ 15 ].PhysicalType = ePhysicalControlType::Momentary;
		bindings[ 15 ].ControlRole = eControlRole::Supercruise;
		bindings[ 15 ].Value = KEY_K;

		return bindings;
	}
}
#endif

// Header:
// 32 bit CRC
// 16 bit version number
// 24 byte binding struct * number of buttons

namespace
{
	static const uint16_t CRC_POSITION = 0;
	static const uint16_t VERSION_POSITION = sizeof( uint32_t );
	static const uint16_t BINDINGS_POSITION = VERSION_POSITION + sizeof( uint16_t );

	static const uint16_t BINDINGS_VERSION = 1;
}

inline uint32_t Eliteduino::Bindings::BindingsManager::CalculateCRC() const
{
	const uint16_t BINDINGS_END_POSITION = BINDINGS_POSITION + ( m_bindingCount * sizeof( Binding ) );
	return CalculateCRC( BINDINGS_POSITION, BINDINGS_END_POSITION );
}

// Adapted from https://forum.arduino.cc/index.php?topic=407671.msg2804981#msg2804981
uint32_t Eliteduino::Bindings::BindingsManager::CalculateCRC( uint16_t start, uint16_t end )
{
	const uint32_t crc_table[ 16 ] =
	{
		0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
		0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
		0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
		0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
	};

	uint32_t crc = ~0L;

	for ( int index = start; index < end; ++index )
	{
		const uint8_t value = EEPROM[ index ];

		crc = crc_table[ ( crc ^ value ) & 0x0f ] ^ ( crc >> 4 );
		crc = crc_table[ ( crc ^ ( value >> 4 ) ) & 0x0f ] ^ ( crc >> 4 );
		crc = ~crc;
	}

	return crc;
}

void Eliteduino::Bindings::BindingsManager::StoreDefaultBindings()
{
	// First the version number
	uint16_t version;
	EEPROM.get( VERSION_POSITION, version );
	if ( version != BINDINGS_VERSION )
	{
		PRINT( "Writing version number '", BINDINGS_VERSION, "' to eeprom byte position: ", VERSION_POSITION );
		EEPROM.put( VERSION_POSITION, BINDINGS_VERSION );
	}

	// Then the bindings themselves
	for ( uint8_t i = 0; i < m_bindingCount; ++i )
	{
		Binding& binding = m_bindings[ i ];
		binding = { eVirtualControlType::Gamepad, ePhysicalControlType::Momentary, eControlRole::Undefined, (uint8_t)(i + 1) };

		PRINT( "Setting default binding for button: ", i );
		PRINT( "  - Virtual control type: ", (int)binding.VirtualType );
		PRINT( "  - Physical control type: ", (int)binding.PhysicalType );
		PRINT( "  - Value: ", binding.Value );

		for ( uint8_t byte = 0; byte < sizeof(Binding); ++byte )
		{
			const uint16_t position = BINDINGS_POSITION + ( i * sizeof( Binding ) ) + byte;

			PRINT( "    -- Writing byte ", binding.Raw( byte ), " to position: ", position );
			EEPROM.update( position, binding.Raw( byte ) );
		}
	}

	// Finally the CRC
	const uint32_t calculatedCRC = CalculateCRC();
	EEPROM.put( CRC_POSITION, calculatedCRC );
	PRINT( "Writing CRC: ", calculatedCRC );
}

void Eliteduino::Bindings::BindingsManager::LoadBindings()
{
	// Skip over the version for now, since it doesn't really have any utility at this point (it's more of a "just in case" kinda thing)

	for ( uint8_t i = 0; i < m_bindingCount; ++i )
	{
		Binding& binding = m_bindings[ i ];

		PRINT( "Loading binding for button: ", i );

		for ( uint8_t byte = 0; byte < sizeof( Binding ); ++byte )
		{
			const uint16_t position = BINDINGS_POSITION + ( i * sizeof( Binding ) ) + byte;

			binding.Raw( byte ) = EEPROM[ position ];
			PRINT( "    -- Read byte ", binding.Raw( byte ), " to position: ", position );
		}
		PRINT( "  - Virtual control type: ", (int)binding.VirtualType );
		PRINT( "  - Physical control type: ", (int)binding.PhysicalType );
		PRINT( "  - Value: ", binding.Value );
	}
}

void Eliteduino::Bindings::BindingsManager::Initialize( uint8_t numRows, uint8_t numCols )
{
	m_rowCount = numRows;
	m_bindingCount = numRows * numCols;

#ifdef ELITEDUINO_DEVICE_PROMICRO_16BUTTONSONLY
	m_bindings = ForceConfigureDansBindings();
	return;
#endif

	m_bindings = new Binding[ m_bindingCount ];

	// In future, if we change the layout of the binding struct for any reason,
	// we will need to change this to be able to support reading in data in an old format
	if ( AreStoredBindingsValid() )
	{
		LoadBindings();
	}
	else
	{
		StoreDefaultBindings();
	}
}

bool Eliteduino::Bindings::BindingsManager::AreStoredBindingsValid() const
{
	uint32_t storedCRC = 0;
	EEPROM.get( CRC_POSITION, storedCRC );

	const uint32_t calculatedCRC = CalculateCRC();

	PRINT( "Stored CRC: ", storedCRC );
	PRINT( "Calc'd CRC: ", calculatedCRC );
	PRINT( "CRCs are equal: ", storedCRC == calculatedCRC );

	return storedCRC == calculatedCRC;
}

const Eliteduino::Bindings::Binding* Eliteduino::Bindings::BindingsManager::GetBinding( uint8_t index ) const
{
	return &m_bindings[ index ];
}

const Eliteduino::Bindings::Binding* Eliteduino::Bindings::BindingsManager::GetBinding( uint8_t row, uint8_t column ) const
{
	uint8_t index = MatrixIndex( row, column, m_rowCount );
	return &m_bindings[ index ];
}
