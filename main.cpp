#include "CExpression.h"
#include "CCaller.h"
#include <iostream>
using std::cout;
using std::endl;

int myFunc(CVMState* pVM)
{
	int ia = pVM->PopInt();
	int ib = pVM->PopInt();
	int iresult = ia+ib;
	pVM->PushInt(iresult);
	return 0;
}

int main()
{
	CharacterUtils::init();
	//char * pExpress = "1+ceil(sqrt((2+3)*4))-5";
	char* pe2 = "1+myxxFunc(5,6)";
	CSCaller  *caller = CSCaller::getInstance();
	caller->RegisterFunction("myxxFunc",myFunc,2);
	CExpression ce;
	//ce.ParseExpression(pe2);
	//Number xx=ce.ParseFile("D:\\vmware.log");

	//Number xx = ce.DoCount();
	Number xx = ce.Parse(pe2);
	if (xx.iType == 1)
	{
		cout<<xx.uValue.iData<<endl;
	}else
		cout<<xx.uValue.dData<<endl;
	system("pause");
	return 0;
}
