#include "CCaculator.h"
#include <assert.h>
#include <math.h>
Number Zero;
const float LN_2 = 0.693147180559945f;

bool Number::operator >( Number& num2)
{
	if (iType== 2  || num2.iType==2)
	{
		double dme;
		double dother;
		GetDataToType(dme,2);
		num2.GetDataToType(dother,2);
		return dme>dother;
	}else
	{
		int ime;
		int iother;
		GetDataToType(ime,1);
		num2.GetDataToType(iother,1);
		return ime>iother;
	}	
}

bool Number::operator <( Number& num2)
{
	if (iType== 2  || num2.iType==2)
	{
		double dme;
		double dother;
		GetDataToType(dme,2);
		num2.GetDataToType(dother,2);
		return dme<dother;
	}else
	{
		int ime;
		int iother;
		GetDataToType(ime,1);
		num2.GetDataToType(iother,1);
		return ime<iother;
	}	
}

bool Number::operator ==( Number& num2)
{
	if (iType== 2  || num2.iType==2)
	{
		double dme;
		double dother;
		GetDataToType(dme,2);
		num2.GetDataToType(dother,2);
		return dme==dother;
	}else if(iType == 1 || num2.iType==1)
	{
		int ime;
		int iother;
		GetDataToType(ime,1);
		num2.GetDataToType(iother,1);
		return ime<iother;
	}else
	{
		return (uValue.bData == num2.uValue.bData);
	}
}

bool Number::operator <=( Number& num2)
{
	return !((*this)>num2);
}

bool Number::operator >=( Number& num2)
{
	return !((*this)<num2);
}

bool Number::operator !=( Number& num2)
{
	return !((*this)==num2);
}

Number& Number::operator +=( Number& num2)
{
	if (iType == 2  || num2.iType==2)
	{
		double dme;
		double dother;
		GetDataToType(dme,2);
		num2.GetDataToType(dother,2);
		iType = 2;
		uValue.dData = dme+dother;

	}else if (iType==1 || num2.iType==2)
	{
		int ime;
		int iother;
		GetDataToType(ime,1);
		num2.GetDataToType(iother,1);
		iType = 1;
		uValue.iData = ime+iother;
	}else
	{
		iType = 0;
		uValue.bData = true;

	}
	return *this;
}

Number& Number::operator -=( Number& num2)
{
	if (iType == 2  || num2.iType==2)
	{
		double dme;
		double dother;
		GetDataToType(dme,2);
		num2.GetDataToType(dother,2);
		iType = 2;
		uValue.dData = dme-dother;
	}else if (iType==1 || num2.iType==2)
	{
		int ime;
		int iother;
		GetDataToType(ime,1);
		num2.GetDataToType(iother,1);
		iType = 1;
		uValue.iData = ime-iother;
	}else
	{
		iType = 0;
		if (uValue.bData == num2.uValue.bData)
		{
			uValue.bData = false;
		}
	}
	return *this;
}

Number& Number::operator *=( Number& num2)
{
	if (iType == 2  || num2.iType==2)
	{
		double dme;
		double dother;
		GetDataToType(dme,2);
		num2.GetDataToType(dother,2);
		iType = 2;
		uValue.dData = dme*dother;
	}else if (iType==1 || num2.iType==1)
	{
		int ime;
		int iother;
		GetDataToType(ime,1);
		num2.GetDataToType(iother,1);
		iType = 1;
		uValue.iData = ime * iother;
	}else
	{
		iType = 0;
		if (!uValue.bData && !num2.uValue.bData)
		{
			uValue.bData = false;
		}
	}
	return *this;
}

Number& Number::operator /=( Number& num2)
{
	if (iType == 2  || num2.iType==2)
	{
		double dme;
		double dother;
		GetDataToType(dme,2);
		num2.GetDataToType(dother,2);
		iType = 2;
		uValue.dData = dme/dother;
	}else if (iType==1 || num2.iType==1)
	{
		int ime;
		int iother;
		GetDataToType(ime,1);
		num2.GetDataToType(iother,1);
		iType = 1;
		uValue.iData = ime / iother;
	}else
	{
		iType = 0;
		if (!uValue.bData && !num2.uValue.bData)
		{
			uValue.bData = false;
		}
	}
	return *this;
}


//这里true按照1 算 false按0算
void  TwoParamerMethod(Number&  num2,int iType,Number& result)
{
	bool bRet;
	switch (iType)
	{
	case 0:
		//"||"
		bRet = (num2) || (result);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;
	case 1:
		//"&&"
		bRet = (num2) || (result);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;
	case 2:
		//>
		bRet = (num2 > result);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;
	case 3:
		// <
		bRet = (num2< result);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;
	case 4:
		//>=
		bRet =( num2>= result);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;
	case 5:
		//<=
		bRet = (num2<= result);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;
	case 6:
		//==
		bRet = (result ==  num2);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;
	case 7:
		//!=
		bRet = (result !=  num2);
		result.iType = 0;
		result.uValue.bData = bRet;
		break;

	case 8:
		//+
		result += num2;
		break;

	case 9:
		//-
		num2-=result;
		result = num2;
		break;

	case 10:
		//*
		result *=num2;
		break;

	case 11:
		// /
		num2 /= result;
		result = num2;
		break;
	case 35: //pow
		if (num2.iType==1)
		{
			if (result.iType ==1)
			{
				num2.uValue.dData = pow((double)num2.uValue.iData,(double)result.uValue.iData);
			}else
				num2.uValue.dData = pow((double)num2.uValue.iData,(double)result.uValue.dData);

			num2.iType = 2;
			result = num2;
		}else if (num2.iType ==2)
		{
			if (result.iType ==1)
			{
				num2.uValue.dData = pow((double)num2.uValue.dData,(double)result.uValue.iData);
			}else
				num2.uValue.dData = pow((double)num2.uValue.dData,(double)result.uValue.dData);

			result = num2;
		}
		break;
	default:
		assert(0 && "未知二元操作符!!");
		break;
	}
}
void OneParamerMethod(Number& result,int iType)
{
	switch (iType)
	{
	case 30:  // sqrt
		if (result.iType==1)
		{
			result.uValue.dData = sqrt((double)result.uValue.iData);
			result.iType = 2;
		}else if (result.iType ==2)
		{
			result.uValue.dData = sqrt((double)result.uValue.dData);
		}
		break;
	case 31:  //sin
		if (result.iType==1)
		{
			result.uValue.dData = sin((double)result.uValue.iData);
			result.iType = 2;
		}else if (result.iType ==2)
		{
			result.uValue.dData = sin((double)result.uValue.dData);
		}
		break;
	case 32: //cos
		if (result.iType==1)
		{
			result.uValue.dData = cos((double)result.uValue.iData);
			result.iType = 2;
		}else if (result.iType ==2)
		{
			result.uValue.dData = cos((double)result.uValue.dData);
		}
		break;
	case 33:  //tan
		if (result.iType==1)
		{
			result.uValue.dData = tan((double)result.uValue.iData);
			result.iType = 2;
		}else if (result.iType ==2)
		{
			result.uValue.dData = tan((double)result.uValue.dData);
		}
		break;
	case 34:  //cotan
		if (result.iType==1)
		{
			result.uValue.dData = 1.0f / tan((double)result.uValue.iData);
			result.iType = 2;
		}else if (result.iType ==2)
		{
			result.uValue.dData = 1.0f / tan((double)result.uValue.dData);
		}
		break;

	case 36: // log2
		if (result.iType==1)
		{
			result.uValue.dData = logf((double)result.uValue.iData)/LN_2;
			result.iType = 2;
		}else if (result.iType ==2)
		{
			result.uValue.dData = logf((double)result.uValue.dData)/ LN_2;
		}
		break;
	case 37: //abs
		if (result.iType == 2)
		{
			result.uValue.dData =((result.uValue.dData <0.0f )?(-result.uValue.dData):result.uValue.dData) ;
		}else if (result.iType ==1)
		{
			result.uValue.iData =((result.uValue.iData <0 )?(-result.uValue.iData):result.uValue.iData) ;
		}
		break;
	case 38: // floor
		if (result.iType == 2)
		{
			result.iType = 1;
			result.uValue.iData = (int)(result.uValue.dData);
		}
		break;
	case 39: //ceil
		if (result.iType ==2)
		{
			result.iType = 1;
			result.uValue.iData = ceilf(result.uValue.dData);
		}
		break;
	default:
		assert(0 && "未知一元操作符!!");
		break;
	}
}
