int LoadSound(char *file);
void UnloadSound(int snd);
void SetSeVolume(double volume);
void Direct_PlaySe(int snd);

int LoadBGM(char *file);
void UnloadBGM(int bgm);
void Direct_PlayBGM(int bgm, int playOnce = 0);
void Direct_StopBGM(void);
void Direct_SetBGMVolume(double volume);
void Direct_FadeOut(double startvol, double subvol);

void SoundFrame(void);

void PlaySe(int snd);
void PlayBGM(int bgm, int playOnce = 0);
void StopBGM(void);
void SetBGMVolume(double volume);
void FadeOut(double startvol, double subvol);
