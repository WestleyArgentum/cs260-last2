/**************************************************************************************************/
/*!
@file   Threading.hpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention � Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#include "Threading.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutex Public Methods

/**************************************************************************************************/
/**************************************************************************************************/
Mutex::Mutex( void )
{
  InitializeCriticalSection( &criticalSection_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
Mutex::~Mutex( void ) throw()
{
  DeleteCriticalSection( &criticalSection_ );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutex Private Methods

/**************************************************************************************************/
/**************************************************************************************************/
void Mutex::Acquire( void )
{
  EnterCriticalSection( &criticalSection_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Mutex::Release( void )
{
  LeaveCriticalSection( &criticalSection_ );
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Lock Methods

/**************************************************************************************************/
/**************************************************************************************************/
Lock::Lock( Mutex &mutex ) : mutex_(mutex)
{
  mutex_.Acquire();
}

/**************************************************************************************************/
/**************************************************************************************************/
Lock::~Lock( void ) throw()
{
  mutex_.Release();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Event Methods

/**************************************************************************************************/
/**************************************************************************************************/
Event::Event( void )
{
    // Create a event in a non-signaled state. (red state)
  handle_ = CreateEvent( NULL, FALSE, FALSE, NULL );
}

/**************************************************************************************************/
/**************************************************************************************************/
Event::~Event( void ) throw()
{
  CloseHandle( handle_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Event::Release( void )
{
    // Put this event into a signled state.
  SetEvent( handle_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
DWORD Event::Wait( DWORD milliseconds )
{
    // Wait until this event is in signaled (green) state.
  return WaitForSingleObject( handle_, milliseconds );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread Methods

/**************************************************************************************************/
/**************************************************************************************************/
Thread::Thread( Routine fn, void *arg ) : running(false)
{
  attributes_.nLength              = sizeof(SECURITY_ATTRIBUTES);
  attributes_.lpSecurityDescriptor = NULL;
  attributes_.bInheritHandle       = FALSE;

  handle_ = CreateThread( &attributes_, 0, fn, arg, CREATE_SUSPENDED, &id_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
Thread::~Thread( void ) throw()
{
  CloseHandle( handle_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Thread::Resume( void )
{
  running = true;
  ResumeThread( handle_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Thread::WaitForDeath( void )
{
  running = false;
  WaitForSingleObject( handle_, INFINITE );
}

/**************************************************************************************************/
/**************************************************************************************************/
bool Thread::Terminate( void )
{
  return TerminateThread( handle_, -1 ) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// RoutineObject Methods

/**************************************************************************************************/
/**************************************************************************************************/
RoutineObject::RoutineObject( void ) : dying_(0),
// Passing our RoutineObject to our thread before it is constructed. It is not a flaw because we
//  start our thread's suspended, therefore the thread will use this object at some point after this
//  object is created.
#pragma warning( disable : 4355 )     // 'this' : used in base member initializer list
  thread_( RoutineObject::Routine, this )
#pragma warning( default : 4355 )
{
}

/**************************************************************************************************/
/**************************************************************************************************/
void RoutineObject::Kill( void )
{
  ++dying_;
  FlushThread();
  thread_.WaitForDeath();
}

/**************************************************************************************************/
/**************************************************************************************************/
DWORD WINAPI RoutineObject::Routine( void *arg ) 
{
    // Collect our Routine, then run the Routine until the object is finished.
  RoutineObject *object = reinterpret_cast<RoutineObject*>(arg);

    // Announce that we are starting a particular thread and the id of that thread.
  //std::cout << "Starting thread: " << object->thread_.id_ << std::endl;

  try
  {
    object->InitializeThread();
    object->Run();
  }
  catch ( const std::exception &e )
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }
  catch ( ... )
  {
    std::cerr << "Unknown error occured." << std::endl;
  }

  //std::cout << "Thread exiting: " << object->thread_.id_ << std::endl;
  object->ExitThread();

  return 0;
}

