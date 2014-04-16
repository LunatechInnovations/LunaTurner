#include <iostream>
#include <vector>
#include "GPIO.h"
#include "StepperMotor.h"
#include <chrono>
#include <thread>
#include "Angles.h"
#include <cmath>
#include "Server.h"

#define SMALL_WHEEL 30.56f
#define LARGE_WHEEL 68.75f

using namespace std;

void waitForInput( GPIOPin *input, const bool status )
{
	while( input->getValue() != status )
		this_thread::sleep_for( chrono::milliseconds( 200 ) );
}

int main( int argc, char **argv )
{
	GPIOPin *outpDir = GPIO::Instance()->getPin( 18 );
	GPIOPin *outpPulse = GPIO::Instance()->getPin( 15 );
	GPIOPin *outpEnable = GPIO::Instance()->getPin( 14 );
	StepperMotor motor( outpDir, outpPulse, outpEnable );

//	GPIOPin *outpPLC = GPIO::Instance()->getPin( 23 );
	GPIOPin *inpPLC = GPIO::Instance()->getPin( 24 );
	inpPLC->setupInput();

	cout << "Enabling motor.\n";
	motor.enable();

	cout << "Starting server\n";
	Server server{ 2345 };

	while( true )
	{
		cout << "Waiting for connection.\n";

		if( !server.waitForConnection() )
		{
			cout << "Connection failed.\n";
			continue;
		}

		cout << "Waiting for angles.\n";
		string msg = server.waitForAngles();
		cout << "size: " << msg.size() << endl;
		if( msg.empty() )
			continue;

		Angles a{ Angles::fromString( msg ) };
		vector<double> list = a.angles();

		double current_angle = 0.0f;
		int total_steps = 0;

		for( const auto i : list )
		{
			cout << "Turning\n";
			waitForInput( inpPLC, false );

			double nSteps = ((i - current_angle) / (360.0f / 400.0f)) * (LARGE_WHEEL / SMALL_WHEEL);

			motor.step( (int)(round( nSteps )), StepperMotor::Directions::Cw );

			current_angle = i;
			total_steps += round( nSteps );

			waitForInput( inpPLC, true );
		}
		waitForInput( inpPLC, false );

		motor.step( (int)(round( total_steps )), StepperMotor::Directions::Ccw );

		waitForInput( inpPLC, true );
	}

	return 1;
}
