#include "Components.hpp"

#include "WindowsLibrary/CommandCenter.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// ComponentInfo Methods

/**************************************************************************************************/
/**************************************************************************************************/
ComponentInfo::ComponentInfo( DWORD style, unsigned posx, unsigned posy, unsigned width,
  unsigned height ) : style_(style), posx_(posx), posy_(posy), width_(width), height_(height)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Textbox Methods

/**************************************************************************************************/
/**************************************************************************************************/
Textbox::Textbox( const std::string &name, const ComponentInfo &info ) : name_(name), info_(info)
{
}

/**************************************************************************************************/
/**************************************************************************************************/
HWND Textbox::Create( HWND parent, HINSTANCE hInstance )
{
  handle_ = CreateWindow(
    "EDIT",
    name_.c_str(),
    info_.style_,                 // Style options for this textbox
    info_.posx_, info_.posy_,     // Textbox position on our window.
    info_.width_, info_.height_,  // Textbox height and width
    parent,                       // Attatch this textbox to the window specified,
    NULL,
    hInstance,                    // Within this application.
    NULL );

  SetTextLimit( 10000 );

  return handle_;
}

/**************************************************************************************************/
/**************************************************************************************************/
void Textbox::Init( void )
{
  wndProc_ = (WNDPROC)SetWindowLong( handle_, GWL_WNDPROC, (LPARAM)EditProc );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Textbox::SetTextLimit( unsigned limit )
{
  limit_ = limit;

  SendMessage( handle_, EM_SETLIMITTEXT, limit, 0 );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Textbox::SetText( const std::string &str )
{
  SendMessage( handle_, WM_SETTEXT, 0, (LPARAM)str.c_str() );
  SendMessage( handle_, WM_VSCROLL, SB_BOTTOM, NULL );
}

/**************************************************************************************************/
/**************************************************************************************************/
std::string Textbox::GetText( void )
{
  char *buffer = new char[limit_];
  std::string str;
  int characters = (int)SendMessage( handle_, WM_GETTEXT, limit_, (LPARAM)buffer );

  str.assign( buffer, characters );
  delete buffer;

  return str;
}

/**************************************************************************************************/
/**************************************************************************************************/
LRESULT CALLBACK Textbox::EditProc( HWND hWndEdit, UINT msg, WPARAM wParam, LPARAM lParam )
{
  Textbox *textbox = WinSys->HasComp<Textbox*>( hWndEdit );
  MSG message;

  switch ( msg )
  {
    case WM_KEYDOWN:
		  switch (wParam)
		  {
			  case VK_RETURN:
          {
              // Get the message that the client would like to post.
            std::string str = textbox->GetText();
            textbox->Clear();

              // Post this message so someone can process this message.
            CommandCenter->PostMsg( str, CID_SendMessage );

				    PeekMessage( &message, hWndEdit, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE );
          }
				  break;
		  }
      break;
  }

	return CallWindowProc( textbox->wndProc_, hWndEdit, msg, wParam, lParam );
}


/**************************************************************************************************/
/**************************************************************************************************/
void Textbox::AddText( const std::string &str )
{
    // Get the current text within our window so we don't clear it on the client.
  std::string current = GetText();
  unsigned size = static_cast<unsigned>( current.size() + str.size() );

    // If we reached the end of our buffer, start pushing our old data out to make room for the new
    //  data comming in!
  if ( size > limit_ )
  {
    current = current.substr( str.size() );
  }

  current += str;   // Add the current text to the end of this string.

    // Now set the text to have the 'str' passed in added to the textbox.
  SetText( current );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Textbox::Clear( void )
{
  SetText( "" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Button Methods

/**************************************************************************************************/
/**************************************************************************************************/
Button::Button( const std::string &name, int id, const ComponentInfo &info )
  : name_(name), info_(info), id_(id)
{
}

/**************************************************************************************************/
/**************************************************************************************************/
HWND Button::Create( HWND parent, HINSTANCE hInstance )
{
  handle_ = CreateWindow(
    "BUTTON",
    name_.c_str(),
    info_.style_,                 // Style options for this textbox
    info_.posx_, info_.posy_,     // Textbox position on our window.
    info_.width_, info_.height_,  // Textbox height and width
    parent,                       // Attatch this textbox to the window specified,
    (HMENU)id_,                   // Give this button a particular command id so we can reference it
                                  //  as such when checking for button click.
    hInstance,                    // Within this application.
    NULL );

  return handle_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Listbox Methods

/**************************************************************************************************/
/**************************************************************************************************/
Listbox::Listbox( const std::string &name, const ComponentInfo &info ) : name_(name), info_(info)
{
}

/**************************************************************************************************/
/**************************************************************************************************/
HWND Listbox::Create( HWND parent, HINSTANCE hInstance )
{
  handle_ = CreateWindow(
    "LISTBOX",
    name_.c_str(),
    info_.style_,                 // Style options for this textbox
    info_.posx_, info_.posy_,     // Textbox position on our window.
    info_.width_, info_.height_,  // Textbox height and width
    parent,                       // Attatch this textbox to the window specified,
    NULL,
    hInstance,                    // Within this application.
    NULL );

  return handle_;
}

/**************************************************************************************************/
/**************************************************************************************************/
void Listbox::AddString( const std::string &str )
{
  Lock lock( mutex_ );

  SendMessage( handle_, LB_ADDSTRING, 0, (LPARAM)str.c_str() );
}

/**************************************************************************************************/
/**************************************************************************************************/
void Listbox::RemoveString( const std::string &str )
{
  Lock lock( mutex_ );

    // Attempt to find the string of ours to remove from this listbox (if we find it.)
  LRESULT index = SendMessage( handle_, LB_FINDSTRING, (WPARAM)-1, (LPARAM)str.c_str() );

  if ( index != LB_ERR )
  {
      // If found, remove this string.
    SendMessage( handle_, LB_DELETESTRING, index, 0 );
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
const char* Listbox::GetSelected( void )
{
  Lock lock( mutex_ );

  LRESULT index = SendMessage( handle_, LB_GETCURSEL, 0, 0 );

  if ( index != LB_ERR )
  {
    memset( selected_, 0, sizeof(selected_) );

    SendMessage( handle_, LB_GETTEXT, (WPARAM)index, (LPARAM)selected_ );

    return selected_;
  }
  else
  {
    return NULL;
  }
}

