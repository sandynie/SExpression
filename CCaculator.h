#ifndef _CCACULATOR_H
#define _CCACULATOR_H
struct stNumber;
typedef struct stNumber Number;
struct stNumber{
	union{
		bool bData;
		int iData;
		double dData;
	}uValue;
	int iType;  //数据类型  三种 0,1,2   //接下来会逐渐和  stStatueStack 统一
	operator bool()const{
		if (iType==0)
		{
			return uValue.bData;
		}else if (iType == 1)
		{
			return uValue.iData!= 0;
		}else
			return uValue.dData!= 0.0;
	}
	//  iValueType = 0 转成bool  ,  1 转成int  ，2 转成double
	template<class T>
	void GetDataToType( T& toValue,int iValueType)
	{
		if (iValueType == 0)
		{
			if (iType == 0)
			{
				toValue = uValue.bData;
			}else if (iType == 1)
			{
				toValue =(uValue.iData != 0);
			}else
			{
				toValue = (uValue.dData != 0.0);
			}
		}else if (iValueType == 1)
		{
			if (iType == 0)
			{
				toValue = (uValue.bData ? 1: 0 );
			}else if (iType == 1)
			{
				toValue = uValue.iData;
			}else
				toValue = (int)uValue.dData;
		}else
		{
			if (iType == 0)
			{
				toValue = (uValue.bData? 1.0:0.0);
			}else if (iType == 1)
			{
				toValue =(double) uValue.iData;
			}else
				toValue = uValue.dData;
		}
	}
	bool operator >( Number& num2);
	bool operator <( Number& num2);
	bool operator ==( Number& num2);
	bool operator !=( Number& num2);
	bool operator <=( Number& num2);
	bool operator >=( Number& num2);

	Number& operator +=( Number& num2);
	Number& operator -=( Number& num2);
	Number& operator *=( Number& num2);
	Number& operator /=( Number& num2);
};



extern Number Zero;
// 只处理内置运算符和函数
void  TwoParamerMethod(Number&  result, int iType, Number& num2);
void OneParamerMethod(Number& num1,int iType);
#endif
