#include "CStack.h"
#include <assert.h>
#include <string.h>
CStack::CStack(int buffSize): m_iSize(0),m_iFullSize(buffSize),m_pData(NULL),m_iSigal(0)
{
	if (buffSize<0)
	{
		assert( 0 && "初始栈大小不能小于0");
	}
	m_pData = new stStatueStack[buffSize];
}

CStack::~CStack()
{
	if (m_pData != NULL)
	{
		delete [] m_pData;
		m_pData = NULL;
	}
}

bool CStack::Clear()
{
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
		m_iSize = 0;
		m_iFullSize = 10;
		return true;
	}
	return false;
}

bool CStack::IsEmpty()
{
	return 0==m_iSize;
}

void CStack::Push(stStatueStack& elem)
{
	if (m_iSize < m_iFullSize)
	{
		m_pData[m_iSize++] = elem;
	}else
	{
		m_iFullSize *= 2;
		stStatueStack* m_TmpData = new stStatueStack[m_iFullSize];
		assert( m_TmpData != NULL  && "内存分配失败");
		memcpy(m_TmpData, m_pData, m_iSize* sizeof(stStatueStack));
		m_TmpData[m_iSize++] = elem;
		delete [] m_pData;
		m_pData = m_TmpData;
	}
}

stStatueStack CStack::Pop()
{
	assert( m_iSize>0 && "The stack is empty");
	// 每次*2 增长栈， 当栈减为原来的1/3 后主动缩小
	int iNeedResize = m_iFullSize /3;
	if (--m_iSize == iNeedResize)
	{
		m_iFullSize = iNeedResize;
		stStatueStack* m_TmpData = new stStatueStack[m_iFullSize];
		memcpy(m_TmpData, m_pData, m_iFullSize*sizeof(stStatueStack));
		delete [] m_pData;
		m_pData = m_TmpData;
	}else
	{

	}
	return m_pData[m_iSize];
}

int CStack::Size()
{
	return m_iSize;
}

stStatueStack& CStack::Top()const
{
	assert( m_iSize>0 && "The stack is empty");
	return m_pData[m_iSize-1];
}
