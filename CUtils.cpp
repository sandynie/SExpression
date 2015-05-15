#include "CUtils.h"
#include <stdlib.h>
#include <algorithm>
#include <assert.h>
using std::remove_if;
map<string,OperatorInfo> CharacterUtils::m_mapOperator;
map<string,OperatorInfo> CharacterUtils::m_mapOpMethods;
map<string, OperatorInfo> CharacterUtils::m_mapHashMethods;
char CharacterUtils::LEFTQUOTE='(';
char CharacterUtils::RIGHTQUOTE=')';
char CharacterUtils::FUNCTIONQUOTE=',';
OperatorInfo CharacterUtils::LEFTQUOTEINFO={-1,12,0};
OperatorInfo CharacterUtils::RIGHTQUOTEINOF={0,13,0};
int CharacterUtils::iIdIndex = 0;
int CharacterUtils::IFUNCTIONPRIORITY = 5;
int CharacterUtils::USERDEFINEFUNCTION=100;  //所有自定义的函数都从这里开始
int CharacterUtils::USERDEFINEFUNCTINBASE=100;
stOperatorInfo CharacterUtils::GetOperatorInfo(int ip,int iiIndex, int iiParam)
{
	stOperatorInfo  soi;
	soi.iIndex = iiIndex;
	soi.iPrority = ip;
	soi.iParams = iiParam;
	return soi;
}
//how 允许函数多个参数，快速查找.......
/*
定义如下规则：  1.运算符的id 在一个字节范围内。
2.函数优先级通一定义为5
3.函数可以带多个参数，通过计算函数名的hash码，放在map中
void (*process)(vector<Number>& xx)
*/
void CharacterUtils::init()
{
	m_mapOperator.clear();
	m_mapOperator["||"]=CharacterUtils::GetOperatorInfo(0,0,2);
	m_mapOperator["&&"]=CharacterUtils::GetOperatorInfo(1,1,2);
	m_mapOperator[">"]=CharacterUtils::GetOperatorInfo(2,2,2);
	m_mapOperator["<"]=CharacterUtils::GetOperatorInfo(2,3,2);
	m_mapOperator[">="]=CharacterUtils::GetOperatorInfo(2,4,2);
	m_mapOperator["<="]=CharacterUtils::GetOperatorInfo(2,5,2);
	m_mapOperator["=="]=CharacterUtils::GetOperatorInfo(2,6,2);
	m_mapOperator["!="]=CharacterUtils::GetOperatorInfo(2,7,2);
	m_mapOperator["+"]=CharacterUtils::GetOperatorInfo(3,8,2);
	m_mapOperator["-"]=CharacterUtils::GetOperatorInfo(3,9,2);
	m_mapOperator["*"]=CharacterUtils::GetOperatorInfo(4,10,2);
	m_mapOperator["/"]=CharacterUtils::GetOperatorInfo(4,11,2);
	m_mapOperator["("]=CharacterUtils::GetOperatorInfo(-1,12,0);
	m_mapOperator[")"]=CharacterUtils::GetOperatorInfo(0,13,0);

	m_mapOpMethods["sqrt"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,30,1);
	m_mapOpMethods["sin"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,31,1);
	m_mapOpMethods["cos"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,32,1);
	m_mapOpMethods["tan"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,33,1);
	m_mapOpMethods["cotan"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,34,1);
	m_mapOpMethods["pow"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,35,2);
	m_mapOpMethods["log"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,36,1);
	m_mapOpMethods["abs"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,37,1);
	m_mapOpMethods["floor"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,38,1);
	m_mapOpMethods["ceil"]=CharacterUtils::GetOperatorInfo(IFUNCTIONPRIORITY,39,1);
}


/**
* 比较好的hash算法
* http://murmurhash.googlepages.com/
*/
unsigned int CharacterUtils::murMurHash(const void *key, int len)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	const int seed = 97;
	unsigned int h = seed ^ len;
	// Mix 4 bytes at a time into the hash
	const unsigned char *data = (const unsigned char *)key;
	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;
		k *= m; 
		k ^= k >> r; 
		k *= m; 
		h *= m; 
		h ^= k;
		data += 4;
		len -= 4;
	}
	// Handle the last few bytes of the input array
	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};
	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h;
}

bool CharacterUtils::IsCharacter(char c)
{
	return (c>='A' && c<='Z') || (c>='a' && c<='z');
}
bool CharacterUtils::IsDigit(char c)
{
	return (c>='0' && c<='9'   || c=='-');
}
void CharacterUtils::TrimAll(string& str)
{
	int iSize = str.size();
	assert(iSize!=0 && "无法trim 空 string");
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

bool CharacterUtils::IsOperaterSimple(char c)
{
	return (c=='+' || c=='-' || c=='*' || c=='/' || c=='&' || c=='=' || c=='!' || c=='>' || c=='<' || c=='|' || c=='*');  
}
bool CharacterUtils::IsDot(char c)
{
	return (c=='.');
}	

bool CharacterUtils::IsSpace(char c)
{
	return (c==' ');
}

int CharacterUtils::FromStrToInt(string& str)
{
	return atoi(str.c_str());
}

double CharacterUtils::FromStrToDouble(string& str)
{
	return atof(str.c_str());
}

bool CharacterUtils::RegisterUserDefineFunction(string str,int iParamLen)
{
	operatorIter hIter = m_mapHashMethods.find(str);
	if (hIter == m_mapHashMethods.end())
	{
		unsigned iHashCode = CharacterUtils::murMurHash(str.c_str(),str.size());
		OperatorInfo oinfo;
		oinfo.iHasdCode = iHashCode;
		oinfo.iParams =  iParamLen;
		oinfo.iIndex = USERDEFINEFUNCTION;
		USERDEFINEFUNCTION ++;
		oinfo.iPrority = IFUNCTIONPRIORITY;
		m_mapHashMethods.insert(HashValueType(str,oinfo));
		return true;
	}
	return false;
}

bool CharacterUtils::IsHashMethod(string str,OperatorInfo&inInfo)
{
	operatorIter  hIter = m_mapHashMethods.find(str);
	if (hIter != m_mapHashMethods.end())
	{
		inInfo  =hIter->second;
		return true;
	}
	return false;
}

bool CharacterUtils::IsMethod(string str,OperatorInfo& inInfo)
{
	operatorIter fIter = m_mapOpMethods.find(str);
	if(fIter != m_mapOpMethods.end())
	{
		inInfo = fIter->second;
		return true;
	}
	return false;
}

bool CharacterUtils::IsOperater(string str,OperatorInfo & inInfo)
{
	operatorIter fIter = m_mapOperator.find(str);
	if(fIter != m_mapOperator.end())
	{
		inInfo = fIter->second;
		return true;
	}
	return false;
}

int CharacterUtils::CompareOperator(string str1,string str2)
{
	operatorIter fiter1 = m_mapOperator.find(str1);
	if (fiter1 != m_mapOperator.end())
	{
		operatorIter fiter2 = m_mapOperator.find(str2);
		if (fiter2 != m_mapOperator.end())
		{
			return ((fiter1->second.iPrority - fiter2->second.iPrority)>=0);
		}
		return -1;
	}
	return -2;

}
