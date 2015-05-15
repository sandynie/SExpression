#include "CExpression.h"
#include "CCaller.h"
#include <stack>
#include <assert.h>
using std::stack;
extern Number Zero;
bool  CExpression::ParseExpression(const char* str,bool bType)
{
	if (bType)
	{
		return ParsePrefixExpression(str);
	}else
		return ParsePostExpression(str);
}

int CExpression::readNumber(NumberOperator& value,const char* str, int iStart)
{
	bool bIsDouble = false;
	int iEnd = 0;
	for (int i=iStart;i>=0;i--)
	{
		if (CharacterUtils::IsDot(*(str+i)))
		{
			if (bIsDouble)
			{
				assert(0 && " 小数点只能有一个");
			}
			bIsDouble  = true;
		}else if (!CharacterUtils::IsDigit(*(str+i)))
		{
			iEnd = i+1;
			break;
		}else if (i==0)
		{
			iEnd = 0;
			break;
		}
	}
	string tmp = string(str+iEnd,iStart-iEnd+1);
	if (bIsDouble)
	{
		value.bType  = true;
		value.uValues.uNum.iType = 2;
		value. uValues.uNum.uValue.dData= CharacterUtils::FromStrToDouble(tmp);
	}else
	{
		value.bType =true;
		value.uValues.uNum.iType= 1;
		value. uValues.uNum.uValue.iData = CharacterUtils::FromStrToInt(tmp);
	}
	return iEnd;
}

int CExpression::readMethod(OperatorInfo& oInfo, const char* str,int iStart)
{
	int iEnd = 0;
	for (int i=iStart;i>=0;i--)
	{
		if (!CharacterUtils::IsCharacter(*(str+i)))
		{
			iEnd = i+1;
			break;
		}else if (i==0)
		{
			iEnd = 0;
			break;

		}
	}
	string strMetho(str+iEnd,iStart-iEnd+1);
	if (!CharacterUtils::IsMethod(strMetho,oInfo))
	{
		if (!CharacterUtils::IsHashMethod(strMetho,oInfo))
		{
			assert(0 && "未知方法");
		}
	}
	return iEnd;
}


int CExpression::readOperator(OperatorInfo& oInfo, const char* str, int iStart)
{
	int iEnd = 0;	
	for (int i=iStart;i>=0;i--)
	{
		if (!CharacterUtils::IsOperaterSimple(*(str+i))  && !CharacterUtils::IsSpace(*(str+i)))
		{
			iEnd = i+1;
			break;
		}else if (i==0)
		{
			iEnd = 0;
			break;
		}
	}
	string opstr(str+iEnd,iStart-iEnd+1);
	CharacterUtils::TrimAll(opstr);
	if (!CharacterUtils::IsOperater(opstr,oInfo))
	{
		assert(0 && "无法识别的运算符!");
	}
	return iEnd;
}

void CExpression::judgeOperation(OperatorInfo& oInfo)
{
	if (m_stackOperator.empty())
	{
		m_stackOperator.push(oInfo);
		return;
	}
	OperatorInfo topInfo = m_stackOperator.top();
	if (topInfo.iIndex == 13)
	{
		m_stackOperator.push(oInfo);
		return;
	}
	if (oInfo.iPrority >= topInfo.iPrority)
	{
		m_stackOperator.push(oInfo);
		return ;
	}
	//将  operator栈顶弹出压如 number栈
	NumberOperator no;
	no.bType = false;
	no.uValues.uOI = topInfo;
	m_stackOperator.pop();
	m_dequeNumber.push_back(no);
	judgeOperation(oInfo);
}

void CExpression::doTheLast()
{
	while(m_stackOperator.empty()==false)
	{
		OperatorInfo  topInfo = m_stackOperator.top();
		NumberOperator no;
		no.bType = false;
		no.uValues.uOI = topInfo;
		m_stackOperator.pop();
		m_dequeNumber.push_back(no);
	}
}

void CExpression::adjuestOperation()
{
	OperatorInfo topInfo = m_stackOperator.top();
	while (topInfo.iIndex != 13)
	{
		NumberOperator no;
		no.bType = false;
		no.uValues.uOI = topInfo;
		m_stackOperator.pop();
		m_dequeNumber.push_back(no);
		topInfo = m_stackOperator.top();
	}
	m_stackOperator.pop();
}

Number CExpression::ParseFile(const char* filepath, bool bType)
{
	long lSize;
	char * buffer;  
	size_t result;  
	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */ 
	FILE *pFile = fopen (filepath, "rb" );
	if (pFile==NULL)
	{
		assert( 0 && "文件打开失败");
	}

	/* 获取文件大小 */
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	/* 分配内存存储整个文件 */ 
	buffer = (char*) malloc (sizeof(char)*lSize+1);
	if (buffer == NULL)
	{
		assert(0 && "内存分配失败");
	}

	/* 将文件拷贝到buffer中 */
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize)
	{
		assert(0 && "文件读取失败");
	}
	buffer[lSize]='\0';
	fclose (pFile);

	if (ParseExpression(buffer,bType))
	{
		free (buffer);
		return DoCount();
	}
	free (buffer);
	return Zero;
}

Number CExpression::Parse(const char* str, bool bType )
{
	if (ParseExpression(str,bType))
	{
		return DoCount();
	}
	return Zero;
}
/*
(1) 初始化两个栈：运算符栈S1和储存中间结果的栈S2；
(2) 从右至左扫描中缀表达式；
(3) 遇到操作数时，将其压入S2；
(4) 遇到运算符时，比较其与S1栈顶运算符的优先级：
(4-1) 如果S1为空，或栈顶运算符为右括号“)”，则直接将此运算符入栈；
(4-2) 否则，若优先级比栈顶运算符的较高或相等，也将运算符压入S1；
(4-3) 否则，将S1栈顶的运算符弹出并压入到S2中，再次转到(4-1)与S1中新的栈顶运算符相比较；
(5) 遇到括号时：
(5-1) 如果是右括号“)”，则直接压入S1；
(5-2) 如果是左括号“(”，则依次弹出S1栈顶的运算符，并压入S2，直到遇到右括号为止，此时将这一对括号丢弃；
(6) 重复步骤(2)至(5)，直到表达式的最左边；
(7) 将S1中剩余的运算符依次弹出并压入S2；
(8) 依次弹出S2中的元素并输出，结果即为中缀表达式对应的前缀表达式。
*/
bool  CExpression::ParsePrefixExpression(const char* str)
{
	m_dequeNumber.clear();
	while(m_stackOperator.empty()==false) m_stackOperator.pop();
	assert(str!=NULL);
	int iLen = strlen(str)-1;
	assert(iLen>0);
	bool num_state = true;
	const char * p = str;
	for (int i=iLen;i>=0;i--)
	{
		int iEnd = i;
		if (CharacterUtils::IsDigit(*(p+i))   || CharacterUtils::IsDot(*(p+i)))
		{
			NumberOperator  value;
			i=readNumber(value,p,i);
			m_dequeNumber.push_back(value);
		}else if (CharacterUtils::IsOperaterSimple(*(p+i)))
		{
			OperatorInfo oInfo;
			i = readOperator(oInfo,p,i);
			judgeOperation(oInfo);
		}else if (CharacterUtils::IsCharacter(*(p+i)))
		{
			OperatorInfo oInfo;
			i = readMethod(oInfo,p,i);
			judgeOperation(oInfo);

		}else if (CharacterUtils::IsSpace(*(p+i)))
		{
			continue;
		}else if (CharacterUtils::IsRightQuote(*(p+i)))
		{
			m_stackOperator.push(CharacterUtils::RIGHTQUOTEINOF);
		}else if (CharacterUtils::IsLeftQuote(*(p+i)))
		{
			adjuestOperation();
		}else if (CharacterUtils::IsFunctionQuote(*(p+i)))
		{
			continue;
		}
		else
		{
			printf("未定义 %c",*(p+i));
			assert(0 && "未定义符号！");
		}
	}
	doTheLast();
	return true;
}


/*
(1) 初始化两个栈：运算符栈S1和储存中间结果的栈S2；
(2) 从左至右扫描中缀表达式；
(3) 遇到操作数时，将其压入S2；
(4) 遇到运算符时，比较其与S1栈顶运算符的优先级：
(4-1) 如果S1为空，或栈顶运算符为左括号“(”，则直接将此运算符入栈；
(4-2) 否则，若优先级比栈顶运算符的高，也将运算符压入S1（注意转换为前缀表达式时是优先级较高或相同，而这里则不包括相同的情况）；
(4-3) 否则，将S1栈顶的运算符弹出并压入到S2中，再次转到(4-1)与S1中新的栈顶运算符相比较；
(5) 遇到括号时：
(5-1) 如果是左括号“(”，则直接压入S1；
(5-2) 如果是右括号“)”，则依次弹出S1栈顶的运算符，并压入S2，直到遇到左括号为止，此时将这一对括号丢弃；
(6) 重复步骤(2)至(5)，直到表达式的最右边；
(7) 将S1中剩余的运算符依次弹出并压入S2；
(8) 依次弹出S2中的元素并输出，结果的逆序即为中缀表达式对应的后缀表达式（转换为前缀表达式时不用逆序）。
*/
bool CExpression::ParsePostExpression(const char* str)
{
	return true;
}

/*
例如前缀表达式“- × + 3 4 5 6”：
(1) 从右至左扫描，将6、5、4、3压入堆栈；
(2) 遇到+运算符，因此弹出3和4（3为栈顶元素，4为次顶元素，注意与后缀表达式做比较），计算出3+4的值，得7，再将7入栈；
(3) 接下来是×运算符，因此弹出7和5，计算出7×5=35，将35入栈；
(4) 最后是-运算符，计算出35-6的值，即29，由此得出最终结果。
可以看出，用计算机计算前缀表达式的值是很容易的。
*/
Number CExpression::DoCount()
{
	int iLen = m_dequeNumber.size();
	if (iLen == 0)
	{
		return Zero;
	}
	stack<Number>  nums;
	while (!m_dequeNumber.empty())
	{
		NumberOperator  no = m_dequeNumber.front();
		m_dequeNumber.pop_front();
		if (no.bType)
		{
			nums.push(no.uValues.uNum);
		}else
		{
			if (no.uValues.uOI.iIndex >= CharacterUtils::USERDEFINEFUNCTINBASE)
			{
				//这里对用户自定义函数进行计算
				CSCaller*  caller = CSCaller::getInstance();
				int iCountVmCode = caller->NewStack();
				int iParmLen = no.uValues.uOI.iParams;
				while (iParmLen>0)
				{
					caller->PushToStack(nums.top());
					nums.pop();
					iParmLen --;
				}
				Number  toStack;
				caller->DoCallFromExpressionByHashCode(iCountVmCode,no.uValues.uOI.iHasdCode,toStack);
				nums.push(toStack);
				caller->ClearStack(iCountVmCode);
			}else
			{
				//计算
				if (no.uValues.uOI.iParams == 1)
				{
					OneParamerMethod(nums.top(),no.uValues.uOI.iIndex);
				}else
				{
					Number num1 = nums.top();
					nums.pop();
					TwoParamerMethod(num1,no.uValues.uOI.iIndex,nums.top());
				}
			}

		}
	}
	return nums.top();
}
