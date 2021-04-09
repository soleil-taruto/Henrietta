Puyo_t *TakePuyo(void);
void GivePuyo(Puyo_t *i);

Puyo_t *CreatePuyo(int color, double realX, double realY, double trueX, double trueY);
void ReleasePuyo(Puyo_t *i);

int AddPuyo(Puyo_t *i);
Puyo_t *UnaddPuyo(int index);

int GetNextColor(int mapidx);

int GetPuyoSenyuu(Map_t *map);
double GetPuyoSenyuuRit(Map_t *map);
