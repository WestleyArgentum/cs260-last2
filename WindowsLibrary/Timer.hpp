#pragma once

#include <windows.h>      // LARGE_INTEGER, QueryPerformanceCounter, QueryPerformanceFrequency

class Timer
{
  public:
    Timer( void );

    void Start( void );
    void Stop( void );

    void Pause( void );
    void Resume( void );

    const double& TimeElapsed( void );

    bool IsPaused( void ) const { return paused_; }

  private:
    LARGE_INTEGER start_;       ///< Holds the time when we started our timer.
    LARGE_INTEGER end_;

      /// Contains the frequency of the processor used to help calculate time.
    LARGE_INTEGER frequency_;

      /// Keeps track of how much time has elapsed since the start of our timer (milliseconds)
    double elapsed_;
      /// Stores our elasped time if we pause the timer and want to restart it!
    double previous_;
    bool paused_;           ///< Flag that states if our Timer is in a "Paused" state.
    bool done_;             ///< Specifies if the timer is done. ( End() is called )

    void ResetTimer( void );

    double SecondsPassed( void ) const;

    void TrueStart( void );

    void UpdateTime( void );
};    // class Timer

