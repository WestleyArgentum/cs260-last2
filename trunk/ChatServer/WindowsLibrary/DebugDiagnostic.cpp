/**************************************************************************************************/
/*!
@file   DebugDiagnostic.cpp
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
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>

  /// The maximum buffer size that we are allowing for users to print at one given point and time.
const unsigned int MAX_BUFFER_SIZE   = 1024 * 4;

  /// Determines the message box flags when signalling an error to the user.
const unsigned int MESSAGE_BOX_FLAGS = MB_ICONEXCLAMATION;

/**************************************************************************************************/
/**************************************************************************************************/
void DebugPrintHandler( const char *msg , ... )
{
  char buffer[MAX_BUFFER_SIZE] = {0};

  va_list args;
  va_start( args, msg );

    // Append a newline to the end of our string.
  sprintf_s( buffer, MAX_BUFFER_SIZE, "%s\n", msg );

    // Basically sprintf, but where we have to pull off the va list since our function uses the
    //  ellipses operator.
  vsnprintf_s( buffer, MAX_BUFFER_SIZE, _TRUNCATE, buffer, args );
  va_end( args );

    // Print the information within our buffer into Visual Studio's output window.
  OutputDebugString( buffer );
}

/**************************************************************************************************/
/**************************************************************************************************/
bool SignalErrorHandler( const char *file, unsigned int line, const char *msg, ... )
{
  char buffer[MAX_BUFFER_SIZE] = {0};

    // Check to see if we even have a message to print to our screen...
  if ( msg != NULL )
  {
    va_list args;
    va_start( args, msg );

      // Format the error message that occured similar to a compiler error/warning so you can click
      //  in the output window to bring you to the file and line where the break was triggered.
    sprintf_s( buffer, MAX_BUFFER_SIZE, "%s(%d) : %s\n", file, line, msg );

      // Concatinate the variatic arguments into our buffer.
    vsnprintf_s( buffer, MAX_BUFFER_SIZE, _TRUNCATE, buffer, args );
    va_end( args );
  }
  else
  {
      // Print that no error message has been specified to display.
    strcpy_s( buffer, MAX_BUFFER_SIZE, "No Error Message.\n" );
  }

  OutputDebugString( buffer );

    // Display our message to the user notifying what happened through a message box.
  MessageBox( NULL, buffer, "Error!", MESSAGE_BOX_FLAGS );

  return true;
}

