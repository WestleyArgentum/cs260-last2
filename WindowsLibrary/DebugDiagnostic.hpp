#pragma once

#ifdef _MSC_VER
    /// Causes the program to halt execution. (Prompts to run the debugger, Microsoft only.)
  #define DEBUG_BREAK_ __debugbreak()
#else
    /// Causes the program to halt execution. (Throws an interrupt, other compiler.)
  #define DEBUG_BREAK_ do { _asm int 3 } while( 0 )
#endif      // _MSC_VER

  // Enable our debug diagnostics for our game.
#if !defined ( ENABLE_DEBUG_DIAGNOSTICS_ )
  #if defined ( _DEBUG )
    #define ENABLE_DEBUG_DIAGNOSTICS_ 1
  #else
    #define ENABLE_DEBUG_DIAGNOSTICS_ 0
  #endif      // define ( _DEBUG )
#endif      // !define ( ENABLE_DEBUG_DIAGNOSTICS_ )

  /// Print debug information into Visual Studio's output window.
void DebugPrintHandler( const char *msg, ... );

  /// Print a message to the screen displaying to the user the logic error that has occured.
bool SignalErrorHandler( const char *expression, const char *file, unsigned int line,
                         const char *msg = 0, ... );

  // If we have our DEBUG_DIAGNOSTICS enabled... include our Printing/Assert Macros.
#if ENABLE_DEBUG_DIAGNOSTICS_

    /// Macro used to print a message to the debug window.
  #define DebugPrint( ... ) DebugPrintHandler( __VA_ARGS__ )

    /// Macro used to print an error message if the Predicate evaluates to true
  #define ErrorIf( Predicate, ... ) \
    do \
    { \
      if ( ( Predicate ) && SignalErrorHandler( #Predicate, __FILE__, __LINE__, __VA_ARGS__ ) ) \
      { \
        DEBUG_BREAK_; \
      } \
    } while ( 0 )

#else
    // Strip our DEBUG_DIAGNOSTICS from our code.

  #define DebugPrint( ... ) ( (void) 0 )
  #define ErrorIf( ... ) ( (void) 0 )

#endif      // ENABLE_DEBUG_DIAGNOSTICS_

