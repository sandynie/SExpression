#ifndef  _CCALLER_H
#define _CCALLER_H
#include "CVMState.h"
#include "CUtils.h"
#include <assert.h>
#include <map>
using std::map;
typedef int (*CallFunc)(CVMState* pVM);
class CSCaller
{
public:
	typedef map<int, CallFunc>::iterator  FuncIter;
	typedef map<int, CallFunc>::value_type  FunctMapType;
	static CSCaller* getInstance();
	int NewStack();
	int DoCallFromExpression(int iCode, CallFunc  func,Number & rNum);  //从脚本调用C++
	int DoCallFromExpressionByName(int iCode, char * pStrName,Number & rNum);  //从脚本调用C++
	int DoCallFromExpressionByHashCode(int iCode, int  iFuncHashCode,Number & rNum); 
	bool RegisterFunction(const char* pFuncName, CallFunc func, int iParamLen);
	void PushToStack(Number&  oInfo);
	void ClearStack(int iCode);
protected:
private:
	CVMState*  pState;
	CSCaller(){pState = new CVMState();}
	map<int, CallFunc > m_mapFunc;
};
#endif
