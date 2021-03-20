#define IsPlayerWall(cellType) \
	( \
		(cellType) == CT_WALL || \
		(cellType) == CT_WALL_RIGHT_SIGN || \
		(cellType) == CT_WALL_PSEUDO_RIGHT_SIGN || \
		(cellType) == CT_WALL_WRONG_SIGN || m_isRange((cellType), ET_COOKIE_FIRST, ET_COOKIE_LAST))

#define EDIT_WIN_W 240
#define EDIT_WIN_H ((SCREEN_H / 16) * 16)

void TPCN_INIT(void);
void TPCN_FNLZ(void);
void TryPutCellNumber(MCell_t *cell);

void GameMain(void);
