/**************************************************************************************************/
/*!
@file   ClientProcesses.hpp
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
#include "Client.hpp"
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

struct StartFileTransferProcess : public ICommandProcess
{
  StartFileTransferProcess( Client *client ) : client_(client) {;}
  virtual void operator()( const Command &command );

  Client *client_;
};

struct SendFileTransferInfoProcess : public ICommandProcess
{
  SendFileTransferInfoProcess( Client * client ) : client_(client) {;}
  virtual void operator()( const Command &command );

  Client *client_;
};    // struct SendFileProcess

struct SendMessageProcess : public ICommandProcess
{
  SendMessageProcess( Client *client ) : client_(client) {;}
  virtual void operator() ( const Command &command );

  Client *client_;
};

struct ErrorBoxProcess : public ICommandProcess
{
	virtual void operator() ( const Command &command );
};
