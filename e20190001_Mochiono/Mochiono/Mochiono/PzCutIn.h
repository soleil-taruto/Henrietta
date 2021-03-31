CutIn_t *CIEx_TakeCutIn(int mapIndex, int side);
void CIEx_ActCutIn(CutIn_t *i);
void CIEx_AddStar(int num);

void CI_AddCutIn(int rensanum, int mapIndex, int side);
void CI_AddDamage(int mapIndex, int side);
void CI_AddJamaStar(int destJamaCount, int destMapIndex, double startX, double startY, double endX, double endY);
void CI_AddHiss(int mapIndex, PuzzPair_t pairSide);
