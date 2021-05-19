uint *crc32GetSwapTable(void);

uint crc32NewCounter(void);
void crc32UpdateChar(uint *pCounter, int chr);
void crc32UpdateBlock(uint *pCounter, void *block, uint blockSize);
void crc32UpdateLine(uint *pCounter, char *line);
uint crc32Finisher(uint counter);

uint crc32CheckBlock(void *block, uint blockSize);
uint crc32CheckLine(char *line);

void PutCRC(uchar *block, uint blockSize);
uint CheckCRC(uchar *block, uint blockSize);
