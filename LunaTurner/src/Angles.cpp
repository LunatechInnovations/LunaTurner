/*
 * Angles.cpp
 *
 *  Created on: Apr 2, 2014
 *      Author: john
 */

#include "Angles.h"
#include <iostream>
#include <stdexcept>

Angles::Angles()
{
}

Angles::~Angles()
{
}

const Angles Angles::fromString( const std::string values )
{
	std::string cw;
	Angles ret;

	for( const auto i : values )
	{
		if( i != ';' )
		{
			cw.push_back( i );
		}
		else
		{
			try { ret.addAngle( std::stod( cw ) ); }
			catch( const std::invalid_argument &e )	{ return Angles(); }
			cw.clear();
		}
	}

	return ret;
}

std::vector<double> Angles::angles()
{
	return angles_;
}

const unsigned int Angles::size()
{
	return angles_.size();
}

void Angles::addAngle( double angle )
{
	angles_.push_back( angle );
}
