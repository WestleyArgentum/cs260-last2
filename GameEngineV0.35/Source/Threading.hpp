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
#pragma once

class Mutex
{
  public:
    Mutex( void );
    ~Mutex( void ) throw();

    void Acquire( void );
    void Release( void );

  private:
    CRITICAL_SECTION criticalSection_;

      // Not implemented.
    Mutex( const Mutex& );
};    // class Mutex

class Lock
{
  public:
    Lock( Mutex &mutex );
    ~Lock( void ) throw();

  private:
      // Not implemented.
    Lock( const Lock &);
      // Note: Cannot be generated by the compiler since we have a reference member.
    Lock& operator=( const Lock& );

    Mutex &mutex_;
};    // class Lock

class Event
{
  public:
    Event( void );
    ~Event( void ) throw();

    void Release( void );
    DWORD Wait( DWORD milliseconds );

    //operator HANDLE ( void ) { return handle_; }

  private:
    HANDLE handle_;
};    // class Event

class Thread
{
  public:
    friend class RoutineObject;

  public:
      // Prototype: DWORD ( WINAPI *Routine )( void* arg );
    typedef LPTHREAD_START_ROUTINE Routine;

    Thread( Routine fn, void *arg );
    ~Thread( void ) throw();

    void Resume( void );

	  bool IsRunning( void ) const { return running; }
    void WaitForDeath( void );
    bool Terminate( void );

  private:
    SECURITY_ATTRIBUTES attributes_;
    HANDLE handle_;   ///< Handle to our thread.
    DWORD id_;        ///< Thread id.
	bool running;     ///< If thread is running.
};    // class Thread

class RoutineObject
{
  public:
    RoutineObject( void );
    virtual ~RoutineObject( void ) throw() {;}
    void Kill( void );

  protected:
    virtual void InitializeThread( void ) = 0;
    virtual void Run( void ) = 0;
    virtual void ExitThread( void ) throw() = 0;

    virtual void FlushThread( void ) = 0;   // Called when killed is called.

      // Defines the Routine that each Thread calls upon each RoutineObject.
    static DWORD WINAPI Routine( void *arg );

    int dying_;
    Thread thread_;
};    // class ActiveObject
