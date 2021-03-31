// -- デバッグ用 --

// [LOG_ENABLED_SWITCH]
/*
#define LOG_ENABLED 0
/*/
#define LOG_ENABLED 1
//*/

// ----

typedef unsigned char uchar;

#define PI 3.141592653589793

#define lengthof(list) \
	(sizeof(list) / sizeof(*list))

#define m_min(v1, v2) ((v1) < (v2) ? (v1) : (v2))
#define m_max(v1, v2) ((v1) < (v2) ? (v2) : (v1))

#define m_range(value, minval, maxval) \
	do { \
	value = m_min(value, maxval); \
	value = m_max(value, minval); \
	} while(0)

/*
	fvar : float, double
	0.0 < scale < 1.0
*/
#define nearize(fvar, target, scale) \
	do { \
	fvar -= target; \
	fvar *= scale; \
	fvar += target; \
	} while(0)

/*
	fvar : float, double
	0.0 < margin
*/
#define adjustize(fvar, target, margin) \
	do { \
	fvar -= target; \
	if(abs(fvar) < margin) fvar = 0.0; \
	fvar += target; \
	} while(0)

#define nearize_adj(fvar, target, scale, margin) \
	do { \
	nearize(fvar, target, scale); \
	adjustize(fvar, target, margin); \
	} while(0)

/*
	int frmidx;
	double frmpct;
*/
#define frameloop(frmidx, frmpct, count) \
	for(frmidx = 0; frmidx <= count && (frmpct = (double)frmidx / count, 1); frmidx++)

/*
	int frmidx;
	double frmpct;
*/
#define frameif(frmidx, frmpct, count) \
	if(frmidx < count ? (frmpct = (double)frmidx / count, 1) : (frmidx -= count, 0))

#define m_pow(value) \
	( (value) * (value) )

#define nearthan(x1, y1, x2, y2, kyori) \
	( m_pow(x1 - x2) + m_pow(y1 - y2) < m_pow(kyori) )

#define isinside(x, y, x1, y1, x2, y2) \
	( x1 <= x && x <= x2 && y1 <= y && y <= y2 )

#define m_tonot(var) \
	( (var) = (var) ? 0 : 1 )

#define m_countDown(var) \
	do { \
	     if((var) < 0) (var)++; \
	else if((var) > 0) (var)--; \
	} while(0)

/*
	denom:
		 a as (+= 1 / a)
		-a as (-= 1 / a)
*/
#define m_incDenom(var, denom) \
	((var) += 1.0 / (denom))
//	((var) *= (denom), (var) = (double)((int)(var) + 1), (var) /= (denom))

// ----

#define BORDER_OF_NUMERIC 1000000000
#define JAMA_OOSUGI_NUM 15
#define JAMA_MAX_ONCE 20
#define HISS_SCORE 5000 // 必殺技発動に必要なスコア

// ----
// 追加

#define IMAX BORDER_OF_NUMERIC
#define BLOCK_COLOR_NUM 6

// ----

typedef enum PuzzPlayer_et
{
	PZPLYR_1P1,
	PZPLYR_1P2,
	PZPLYR_2P1,
	PZPLYR_2P2,

	PZPLYR_MAX, // == len
}
PuzzPlayer_t;

typedef enum PuzzPair_et
{
	PZPAIR_1P,
	PZPAIR_2P,

	PZPAIR_MAX, // == len
}
PuzzPair_t;

typedef enum HetaChara_et
{
	CHARA_NONE, // 居ない場合

	CHARA_I, // 伊
	CHARA_E, // 英
	CHARA_S, // 西
	CHARA_C, // 中
	CHARA_G, // 独
	CHARA_R, // 南
	CHARA_J, // 日
	CHARA_P, // 普
	CHARA_F, // 仏
	CHARA_A, // 米
	CHARA_U, // 露

	CHARA_MAX, // == len
}
HetaChara_t;

// 使わない。
/*
typedef enum HetaPair_et
{
	PAIR_NONE, // ソロの場合

	PAIR_IR,
	PAIR_IJ,
	PAIR_EC,
	PAIR_EJ,
	PAIR_SR,
	PAIR_GI,
	PAIR_GJ,
	PAIR_GP,
	PAIR_PI,
	PAIR_PJ,
	PAIR_FE,
	PAIR_FS,
	PAIR_AE,
	PAIR_UP,
	PAIR_UA,

	PAIR_MAX, // == len
}
HetaPair_t;
*/

typedef enum HetaBasho_et
{
	BASHO_SUNFLOWER,
	BASHO_FLOWER,
	BASHO_SEA,
	BASHO_LAKE,
	BASHO_DESERT,
	BASHO_FOREST,
	BASHO_JINJA,
	BASHO_NIGHT,

	BASHO_MAX, // == len
}
HetaBasho_t;

extern char *LCharaList[CHARA_MAX];
extern char *LBashoList[BASHO_MAX];

#define CORRECT_PAIR_MAX 26
extern int CorrectPairs[CORRECT_PAIR_MAX][2];

// エンディング
enum
{
	ED_GI,
	ED_GJ,
	ED_GP,
	ED_IJ,
	ED_IP,
	ED_PJ,

	ED_MAX,
};
