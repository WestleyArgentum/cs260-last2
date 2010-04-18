///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Precompiled.h
///	Precompiled Header file which includes the large unchanging header of stl, windows
///	DirectX. These are 'precompiled once' and used for every cpp. This helps to 
///	greatly reduce compile times.
///
///	Authors: Chris Peters
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////

  // Removing the warning
  //  - C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable : 4290 )

#include "WindowsIncludes.h"
#include "DirectXIncludes.h"
#include "Containers.h"
#include "DebugDiagnostic.h"
#include <algorithm>

