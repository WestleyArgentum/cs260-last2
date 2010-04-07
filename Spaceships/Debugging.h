#pragma once
#include <map>
using namespace std;

struct errCompare
{
	bool operator()(const HRESULT err, const HRESULT input) const
	{
		return err == input;
	}
};

class GlobalDebug{

public:
	virtual ~GlobalDebug(void);
	static GlobalDebug* GetInstance(void);
	void DbgOutInt(string label, int value ); 
	void DbgOut(string label);
	void DbgOutFloat(string label, float value);
	static bool AssertAndLogResult(HRESULT result, string methodName);
	void RegisterErrorCode(HRESULT err, string message);
private:
	
	GlobalDebug(void);

	map<HRESULT,string> errMessages;
	
	


	int globalmsgcount;
	static GlobalDebug instance;

};
