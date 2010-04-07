#include "MainWindProc.h"

extern MessageHandler* G_globalMessageHandler;

LRESULT CALLBACK MainWindProc( HWND hWnd, UINT msg, WPARAM wParam,
   LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hDC;
	//int i;
	MessageHandler* handler = Game::GetWindProcHandler();


   switch( msg ) {

/**************************************************************\
*     WM_PAINT:                                                *
\**************************************************************/

      case WM_PAINT:
  
		 hDC = BeginPaint( hWnd, &ps );
     	
         EndPaint( hWnd, &ps );
         break;

/**************************************************************\
*     WM_COMMAND:                                              *
\**************************************************************/

      /*case WM_COMMAND:
         switch( wParam ) {
            case IDM_ABOUT:
               DialogBox( ghInstance, "AboutDlg", hWnd, (DLGPROC)
                          AboutDlgProc );
            break;
         }
      break;

/**************************************************************\
*     WM_DESTROY: PostQuitMessage() is called                  *
\**************************************************************/

      case WM_DESTROY:
         PostQuitMessage( 0 );
         break;

/**************************************************************\
*     THE WM_TIMER: Update timer has called for an update	   *
\**************************************************************/
	  
	  //case WM_CHAR:
		 // //dispatch this thing to the controller and move along
		 // handler->PassMessage(handler->GetMessageCreator()->CreateMessage(wParam));

		 // break;

	  case WM_KEYDOWN:
		  handler->PassMessage(handler->GetMessageCreator()->CreateMessage(wParam));
		  break;

	/*  case WM_KEYUP:
		  handler->PassMessage(handler->GetMessageCreator()->CreateMessage(MessageSource::KEYUP, wParam));
		  break;*/

/**************************************************************\
*     Let the default window proc handle all other messages    *
\**************************************************************/

      default:
         return( DefWindowProc( hWnd, msg, wParam, lParam ));
   }

  return( DefWindowProc( hWnd, msg, wParam, lParam ));
}