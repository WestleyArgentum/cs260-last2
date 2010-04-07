#include "precompiled.h"

GlobalDebug GlobalDebug::instance;

GlobalDebug* GlobalDebug::GetInstance(void){
	return &instance;
}

//todo:  can this be done?
//template<int> 
//class ErrorRegistrar
//{
//public:
//	static const char* Message;
//};
//
//__declspec(selectany) ErrorRegistrar::Message = NULL;

GlobalDebug::GlobalDebug(void)
{
	RegisterErrorCode(D3DERR_INVALIDCALL, "Invalid call");
	RegisterErrorCode(D3DERR_CONFLICTINGRENDERSTATE, " onflicting render state");
	RegisterErrorCode(D3DERR_CONFLICTINGTEXTUREFILTER, "Conflicting texture filter");
//	RegisterErrorCode(D3DERR_CONFLICTINGTEXTUREPALLETE, "conflicting texture pallet (whatever that means)");
	RegisterErrorCode(D3DERR_DEVICEHUNG, "Device hung (sucks to be you)");
	RegisterErrorCode(D3DERR_DEVICELOST, "Device lost");
RegisterErrorCode(D3DERR_DEVICENOTRESET, "Device not reset (reset it)");
RegisterErrorCode(D3DERR_DEVICEREMOVED, "Device removed (bring it back)");
RegisterErrorCode(D3DERR_DRIVERINTERNALERROR, "Driver internal error (file a bug)");
RegisterErrorCode(D3DERR_DRIVERINVALIDCALL, "Driver Invalid Call (not used)");
RegisterErrorCode(D3DERR_INVALIDDEVICE, "Invalid device");
RegisterErrorCode(D3DERR_MOREDATA, "More Data - buffer can't hold all the data you're sending");
RegisterErrorCode(D3DERR_NOTAVAILABLE, "does not support queried architecture");
RegisterErrorCode(D3DERR_NOTFOUND, "requested item was not found");
RegisterErrorCode(D3DERR_OUTOFVIDEOMEMORY, "Out of video memory- buy a bigger graphics card");
RegisterErrorCode(D3DERR_TOOMANYOPERATIONS, "Too many operations.  One operation");
RegisterErrorCode(D3DERR_UNSUPPORTEDALPHAARG, "Unsupported alpha arg");
RegisterErrorCode(D3DERR_UNSUPPORTEDALPHAOPERATION, "Unsupported alpha operation");
RegisterErrorCode(D3DERR_UNSUPPORTEDCOLORARG, "Unsupported color arg");
RegisterErrorCode(D3DERR_UNSUPPORTEDCOLOROPERATION, "Unsupported color operation");
RegisterErrorCode(D3DERR_UNSUPPORTEDFACTORVALUE, "Unsupported factor value");
RegisterErrorCode(D3DERR_UNSUPPORTEDTEXTUREFILTER, "Unsupported texture filter");
RegisterErrorCode(D3DERR_WASSTILLDRAWING, "Was still drawing");
RegisterErrorCode(D3DERR_WRONGTEXTUREFORMAT, "Wrong texture format");
RegisterErrorCode(E_FAIL, "failed.  just fail.");
RegisterErrorCode(E_INVALIDARG, "invalid argument (E_)");
//RegisterErrorCode(E_INVALIDCALL, "invalid call (E_)");
RegisterErrorCode(E_NOINTERFACE, "no interface");
//RegisterErrorCode(E_NOIMPL, "Not implemented");
RegisterErrorCode(E_OUTOFMEMORY, "E_ out of memory");
RegisterErrorCode(S_NOT_RESIDENT, "not resident");
RegisterErrorCode(S_RESIDENT_IN_SHARED_MEMORY, "resident in shared memory");
RegisterErrorCode(S_PRESENT_MODE_CHANGED, "Present mode changed");
RegisterErrorCode(S_PRESENT_OCCLUDED, "present occluded");
//RegisterErrorCode(D3DERR_UNSUPPORTEDOVERLAY, "does not support overlay");
//RegisterErrorCode(D3DERR_UNSUPPORTEDOVERLAYFORMAT, "does not support overlay format");
//RegisterErrorCode(D3DERR_CANNOTPROTECTCONTENT, "cannot protect content");
//RegisterErrorCode(D3DERR_UNSUPPORTEDCRYPTO, "unsupported crypto");
//RegisterErrorCode(D3DERR_PRESENT_STATISTICS_DISJOINT, "present statistics disjoint");


}

GlobalDebug::~GlobalDebug(void){
	
}

bool GlobalDebug::AssertAndLogResult(HRESULT result, string methodName)
{
#ifdef LOGGING
	if(result == D3D_OK)
		return true;

	string errMessage;
	errMessage.append(methodName);
	errMessage.append(" - ");

	GlobalDebug* dbg = GlobalDebug::GetInstance();

	if(dbg->errMessages.find(result) == dbg->errMessages.end())
	{
		errMessage.append("-unknown error occurred");
		dbg->DbgOutInt(errMessage, result);
	}

	errMessage.append(dbg->errMessages[result]);
	dbg->DbgOut(errMessage);
	return false;	

#else
	return true;
	#endif
}

void GlobalDebug::DbgOutInt(string label, int value)
{
#ifdef LOGGING

	stringstream strs;
	strs << value;
	string output;
	strs << globalmsgcount++;
	output.append(strs.str());

	label.append(strs.str()) ;
	output.append(label);
	label.append(TEXT("\r\n"));
	const char *c_str =output.c_str() ;

	OutputDebugString( c_str ) ;
#endif
}


void GlobalDebug::DbgOut(string label)
{
#ifdef LOGGING
	stringstream strs;
	strs << globalmsgcount++;
	
	label.append(strs.str());
	label.append(TEXT("\r\n"));
	OutputDebugString( label.c_str() ) ;
#endif
}

void GlobalDebug::DbgOutFloat(string label, float value){
#ifdef LOGGING
	stringstream strs;
	strs << value;
	string output;
	
	strs << globalmsgcount++;
	output.append(strs.str());

	label.append(strs.str()) ;
	output.append(label);
	label.append(TEXT("\r\n"));
	const char *c_str =output.c_str() ;
	OutputDebugString( c_str ) ;
#endif
}


void GlobalDebug::RegisterErrorCode(HRESULT err, string message)
{	
	errMessages[err] = message;	
}