#include "StdInc.h"

static void KJam(uchar *list, uint listlen)
{
	for(uint index = 1; index < listlen; index++)
	{
		list[index] = (list[index] + 256 - list[index - 1]) & 0xff;
	}
}
static void CSwap(uchar *list, uint index1, uint index2)
{
	uint swap = list[index1];

	list[index1] = list[index2];
	list[index2] = swap;
}
static void ExtraJamming(uchar counter[4])
{
	KJam(counter, 4);
	CSwap(counter, 0, 3);
	KJam(counter, 4);
	CSwap(counter, 1, 2);
	KJam(counter, 4);
}

#define POLY 0xedb88320

uint *crc32GetSwapTable(void)
{
	static uint *table;

	if(!table)
	{
		uint counter;
		uint i;
		uint j;

		table = (uint *)memAlloc(256 * sizeof(uint));

		for(i = 0; i < 256; i++)
		{
			counter = i;

			for(j = 0; j < 8; j++)
			{
				if(counter & 1)
				{
					counter >>= 1;
					counter ^= POLY;
				}
				else
					counter >>= 1;
			}
			table[i] = counter;
		}
	}
	return table;
}

uint crc32NewCounter(void)
{
	return 0xffffffff;
}
void crc32UpdateChar(uint *pCounter, int chr)
{
	*pCounter = (*pCounter >> 8) ^ crc32GetSwapTable()[(*pCounter ^ chr) & 0xff];
}
void crc32UpdateBlock(uint *pCounter, void *block, uint blockSize)
{
	uint *table = crc32GetSwapTable();
	uchar *p;
	uchar *pEnd;

	pEnd = (uchar *)block + blockSize;

	for(p = (uchar *)block; p < pEnd; p++)
	{
		*pCounter = (*pCounter >> 8) ^ table[(*pCounter ^ *p) & 0xff];
	}
}
void crc32UpdateLine(uint *pCounter, char *line)
{
	crc32UpdateBlock(pCounter, line, strlen(line));
}
uint crc32Finisher(uint counter)
{
	ExtraJamming((uchar *)&counter);
	return counter ^ 0xffffffff;
}

uint crc32CheckBlock(void *block, uint blockSize)
{
	static uint counter;
	counter = crc32NewCounter();
	//uint counter = crc32NewCounter(); // >LINK : fatal error LNK1000: Internal error during IMAGE::BuildImage

	crc32UpdateBlock(&counter, block, blockSize);
	return crc32Finisher(counter);
}
uint crc32CheckLine(char *line)
{
	return crc32CheckBlock(line, strlen(line));
}

void PutCRC(uchar *block, uint blockSize)
{
	uint crc = crc32CheckBlock(block, blockSize);

	block[blockSize + 0] = crc >> 0x00 & 0xff;
	block[blockSize + 1] = crc >> 0x08 & 0xff;
	block[blockSize + 2] = crc >> 0x10 & 0xff;
	block[blockSize + 3] = crc >> 0x18 & 0xff;
}
uint CheckCRC(uchar *block, uint blockSize)
{
	uint crc = crc32CheckBlock(block, blockSize);

	return
		block[blockSize + 0] == (crc >> 0x00 & 0xff) &&
		block[blockSize + 1] == (crc >> 0x08 & 0xff) &&
		block[blockSize + 2] == (crc >> 0x10 & 0xff) &&
		block[blockSize + 3] == (crc >> 0x18 & 0xff);
}
