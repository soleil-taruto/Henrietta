int DirRotClock(int dir);
int DirRotHanClock(int dir);
int DirRot(int dir, int rotDir);

extern int DirX;
extern int DirY;
void SetDir(int dir);

extern int PairX[2];
extern int PairY[2];
void SetPairPos(int x, int y, int dir);

extern int Pzr_Dir;
extern int Pzr_RotDir;
extern int Pzr_X;
extern double Pzr_Y;
extern Map_t *Pzr_Map;
void DoRotate(void);
