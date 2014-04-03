/*
 * StepperMotor.cpp
 *
 *  Created on: Apr 2, 2014
 *      Author: john
 */

#include "StepperMotor.h"
#include <thread>
#include <iostream>
#include <chrono>

StepperMotor::StepperMotor( GPIOPin* outpDir, GPIOPin* outpPulse, GPIOPin* outpEnable )
			: stepping_{ false }, outpDir_{ outpDir }, outpPulse_{ outpPulse }, outpEnable_{ outpEnable }
{
	outpDir_->setupOutput();
	outpDir_->setValue( false );
	outpPulse_->setupOutput();
	outpPulse_->setValue( false );
	outpEnable_->setupOutput();
	outpEnable_->setValue( true );
}

StepperMotor::~StepperMotor()
{
	disable();
}

bool StepperMotor::step( const int nSteps, const Directions dir )
{
	if( stepping_ )
		return false;

//	std::thread( &StepperMotor::step_thread, this, nSteps, dir ).detach();
	step_thread( nSteps, dir );
	return true;
}

bool StepperMotor::isStepping()
{
	return stepping_;
}

void StepperMotor::enable()
{
	outpEnable_->setValue( false );
}

void StepperMotor::disable()
{
	outpEnable_->setValue( true );
}

void StepperMotor::step_thread( const int nSteps, const Directions dir )
{
	stepping_ = true;

	if( dir == Directions::Cw )
		outpDir_->setValue( true );
	else
		outpDir_->setValue( false );

	constexpr int max_speed = 700;								//Puls/sec
	const int acc_pulses = 25 > nSteps / 2 ? nSteps / 2 : 20;	//Number of pulses in acceleration
	const std::chrono::milliseconds acc_time( 1 );				//Number of milliseconds to increase/decrease delay time every cycle
	const std::chrono::milliseconds min_delay( 1000 / max_speed );

	std::chrono::milliseconds delay( std::chrono::milliseconds( min_delay + (acc_time * acc_pulses) ) );

	for( int i = 0; i < nSteps; i++ )
	{
		outpPulse_->setValue( true );
		std::this_thread::sleep_for( delay );
		outpPulse_->setValue( false );
		std::this_thread::sleep_for( delay );

		if( i > nSteps - acc_pulses )
			delay += acc_time;
		else if( delay > min_delay )
			delay -= acc_time;
	}

	stepping_ = false;
}
