#include "WindowsLibrary/Header.hpp"

struct DisplayProcess : public ICommandProcess
{
  DisplayProcess( Textbox *displaybox ) : displaybox_(displaybox) {;}
  virtual void operator()( const Command &command );

  Textbox *displaybox_;
};    // struct DisplayProcess


struct NewUserProcess : public ICommandProcess
{
  NewUserProcess( Listbox *listbox ) : listbox_(listbox) {;}
  virtual void operator()( const Command &command );

  Listbox *listbox_;
};    // struct NewUserProcess


struct RemoveUserProcess : public ICommandProcess
{
  RemoveUserProcess( Listbox *listbox ) : listbox_(listbox) {;}
  virtual void operator()( const Command &command );

  Listbox *listbox_;
};    // struct RemoveUserProcess


struct SendFileProcess : public ICommandProcess
{
  virtual void operator()( const Command &command );
};    // struct SendFileProcess

