#ifndef _CUTILS_H
#define _CUTILS_H
#include <map>
#include <string>
#include "CCaculator.h"
using std::map;
using std::string;
struct stOperatorInfo{
	int iPrority;  //优先级
	short iIndex;   
	short iParams; //标识一元(1)运算还是二元(0)
	int iHasdCode;
};
typedef struct stOperatorInfo OperatorInfo, *LPOperatorInfo;
class CharacterUtils
{
public:
	static bool IsDigit(char c);
	static bool IsCharacter(char c);
	static bool IsDot(char c);
	static bool IsSpace(char c);
	static int FromStrToInt(string& str);
	static bool IsOperaterSimple(char c);
	static double FromStrToDouble(string& str);
	static bool IsOperater(string  str,OperatorInfo& info);
	static bool IsLeftQuote(char c){return c==LEFTQUOTE;};
	static bool IsRightQuote(char c){return c==RIGHTQUOTE;}
	static bool IsMethod(string str,OperatorInfo& inInfo);
	static bool IsHashMethod(string str,OperatorInfo&inInfo);
	static void TrimAll(string& str);
	static bool IsFunctionQuote(char c){return c==FUNCTIONQUOTE;};
	static int CompareOperator(string str1,string str2);
	static unsigned int murMurHash(const void *key, int len);
	static void init();
	static bool RegisterUserDefineFunction(string str,int iParamLen);
	static stOperatorInfo  GetOperatorInfo(int ip,int iIndex, int iiParam);
	typedef map<string,OperatorInfo>::iterator operatorIter;
	typedef map<string,OperatorInfo>::value_type HashValueType;
protected:
public:
	static map<string,OperatorInfo> m_mapOperator;
	static map<string, OperatorInfo> m_mapOpMethods;
	static map<string ,OperatorInfo> m_mapHashMethods;
	static char LEFTQUOTE;
	static char RIGHTQUOTE;
	static char FUNCTIONQUOTE;
	static int iIdIndex;
	static int IFUNCTIONPRIORITY;  //函数优先级
	static int USERDEFINEFUNCTION;
	static int USERDEFINEFUNCTINBASE;
public:
	static OperatorInfo LEFTQUOTEINFO;
	static OperatorInfo RIGHTQUOTEINOF;
};
/*
opLvl = new HashMap<String, Integer>();
opLvl.put("||", 0);
opLvl.put("&&", 1);
opLvl.put(">", 2);
opLvl.put("<", 2);
opLvl.put(">=", 2);
opLvl.put("<=", 2);
opLvl.put("==", 2);
opLvl.put("!=", 2);
opLvl.put("+", 3);
opLvl.put("-", 3);
opLvl.put("*", 4);
opLvl.put("/", 4);
opLvl.put("sqr", 5);
opLvl.put("(", -1);
opLvl.put(")", -2);
*/
#endif
