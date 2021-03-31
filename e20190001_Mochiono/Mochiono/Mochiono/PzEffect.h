int PzefActEffect(void *vi);
CutIn_t *PzefAddEffectLayer(double x, double y, double xa, double ya, double g, double rSpeed, double z, int lifeFrmCnt, int pic, int layer);
CutIn_t *PzefAddEffect(double x, double y, double xa, double ya, double g, double rSpeed, double z, int lifeFrmCnt, int pic);
void PzefAddStarPon(double x, double y, int num);

extern double PzefX;
extern double PzefY;

void PzefGetCellPos(int mapIndex, int x, int y);
void PzefGetCellCenter(int mapIndex, int x, int y);

extern int PzefJSDest;

void PzefInitJSDest(int mapIndex, int jCnt, int jCntL, int jCntR);
void PzefGetJSDest(void);

void PzefAddHissJamaStar(double startX, double startY, double destX, double destY, int color, int num);

void PzefAddHissatsu(int mapIndex);
void PzefAddHissHide(int mapIndex, int ipic, int kyoutsuuPicMode, int framecnt);
void PzefAddHissKirara_XY(int mapIndex, double x, double y);
void PzefAddHissKirara(int mapIndex);

void PzefAddPuyoWarp(double x, double y, int color);
