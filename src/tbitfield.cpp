// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
	if (len < 0)
		throw "negative length";
	BitLen = len;
	MemLen = ((len - 1) /BITS_IN_ONE_MEM + 1);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0)
		return n / BITS_IN_ONE_MEM;
	else
		throw "negative index";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n - GetMemIndex(n) *BITS_IN_ONE_MEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int an = GetMemIndex(n);
	if ((n >= 0) && (n < BitLen))
	{
		pMem[an] |= GetMemMask(n);
	}
	/*{
		int k = n / (sizeof(TELEM) * 8);
		int offset = n % (sizeof(TELEM) * 8);
		TELEM mask = 1 << (sizeof(TELEM) * 8 - offset - 1);
		pMem[k] = pMem[k] |= mask;
	}*/
	else
		throw "wrong index";

}


void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= 0) && (n < BitLen))
	{
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	}
	else
		throw "wrong index";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n >= 0) && (n < BitLen))
	{
		return pMem[GetMemIndex(n)] & GetMemMask(n);
	}
	else
		throw "wrong index";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (MemLen!=bf.MemLen)
	{ 
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen)
	{
		int a = 1;
		for (int i = 0; i<MemLen - 1; i++)		
			if (pMem[i] != bf.pMem[i])
			{
				a = 0;
				break;
			}
		
		for (int i = (MemLen - 1) * BITS_IN_ONE_MEM; i<BitLen; i++)
		
			if (GetBit(i) != bf.GetBit(i))
			{
				a = 0;
				break;
			}	
		return a;
	}
	else
	{
		return 0;
	}
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen)
	{
		int a = 0;
		for (int i = 0; i < MemLen; i++)
		
			if (pMem[i] != bf.pMem[i])
			
				a = 1;
				
		return a;
	}
	else
	{
		return 1;
	}
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i;
	int	len = BitLen;
	if (bf.BitLen>len)
	{
		len = bf.BitLen;
	}
	TBitField a(len);
	for (i = 0; i < MemLen; i++)
	{
		a.pMem[i] = pMem[i];
	}

	for (i = 0; i < bf.MemLen; i++)
	{
		a.pMem[i] |= bf.pMem[i];
	}
	return a;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	
		if (BitLen > bf.BitLen)
		{
			TBitField temp(BitLen);
			for (int i = 0; i < bf.MemLen; i++)
				temp.pMem[i] = pMem[i] & bf.pMem[i];
			for (int i = bf.MemLen; i < temp.MemLen; i++)
				temp.pMem[i] = 0;
			return temp;
		}
		else
		{
			TBitField temp(bf.BitLen);
			for (int i = 0; i < MemLen; i++)
				temp.pMem[i] = pMem[i] & bf.pMem[i];
			for (int i = MemLen; i < temp.MemLen; i++)
				temp.pMem[i] = 0;
			return temp;
		
	}
	/*int i;
	int	len = BitLen;
	if (bf.BitLen > len)
	{
		len = bf.BitLen;
	}
	TBitField a(len);
	for (i = 0; i < MemLen; i++)
	{
		a.pMem[i] = pMem[i];
	}
	for (i = 0; i < bf.MemLen; i++)
	{
		a.pMem[i] &= bf.pMem[i];
	}
	return a;*/
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField a(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i))
		{
			a.ClrBit(i);
		}
		else
			a.SetBit(i);
	}
	return a;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	int i = 0;
	char ch = ' ';
	ostr << "{ ";
	for (; i < len; ++i)
	{
		if (bf.GetBit(i))
		{
			ostr << ch << i << ' ';
		}
	}
	ostr << "} ";
	return ostr;
}
