/*
 * StepperMotor.h
 *
 *  Created on: Apr 2, 2014
 *      Author: john
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

#include "GPIOPin.h"

class StepperMotor
{
	public:
		enum class Directions{ Cw, Ccw };
		StepperMotor( GPIOPin *outpDir, GPIOPin *outpPulse, GPIOPin *outpEnable );
		virtual ~StepperMotor();
		bool step( const int nSteps, const Directions dir );
		bool isStepping();
		void enable();
		void disable();

	private:
		bool stepping_;
		GPIOPin *outpDir_;
		GPIOPin *outpPulse_;
		GPIOPin *outpEnable_;
		void step_thread( const int nSteps, const Directions dir );

};

#endif /* STEPPERMOTOR_H_ */
