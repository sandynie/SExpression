#include "CVMState.h"
#include <assert.h>
CVMState::CVMState():pCurrentStack(NULL)
{
	m_Random.ReSet();
}

CVMState::~CVMState()
{
	miterator mbegin = m_vStackMap.begin();
	miterator mend = m_vStackMap.end();
	while (mbegin != mend)
	{
		delete mbegin->second;
		m_vStackMap.erase(mbegin++);
	}
}

bool CVMState::IsBool()
{
	assert(pCurrentStack!=NULL && "无效栈");
	return (pCurrentStack->Top().iType== 0) ;
}


bool CVMState::IsNumber()
{
	assert(pCurrentStack!=NULL && "无效栈");
	int iType = pCurrentStack->Top().iType;
	return (iType==2 || iType==3 || iType==4);
}

bool CVMState::IsString()
{
	assert(pCurrentStack!=NULL && "无效栈");
	return (pCurrentStack->Top().iType== 4) ;
}

bool CVMState::IsUserData()
{
	assert(pCurrentStack!=NULL && "无效栈");
	return (pCurrentStack->Top().iType== 5) ;
}

void CVMState::ClearVM(CStack* pStack)
{
	assert(pCurrentStack!=NULL && "无效栈");
	int iSignal = pStack->getSignal();
	miterator mFinder = m_vStackMap.find(iSignal);
	if (mFinder != m_vStackMap.end())
	{
		delete mFinder->second;
		m_vStackMap.erase(mFinder);
	}
	if (pStack == pCurrentStack)
	{
		pCurrentStack =NULL;
	}
}

void CVMState::ClearVMByCode(int iCode)
{
	miterator mFinder = m_vStackMap.find(iCode);
	if (mFinder != m_vStackMap.end())
	{
		CStack* pStack = mFinder->second;
		if (pStack == pCurrentStack)
		{
			pCurrentStack =NULL;
		}
		delete mFinder->second;
		m_vStackMap.erase(mFinder);
	}

}


void CVMState::PushBool( bool bValue)
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack  stValue;
	stValue.iType = 0;
	stValue.uMsg.bValue = bValue;
	pCurrentStack->Push(stValue);

}

void CVMState::PushShort(short sValue)
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack  stValue;
	stValue.iType = 1;
	stValue.uMsg.sValue = sValue;
	pCurrentStack->Push(stValue);
}
void CVMState::PushInt(int iValue)
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack  stValue;
	stValue.iType =2;
	stValue.uMsg.iValue = iValue;
	pCurrentStack->Push(stValue);
}

void CVMState::PushDouble(int dValue)
{	
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack  stValue;
	stValue.iType = 3;
	stValue.uMsg.dValue = dValue;
	pCurrentStack->Push(stValue);
}

void CVMState::PushString(char* pStr)
{
	assert(pCurrentStack!=NULL && pStr!=NULL && "无效栈");
	stStatueStack  stValue;
	stValue.iType = 4;
	stValue.uMsg.dPointer = pStr;
	pCurrentStack->Push(stValue);
}

void CVMState::PushUserData( void * pVoid)
{
	assert(pCurrentStack!=NULL && pVoid !=NULL&& "无效栈");
	stStatueStack  stValue;
	stValue.iType = 5;
	stValue.uMsg.dPointer = (char*)pVoid;
	pCurrentStack->Push(stValue);
}

int CVMState::newState()
{
	int iCode = m_Random.Get();
	miterator mFinder = m_vStackMap.find(iCode);
	while (mFinder!=m_vStackMap.end())
	{
		iCode = m_Random.Get();
	}
	CStack* pStack = new CStack();
	pStack->SetSignal(iCode);
	m_vStackMap.insert(mvalute(iCode,pStack));
	pCurrentStack = pStack;
	return iCode;
}
bool CVMState::ExchangeVM(int iCode)
{
	miterator mFinder = m_vStackMap.find(iCode);
	if (mFinder != m_vStackMap.end())
	{
		pCurrentStack = mFinder->second;
		return true;
	}
	return false;
}

bool CVMState::IsNull()
{
	assert(pCurrentStack!=NULL && "无效栈");
	return pCurrentStack->Top().iType==8;
}

void CVMState::PushNull()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack  stValue;
	stValue.iType = 8;
	pCurrentStack->Push(stValue);
}
void CVMState::Pop()
{
	assert(pCurrentStack!=NULL && "无效栈");
	pCurrentStack->Pop();
}
bool CVMState::PopBool()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack stValue = pCurrentStack->Pop();
	if (stValue.iType ==0)
	{
		return stValue.uMsg.bValue;
	}else if (stValue.iType ==1)
	{
		return stValue.uMsg.sValue !=0 ? true:false;
	}else if (stValue.iType ==2)
	{
		return stValue.uMsg.iValue !=0 ? true:false;
	}else if (stValue.iType ==3)
	{
		return stValue.uMsg.dValue != 0.0 ? true:false;
	}else if (stValue.iType ==4)
	{
		if (stricmp(stValue.uMsg.dPointer,"true"))
		{
			return true;
		}
		return false;
	}else
		return false;
}
void CVMState::PopNumber(Number& rNum)
{
	stStatueStack stValue = pCurrentStack->Pop();
	if (stValue.iType ==0)
	{
		rNum.iType = 0;
		rNum.uValue.bData = stValue.uMsg.bValue;
	}else if (stValue.iType ==1 )
	{
		rNum.iType = 1;
		rNum.uValue.iData = stValue.uMsg.sValue;
	}else if (stValue.iType==2)
	{
		rNum.iType = 1;
		rNum.uValue.iData = stValue.uMsg.iValue;
	}else if (stValue.iType ==3)
	{
		rNum.iType = 3;
		rNum.uValue.dData = stValue.uMsg.dValue;
	}
}
short CVMState::PopShort()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack stValue = pCurrentStack->Pop();
	if (stValue.iType ==0)
	{
		return stValue.uMsg.bValue ? 1: 0 ;
	}else if (stValue.iType ==1)
	{
		return stValue.uMsg.sValue;
	}else if (stValue.iType ==2)
	{
		return (short)stValue.uMsg.iValue;
	}else if (stValue.iType ==3)
	{
		return (short)stValue.uMsg.dValue;
	}else if (stValue.iType ==4)
	{
		return (short)atoi(stValue.uMsg.dPointer);

	}else
		return 0;
}
int CVMState::PopInt()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack stValue = pCurrentStack->Pop();
	if (stValue.iType ==0)
	{
		return stValue.uMsg.bValue ? 1: 0 ;
	}else if (stValue.iType ==1)
	{
		return stValue.uMsg.sValue;
	}else if (stValue.iType ==2)
	{
		return stValue.uMsg.iValue;
	}else if (stValue.iType ==3)
	{
		return (int)stValue.uMsg.dValue;
	}else if (stValue.iType ==4)
	{
		return atoi(stValue.uMsg.dPointer);

	}else
		return 0;
}
double CVMState::PopDouble()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack stValue = pCurrentStack->Pop();
	if (stValue.iType ==0)
	{
		return stValue.uMsg.bValue ? 1.0: 0.0 ;
	}else if (stValue.iType ==1)
	{
		return (double)stValue.uMsg.sValue;
	}else if (stValue.iType ==2)
	{
		return (double)stValue.uMsg.iValue;
	}else if (stValue.iType ==3)
	{
		return stValue.uMsg.dValue;
	}else if (stValue.iType ==4)
	{
		return (double)atoi(stValue.uMsg.dPointer);

	}else
		return 0;
}
char* CVMState::PopString()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack stValue = pCurrentStack->Pop();
	if (stValue.iType == 4)
	{
		return stValue.uMsg.dPointer;
	}
	return NULL;
}
char* CVMState::PopUserData()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack stValue = pCurrentStack->Pop();
	if (stValue.iType == 5)
	{
		return stValue.uMsg.dPointer;
	}
	return NULL;
}

void CVMState::PopNull()
{
	assert(pCurrentStack!=NULL && "无效栈");
	stStatueStack stValue = pCurrentStack->Pop();
};
