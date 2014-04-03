/*
 * Angles.h
 *
 *  Created on: Apr 2, 2014
 *      Author: john
 */

#ifndef ANGLES_H_
#define ANGLES_H_

#include <vector>
#include <string>

class Angles
{
	public:
		Angles();
		virtual ~Angles();
		static const Angles fromString( const std::string values );
		std::vector<double> angles();
		const unsigned int size();

	protected:
		void addAngle( double angle );

	private:
		std::vector<double> angles_;
};

#endif /* ANGLES_H_ */
