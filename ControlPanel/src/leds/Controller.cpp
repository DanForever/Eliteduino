#include "Controller.h"

#include <Arduino.h>

#include "Matrix.h"
#include "../PCCommunications.h"
#include "../bindings/Binding.h"
#include "../debug/Debug.h"

void Eliteduino::Leds::Controller::Initialize( const LedMatrixConfig& matrix, PCCommunications* comms )
{
	m_comms = comms;

	m_matrix = new Matrix();

	m_matrix->Initialize( matrix.Pin.Din, matrix.Pin.Clk, matrix.Pin.Cs, matrix.Dimensions.RowCount, matrix.Dimensions.ColumnCount, comms );
	m_matrix->SetAll( true );
}

void Eliteduino::Leds::Controller::Initialize( const LedPinConfig& pins, PCCommunications* comms )
{
	m_comms = comms;

}

void Eliteduino::Leds::Controller::Initialize( const LedMatrixConfig& matrix, const LedPinConfig& pins, PCCommunications* comms )
{
	m_comms = comms;

}

void Eliteduino::Leds::Controller::SetBinding( const MatrixAddress& led, const Bindings::Binding* binding )
{
	if ( binding->ControlRole != eControlRole::Undefined )
	{
		PRINT( "Set binding for: ", (uint8_t)binding->ControlRole, " to ", led.Row, ", ", led.Column );

		m_bindings[ (uint8_t)binding->ControlRole ] = led;
	}
}

void Eliteduino::Leds::Controller::SetBinding( PinAddress led, const Bindings::Binding* binding )
{
}

void Eliteduino::Leds::Controller::Update()
{
	const uint8_t size = static_cast<uint8_t>(eControlRole::Count);

	for ( uint8_t i = 0; i < size; ++i )
	{
		const AnimationData& animation = m_animations[ i ];

		if ( !animation.Animating )
			continue;

		switch ( animation.Type )
		{
		case AnimationType::Blink:
			UpdateAnimationBlink( static_cast<eControlRole>(i), animation );
			break;

		default:
			break;
		}
	}
}

void Eliteduino::Leds::Controller::ProcessMessage( const Message& message )
{
	PRINT( "LED Controller received message of type: ", (uint8_t)message.Data.Type );

	switch ( message.Data.Type )
	{
	case MessageType::Data:
		ProcessStatReport( message );
		break;

	default:
		break;
	}
}

void Eliteduino::Leds::Controller::ProcessStatReport( const Message& message )
{
	constexpr uint8_t StatTypeBufferPosition = 0;
	constexpr uint8_t DataBufferPosition = 1;

	StatType statType = static_cast<StatType>( message.Data.Buffer[ StatTypeBufferPosition ] );
	const uint8_t* data = message.Data.Buffer + DataBufferPosition;
	bool isActive = (bool)( *data );

	PRINT( "Recieved stat report: ", (uint8_t)statType, " active: ", isActive );

	switch ( statType )
	{
	case StatType::MassLocked:
		ToggleAnimation( eControlRole::Supercruise, AnimationType::Off, isActive );
		ToggleAnimation( eControlRole::FrameshiftJump, AnimationType::Off, isActive );
		break;

	case StatType::FsdCooldown:
		ToggleAnimation( eControlRole::Supercruise, AnimationType::Off, isActive );
		ToggleAnimation( eControlRole::FrameshiftJump, AnimationType::Off, isActive );
		break;

	case StatType::Overheating:
		ToggleAnimation( eControlRole::Heatsink, AnimationType::Blink, isActive );
		break;

	case StatType::MenuGalaxyMap:
		ToggleAnimation( eControlRole::GalaxyMap, AnimationType::Blink, isActive );
		break;

	case StatType::MenuSystemMap:
		ToggleAnimation( eControlRole::SystemMap, AnimationType::Blink, isActive );
		break;

	default:
		break;
	}
}

void Eliteduino::Leds::Controller::Set( const Address& address, bool on )
{
	if ( !address.IsSet )
		return;

	if ( address.IsMatrixAddress )
	{
		m_matrix->Set( address.MatrixAddr.Row, address.MatrixAddr.Column, on );
	}
	else
	{
		// todo
	}
}

void Eliteduino::Leds::Controller::ToggleAnimation( eControlRole role, AnimationType animation, bool play )
{
	if ( play )
	{
		PlayAnimation( role, animation );
	}
	else
	{
		StopAnimation( role );
	}
}

void Eliteduino::Leds::Controller::PlayAnimation( eControlRole role, AnimationType type )
{
	const uint8_t index = (uint8_t)role;
	const Address address = m_bindings[ index ];

	// If there's no led bound to the role, then just do nothing
	if ( !address.IsSet )
	{
		PRINT( "Cannot play animation, No address associated with role: ", (uint8_t)role );
		return;
	}

	const unsigned long now = millis();
	AnimationData& animation = m_animations[ index ];

	animation.StartTime = now;
	animation.Type = type;
	animation.Animating = true;

	// Put the LED into its starting state where appropriate
	switch ( type )
	{
	case AnimationType::Off:
		Set( address, false );
		break;

	default:
		break;
	}
}

void Eliteduino::Leds::Controller::UpdateAnimationBlink( eControlRole role, const AnimationData& animation )
{
	const unsigned long now = millis();
	const unsigned long interval = 250; // Blink 4 times a second

	const unsigned long duration = now - animation.StartTime;
	
	const bool shouldBeOff = ( ( duration / interval ) % 2 ) == 0;

	const uint8_t index = (uint8_t)role;
	const Address address = m_bindings[ index ];

	Set( address, !shouldBeOff );
}

void Eliteduino::Leds::Controller::StopAnimation( eControlRole role )
{
	const uint8_t index = (uint8_t)role;
	const Address address = m_bindings[ index ];

	// If there's no led bound to the role, then just do nothing
	if ( !address.IsSet )
		return;

	AnimationData& animation = m_animations[ index ];
	animation.Animating = false;

	// The default state for all LEDs is turned on, so that's what we do when an animation ends
	Set( address, true );
}
