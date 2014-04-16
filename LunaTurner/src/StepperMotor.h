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
		void step( const int nSteps, const Directions dir );
		void enable();
		void disable();

	private:
		GPIOPin *outpDir_;
		GPIOPin *outpPulse_;
		GPIOPin *outpEnable_;

};

#endif /* STEPPERMOTOR_H_ */
