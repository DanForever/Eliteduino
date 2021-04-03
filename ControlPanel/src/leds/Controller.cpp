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
		if ( isActive )
		{
			PlayAnimationOff( eControlRole::Supercruise );
			PlayAnimationOff( eControlRole::FrameshiftJump );
		}
		else
		{
			StopAnimation( eControlRole::Supercruise );
			StopAnimation( eControlRole::FrameshiftJump );
		}

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

void Eliteduino::Leds::Controller::PlayAnimationOff( eControlRole role )
{
	const uint8_t index = (uint8_t)role;
	const Address address = m_bindings[ index ];

	// If there's no led bound to the role, then just do nothing
	if ( !address.IsSet )
	{
		PRINT( "Cannot play \"off\" animation, No address associated with role: ", (uint8_t)role );
		return;
	}

	const unsigned long now = millis();
	AnimationData& animation = m_animations[ index ];

	animation.StartTime = now;
	animation.Type = AnimationType::Off;
	animation.Animating = true;

	Set( address, false );
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
