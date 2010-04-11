/**************************************************************************************************/
/*!
@file   Timer.cpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention © Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#include "Precompiled.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer Public Methods

/**************************************************************************************************/
/**************************************************************************************************/
Timer::Timer( void ) : elapsed_(0.0), previous_(0.0), paused_(false), done_(false)
{
    // Initialize our timers internal frequency
  QueryPerformanceFrequency( &frequency_ );

    // And implicitly start our timer for our client so they can get the time passed since the
    //  creation of this object!
  TrueStart();
}

/**************************************************************************************************/
/**************************************************************************************************/
void Timer::Start( void )
{
    // Resets our time...
  ResetTimer();

    // Then restarts our timer.
  TrueStart();
}

/**************************************************************************************************/
/**************************************************************************************************/
void Timer::Stop( void )
{
  if ( !done_ )
  {
      // Stop our timer.
    UpdateTime();

    done_ = true;
    paused_ = false;    // Remove our timers paused state.
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Timer::Pause( void )
{
    // If our timer is currently done... (aka, not in a running state...)
  if ( done_ || !paused_ )
  {
      // Don't do anything since our timer hasn't been started yet.
    return;
  }

    // If our timer is currently unpaused.
  if ( !paused_ )
  {
    UpdateTime();
    previous_ += elapsed_;

    paused_ = true;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Timer::Resume( void )
{
    // Ignore resuming our timer if the timer is in a unpaused or idle state.
  if ( ( !paused_ ) || done_ )
  {
    return;
  }

    // Re-"start" our timer
  TrueStart();
}

/**************************************************************************************************/
/**************************************************************************************************/
const double& Timer::TimeElapsed( void )
{
    // Update time elapsed if our timer is not stopped and unpaused.
  if ( !done_ && !paused_ )
  {
      // Otherwise update our timer's end time.
    UpdateTime();

      // And calculate how many seconds have passed since
    elapsed_ = previous_ + SecondsPassed();
  }

  return elapsed_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer Private Methods

/**************************************************************************************************/
/**************************************************************************************************/
void Timer::ResetTimer( void )
{
  elapsed_ = 0.0;
  previous_ = 0.0;
  paused_ = false;
  done_ = false;
}

/**************************************************************************************************/
/**************************************************************************************************/
void Timer::TrueStart( void )
{
    // Mark our timer as a unpaused and running state.
  paused_ = false;
  done_ = false;

    // Start our timer.
  QueryPerformanceCounter( &start_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
double Timer::SecondsPassed( void ) const
{
    // Calculate how many seconds have passed from the start to end time.
  return ( end_.QuadPart - start_.QuadPart ) / static_cast<double>( frequency_.QuadPart );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Timer::UpdateTime( void )
{
  QueryPerformanceCounter( &end_ );
}

