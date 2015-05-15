#ifndef _CEXPRESSION_H
#define _CEXPRESSION_H
#include "CUtils.h"
#include <deque>
#include <stack>
using std::deque;
using std::stack;
class CExpression
{
public:
	typedef struct{
		union{
			OperatorInfo  uOI;
			Number uNum;
		}uValues;
		bool bType;  //  true -- number
	}NumberOperator;
	Number  DoCount();
	bool  ParseExpression(const char* str,bool bType = true);
	bool   ParsePrefixExpression(const char* str);//前置
	bool ParsePostExpression(const char* str);  //后置
	Number Parse(const char* str, bool bType = true);
	Number ParseFile(const char* filepath, bool bType = true);
protected:
private:
	int readNumber(NumberOperator& value,const char* str,int iStart);
	int readOperator(OperatorInfo& oInfo, const char* str, int iStart);
	int readMethod(OperatorInfo& oInfo, const char* str,int iStart);
	void judgeOperation(OperatorInfo& oInfo);
	void adjuestOperation();// 遇到左括号
	void doTheLast();
	stack<OperatorInfo> m_stackOperator;
	deque<NumberOperator>  m_dequeNumber;
};
#endif
