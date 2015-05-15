#include "CCaller.h"
#include "CUtils.h"
CSCaller*  CSCaller::getInstance()
{
	static CSCaller  caller;
	return &caller;
}

int CSCaller::DoCallFromExpression(int iCode,CallFunc func,Number & rNum)
{
	bool bCanExcahgne = pState->ExchangeVM(iCode);
	assert(bCanExcahgne  && "无法切换调用栈!");
	func(pState);
	//这里从栈顶弹出数据
	pState->PopNumber(rNum);
	return 0;
}

int CSCaller::NewStack()
{
	return pState->newState();
}

void CSCaller::ClearStack(int iCode)
{
	pState->ClearVMByCode(iCode);
}
bool CSCaller::RegisterFunction(const char* pFuncName, CallFunc func,int iParamLen)
{
	int iHashCode = CharacterUtils::murMurHash(pFuncName,strlen(pFuncName));
	FuncIter  fiter = m_mapFunc.find(iHashCode);
	if (fiter == m_mapFunc.end())
	{
		m_mapFunc.insert(FunctMapType(iHashCode,func));
		CharacterUtils::RegisterUserDefineFunction(string(pFuncName),iParamLen);
		return true;
	}
	return false;
}

int CSCaller::DoCallFromExpressionByName(int iCode, char * pStrName,Number & rNum)
{
	int iHashCode = CharacterUtils::murMurHash(pStrName,strlen(pStrName));
	FuncIter  fiter = m_mapFunc.find(iHashCode);
	assert( fiter!=m_mapFunc.end()  && "当前调用函数未定义!");
	return DoCallFromExpression(iCode,fiter->second,rNum);
}

int CSCaller::DoCallFromExpressionByHashCode(int iCode, int  iFuncHashCode,Number & rNum)
{
	FuncIter  fiter = m_mapFunc.find(iFuncHashCode);
	assert( fiter!=m_mapFunc.end()  && "当前调用函数未定义!");
	return DoCallFromExpression(iCode,fiter->second,rNum);
}


void CSCaller::PushToStack(Number&  oInfo)
{
	if (oInfo.iType == 0)
	{
		pState->PushBool(oInfo.uValue.bData);
	}else if (oInfo.iType ==1)
	{
		pState->PushInt(oInfo.uValue.iData);
	}else if (oInfo.iType ==2)
	{
		pState->PushDouble(oInfo.uValue.dData);
	}

}
