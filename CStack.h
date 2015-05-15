#ifndef _CSTACK_H
#define _CSTACK_H
#include "CMacro.h"
typedef struct {
	int iType;
	union{
		bool bValue;
		short sValue;
		int iValue;
		char* dPointer;  //鎸囬拡鎴栨暟缁?
		double dValue;
	}uMsg;
}stStatueStack;
class CStack
{
public:
	bool IsEmpty();
	bool Clear();
	int  Size();
	stStatueStack Pop();
	void Push(stStatueStack& elem);
	stStatueStack& Top()const;
	CStack(int buffSize = 10);
	~CStack();
	int getSignal()const{return m_iSigal;}
	void SetSignal(int sigal){m_iSigal= sigal;}
protected:
private:
	stStatueStack*  m_pData;
	int m_iFullSize;
	int m_iSize;
	int m_iSigal;
};
#endif
