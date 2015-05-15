#ifndef _CVMSTATE_H
#define _CVMSTATE_H
#include <map>
using std::map;
#include "CStack.h"
#include "CRandom.h"
#include "CCaculator.h"
//虚拟机
class CVMState
{
public:
	CVMState();
	~CVMState();
	int newState();
	bool ExchangeVM(int iCode);
	bool IsBool();
	bool IsNumber();
	bool IsString();
	bool IsUserData();
	bool IsNull();
	void ClearVM(CStack* pStack);
	void ClearVMByCode(int iCode);
	void PushBool(bool bValue);
	void PushShort( short sValue);
	void PushInt( int iValue);
	void PushDouble( int dValue);
	void PushString(char* pStr);
	void PushUserData( void * pVoid);
	void PushNull();
	//  必须是 number 或 string否则返回0
	void Pop();
	bool PopBool();
	short PopShort();
	int PopInt();
	void PopNumber(Number& rNum);
	double PopDouble();
	char* PopString();
	char* PopUserData();
	void PopNull();
	typedef map<int ,CStack*>::iterator miterator;
	typedef map<int, CStack*>::value_type mvalute;
protected:
private:
	ccRandomGenerator  m_Random;
	CStack  *pCurrentStack;
	map<int ,CStack* > m_vStackMap;
};
#endif
