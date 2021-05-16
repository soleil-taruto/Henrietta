#include "StdInc.h"

Dc_t *Dc;

void DcInit_LoadAllPicture(void)
{
	Dc->PicDummy = LoadPicture(ResourcePath("補助\\Dummy.png"));
	Dc->PicBlackWall = LoadPicture(ResourcePath("補助\\黒壁.png"));
	Dc->PicWhiteWall = LoadPicture(ResourcePath("補助\\白壁.png"));

	Dc->PicLogo   = LoadPicture(ResourcePath("画像\\ロゴ画面.png"));
	Dc->PicTitle  = LoadPicture(ResourcePath("画像\\スタート画面.png"));
	Dc->PicSelect = LoadPicture(ResourcePath("画像\\選択肢.png"));
	Dc->PicExit   = Dc->PicDummy; // LoadPicture(ResourcePath("画像\\体験版終了.png"));

	Dc->PicTitleCursor = LoadPicture(ResourcePath("画像\\メニュー画面選択ボタン.png"));

	Dc->PicTitleStartOn      = LoadPicture(ResourcePath("画像\\体験版スタートボタンマウスオン.png"));
	Dc->PicTitleStartPress   = LoadPicture(ResourcePath("画像\\体験版スタートボタンマウスクリック.png"));
	Dc->PicTitleSettingOn    = LoadPicture(ResourcePath("画像\\設定をいじるボタンマウスオン.png"));
	Dc->PicTitleSettingPress = LoadPicture(ResourcePath("画像\\設定をいじるボタンマウスクリック.png"));

	Dc->PicCfgWall       = LoadPicture(ResourcePath("System\\Config\\Wall.png"));
	Dc->PicCfgWindow     = LoadPicture(ResourcePath("System\\Config\\ウィンドウ.png"));
	Dc->PicCfgFullScreen = LoadPicture(ResourcePath("System\\Config\\フルスクリーン.png"));
	Dc->PicCfgKidoku     = LoadPicture(ResourcePath("System\\Config\\既読のみ.png"));
	Dc->PicCfgMidoku     = LoadPicture(ResourcePath("System\\Config\\未読を含む.png"));
	Dc->PicCfgRotL       = LoadPicture(ResourcePath("System\\Config\\左回転.png"));
	Dc->PicCfgRotR       = LoadPicture(ResourcePath("System\\Config\\右回転.png"));
	Dc->PicCfgDir2       = LoadPicture(ResourcePath("System\\Config\\下.png"));
	Dc->PicCfgDir4       = LoadPicture(ResourcePath("System\\Config\\左.png"));
	Dc->PicCfgDir6       = LoadPicture(ResourcePath("System\\Config\\右.png"));
	Dc->PicCfgDir8       = LoadPicture(ResourcePath("System\\Config\\上.png"));
	Dc->PicCfgBomb       = LoadPicture(ResourcePath("System\\Config\\必殺技.png"));
	Dc->PicCfgModoru     = LoadPicture(ResourcePath("System\\Config\\戻る.png"));
	Dc->PicCfgTsumami    = LoadPicture(ResourcePath("System\\Config\\つまみ.png"));
	Dc->PicCfgPressKey   = LoadPicture(ResourcePath("System\\Config\\キーを押して下さい.png"));

	Dc->PicContinueShinai      = LoadPicture(ResourcePath("画像\\コンティニューしない.png"));
	Dc->PicContinueShinaiFocus = LoadPicture(ResourcePath("画像\\コンティニューしない後.png"));
	Dc->PicContinueSuru        = LoadPicture(ResourcePath("画像\\コンティニューする.png"));
	Dc->PicContinueSuruFocus   = LoadPicture(ResourcePath("画像\\コンティニューする後.png"));

	Dc->PicSelectGI = Dc->PicDummy; // LoadPicture(ResourcePath("画像\\ドイツイタリア.png"));
	Dc->PicSelectJP = Dc->PicDummy; // LoadPicture(ResourcePath("画像\\日本プロイセン.png"));

	Dc->PicCpuWall   = Dc->PicDummy; // LoadPicture(ResourcePath("画像\\強さ選択画面.png"));
	Dc->PicCpuCursor = LoadPicture(ResourcePath("画像\\カーソル.png"));

	Dc->PicReady = LoadPicture(ResourcePath("画像\\ready.png"));

	Dc->PicWin      = LoadPicture(ResourcePath("画像\\勝利.png"));
	Dc->PicWinWall  = LoadPicture(ResourcePath("画像\\勝利星.png"));
	Dc->PicWin1P    = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\独伊勝利.png"));
	Dc->PicWin2P    = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\普日勝利.png"));
	Dc->PicLose     = LoadPicture(ResourcePath("画像\\敗北.png"));
	Dc->PicLoseWall = LoadPicture(ResourcePath("画像\\敗北後ろ.png"));
	Dc->PicLose1P   = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\独伊敗北.png"));
	Dc->PicLose2P   = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\普日敗北.png"));

	Dc->PicPauseItem[0][0] = LoadPicture(ResourcePath("System\\Button\\ゲームに戻る.png"));
	Dc->PicPauseItem[0][1] = LoadPicture(ResourcePath("System\\Button\\ゲームに戻る選択中.png"));
	Dc->PicPauseItem[1][0] = LoadPicture(ResourcePath("System\\Button\\ゲームを終了する.png"));
	Dc->PicPauseItem[1][1] = LoadPicture(ResourcePath("System\\Button\\ゲームを終了する選択中.png"));
	Dc->PicPauseItem[2][0] = LoadPicture(ResourcePath("System\\Button\\タイトルに戻る.png"));
	Dc->PicPauseItem[2][1] = LoadPicture(ResourcePath("System\\Button\\タイトルに戻る選択中.png"));
	Dc->PicPauseItem[3][0] = LoadPicture(ResourcePath("System\\Button\\設定.png"));
	Dc->PicPauseItem[3][1] = LoadPicture(ResourcePath("System\\Button\\設定選択中.png"));

	Dc->PicCIStar  = LoadPicture(ResourcePath("カットイン\\Star.png"));
	Dc->PicCI1P1Ren3 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\伊三連鎖.png"));
	Dc->PicCI1P2Ren3 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\日三連鎖.png"));
	Dc->PicCI2P1Ren3 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\独三連鎖.png"));
	Dc->PicCI2P2Ren3 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\普三連鎖.png"));
	Dc->PicCI1P1Ren5 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\伊五連鎖.png"));
	Dc->PicCI1P2Ren5 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\日五連鎖.png"));
	Dc->PicCI2P1Ren5 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\独五連鎖.png"));
	Dc->PicCI2P2Ren5 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\普五連鎖.png"));
	Dc->PicCI1P1Ren7 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\伊最終連鎖.png"));
	Dc->PicCI1P2Ren7 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\日最終連鎖.png"));
	Dc->PicCI2P1Ren7 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\独最終連鎖.png"));
	Dc->PicCI2P2Ren7 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\普最終連鎖.png"));

	Dc->PicCI1P1Damage = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\伊打撃.png"));
	Dc->PicCI1P2Damage = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\日打撃.png"));
	Dc->PicCI2P1Damage = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\独打撃.png"));
	Dc->PicCI2P2Damage = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\普打撃.png"));

	Dc->PicCIHissStar = LoadPicture(ResourcePath("カットイン\\Star必殺.png"));

	Dc->PicCIHiss1P1 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\独必殺技.png"));
	Dc->PicCIHiss1P2 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\伊必殺技.png"));
	Dc->PicCIHiss2P1 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\日必殺技.png"));
	Dc->PicCIHiss2P2 = Dc->PicDummy; // LoadPicture(ResourcePath("カットイン\\普必殺技.png"));

	// 文字 --->

	for(int i = 0; i < 0x100; i++)
	{
		Dc->PicAscii[i] = Dc->PicDummy;
	}
	Dc->PicAscii['!'] = LoadPicture(ResourcePath("フィールド\\数字\\!.png"));
	Dc->PicAscii['"'] = LoadPicture(ResourcePath("フィールド\\数字\\Dq.png"));
	Dc->PicAscii['#'] = LoadPicture(ResourcePath("フィールド\\数字\\!.png"));
	Dc->PicAscii['$'] = LoadPicture(ResourcePath("フィールド\\数字\\$.png"));
	Dc->PicAscii['%'] = LoadPicture(ResourcePath("フィールド\\数字\\%.png"));
	Dc->PicAscii['&'] = LoadPicture(ResourcePath("フィールド\\数字\\&.png"));
	Dc->PicAscii['\'']= LoadPicture(ResourcePath("フィールド\\数字\\Sq.png"));
	Dc->PicAscii['('] = LoadPicture(ResourcePath("フィールド\\数字\\(.png"));
	Dc->PicAscii[')'] = LoadPicture(ResourcePath("フィールド\\数字\\).png"));
	Dc->PicAscii['*'] = LoadPicture(ResourcePath("フィールド\\数字\\Star.png"));

	Dc->PicAscii['0'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_0.png"));
	Dc->PicAscii['1'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_1.png"));
	Dc->PicAscii['2'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_2.png"));
	Dc->PicAscii['3'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_3.png"));
	Dc->PicAscii['4'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_4.png"));
	Dc->PicAscii['5'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_5.png"));
	Dc->PicAscii['6'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_6.png"));
	Dc->PicAscii['7'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_7.png"));
	Dc->PicAscii['8'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_8.png"));
	Dc->PicAscii['9'] = LoadPicture(ResourcePath("フィールド\\数字\\数字_9.png"));

	Dc->PicAscii[';'] = LoadPicture(ResourcePath("フィールド\\数字\\;.png"));
	Dc->PicAscii['<'] = LoadPicture(ResourcePath("フィールド\\数字\\＜.png"));
	Dc->PicAscii['>'] = LoadPicture(ResourcePath("フィールド\\数字\\＞.png"));
	Dc->PicAscii['?'] = LoadPicture(ResourcePath("フィールド\\数字\\？.png"));
	Dc->PicAscii['@'] = LoadPicture(ResourcePath("フィールド\\数字\\@.png"));

	Dc->PicAscii['A'] = LoadPicture(ResourcePath("フィールド\\数字\\A.png"));
	Dc->PicAscii['B'] = LoadPicture(ResourcePath("フィールド\\数字\\B.png"));
	Dc->PicAscii['C'] = LoadPicture(ResourcePath("フィールド\\数字\\C.png"));
	Dc->PicAscii['D'] = LoadPicture(ResourcePath("フィールド\\数字\\D.png"));
	Dc->PicAscii['E'] = LoadPicture(ResourcePath("フィールド\\数字\\E.png"));
	Dc->PicAscii['F'] = LoadPicture(ResourcePath("フィールド\\数字\\F.png"));
	Dc->PicAscii['G'] = LoadPicture(ResourcePath("フィールド\\数字\\G.png"));
	Dc->PicAscii['H'] = LoadPicture(ResourcePath("フィールド\\数字\\H.png"));
	Dc->PicAscii['I'] = LoadPicture(ResourcePath("フィールド\\数字\\I.png"));
	Dc->PicAscii['J'] = LoadPicture(ResourcePath("フィールド\\数字\\J.png"));
	Dc->PicAscii['K'] = LoadPicture(ResourcePath("フィールド\\数字\\K.png"));
	Dc->PicAscii['L'] = LoadPicture(ResourcePath("フィールド\\数字\\L.png"));
	Dc->PicAscii['M'] = LoadPicture(ResourcePath("フィールド\\数字\\M.png"));
	Dc->PicAscii['N'] = LoadPicture(ResourcePath("フィールド\\数字\\N.png"));
	Dc->PicAscii['O'] = LoadPicture(ResourcePath("フィールド\\数字\\O.png"));
	Dc->PicAscii['P'] = LoadPicture(ResourcePath("フィールド\\数字\\P.png"));
	Dc->PicAscii['Q'] = LoadPicture(ResourcePath("フィールド\\数字\\Q.png"));
	Dc->PicAscii['R'] = LoadPicture(ResourcePath("フィールド\\数字\\R.png"));
	Dc->PicAscii['S'] = LoadPicture(ResourcePath("フィールド\\数字\\S.png"));
	Dc->PicAscii['T'] = LoadPicture(ResourcePath("フィールド\\数字\\T.png"));
	Dc->PicAscii['U'] = LoadPicture(ResourcePath("フィールド\\数字\\U.png"));
	Dc->PicAscii['V'] = LoadPicture(ResourcePath("フィールド\\数字\\V.png"));
	Dc->PicAscii['W'] = LoadPicture(ResourcePath("フィールド\\数字\\W.png"));
	Dc->PicAscii['X'] = LoadPicture(ResourcePath("フィールド\\数字\\X.png"));
	Dc->PicAscii['Y'] = LoadPicture(ResourcePath("フィールド\\数字\\Y.png"));
	Dc->PicAscii['Z'] = LoadPicture(ResourcePath("フィールド\\数字\\Z.png"));

	for(int aupr = 'A'; aupr <= 'Z'; aupr++)
	{
		int alwr = aupr + 0x20;

		Dc->PicAscii[alwr] = Dc->PicAscii[aupr];
	}
	Dc->PicAscii['['] = LoadPicture(ResourcePath("フィールド\\数字\\[.png"));
	Dc->PicAscii[']'] = LoadPicture(ResourcePath("フィールド\\数字\\].png"));
	Dc->PicAscii['_'] = LoadPicture(ResourcePath("フィールド\\数字\\_.png"));

	Dc->PicAscii['ｦ'] = LoadPicture(ResourcePath("フィールド\\数字\\wo.png"));

	// <--- 文字

	for(int i = 0; i < 10; i++)
	{
		Dc->PicDir[i] = Dc->PicDummy;
	}
	Dc->PicDir[2] = LoadPicture(ResourcePath("フィールド\\数字\\↓.png"));
	Dc->PicDir[4] = LoadPicture(ResourcePath("フィールド\\数字\\←.png"));
	Dc->PicDir[6] = LoadPicture(ResourcePath("フィールド\\数字\\→.png"));
	Dc->PicDir[8] = LoadPicture(ResourcePath("フィールド\\数字\\↑.png"));

	Dc->PicFace[PZPLYR_1P1][0][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ普通a.png"));
	Dc->PicFace[PZPLYR_1P1][0][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ普通b.png"));
	Dc->PicFace[PZPLYR_1P1][0][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ普通c.png"));
	Dc->PicFace[PZPLYR_1P1][1][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ苦しいa.png"));
	Dc->PicFace[PZPLYR_1P1][1][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ苦しいb.png"));
	Dc->PicFace[PZPLYR_1P1][1][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ苦しいc.png"));
	Dc->PicFace[PZPLYR_1P1][2][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ最悪a.png"));
	Dc->PicFace[PZPLYR_1P1][2][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ最悪b.png"));
	Dc->PicFace[PZPLYR_1P1][2][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\ドイツ最悪c.png"));
	// - - -
	Dc->PicFace[PZPLYR_1P2][0][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア普通a.png"));
	Dc->PicFace[PZPLYR_1P2][0][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア普通b.png"));
	Dc->PicFace[PZPLYR_1P2][0][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア普通c.png"));
	Dc->PicFace[PZPLYR_1P2][1][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア苦しいa.png"));
	Dc->PicFace[PZPLYR_1P2][1][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア苦しいb.png"));
	Dc->PicFace[PZPLYR_1P2][1][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア苦しいc.png"));
	Dc->PicFace[PZPLYR_1P2][2][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア最悪a.png"));
	Dc->PicFace[PZPLYR_1P2][2][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア最悪b.png"));
	Dc->PicFace[PZPLYR_1P2][2][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\イタリア最悪c.png"));
	// - - -
	Dc->PicFace[PZPLYR_2P1][0][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本普通a.png"));
	Dc->PicFace[PZPLYR_2P1][0][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本普通b.png"));
	Dc->PicFace[PZPLYR_2P1][0][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本普通c.png"));
	Dc->PicFace[PZPLYR_2P1][1][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本苦しいa.png"));
	Dc->PicFace[PZPLYR_2P1][1][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本苦しいb.png"));
	Dc->PicFace[PZPLYR_2P1][1][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本苦しいc.png"));
	Dc->PicFace[PZPLYR_2P1][2][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本最悪a.png"));
	Dc->PicFace[PZPLYR_2P1][2][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本最悪b.png"));
	Dc->PicFace[PZPLYR_2P1][2][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\日本最悪c.png"));
	// - - -
	Dc->PicFace[PZPLYR_2P2][0][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン普通a.png"));
	Dc->PicFace[PZPLYR_2P2][0][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン普通b.png"));
	Dc->PicFace[PZPLYR_2P2][0][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン普通c.png"));
	Dc->PicFace[PZPLYR_2P2][1][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン苦しいa.png"));
	Dc->PicFace[PZPLYR_2P2][1][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン苦しいb.png"));
	Dc->PicFace[PZPLYR_2P2][1][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン苦しいc.png"));
	Dc->PicFace[PZPLYR_2P2][2][0] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン最悪a.png"));
	Dc->PicFace[PZPLYR_2P2][2][1] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン最悪b.png"));
	Dc->PicFace[PZPLYR_2P2][2][2] = Dc->PicDummy; // LoadPicture(ResourcePath("顔グラ\\プロイセン最悪c.png"));

	Dc->PicEffeHana   = LoadPicture(ResourcePath("きらきら\\花ピンク透過.png"));
	Dc->PicEffeHoshi  = LoadPicture(ResourcePath("きらきら\\光る星.png"));
	Dc->PicEffeKagaya = LoadPicture(ResourcePath("きらきら\\輝き.png"));

	Dc->PicBlocks[0] = LoadPicture(ResourcePath("ブロック\\お邪魔.png"));
	Dc->PicBlocks[1] = LoadPicture(ResourcePath("ブロック\\黄.png"));
	Dc->PicBlocks[2] = LoadPicture(ResourcePath("ブロック\\紫.png"));
	Dc->PicBlocks[3] = LoadPicture(ResourcePath("ブロック\\青.png"));
	Dc->PicBlocks[4] = LoadPicture(ResourcePath("ブロック\\赤.png"));
	Dc->PicBlocks[5] = LoadPicture(ResourcePath("ブロック\\緑.png"));

	Dc->PicFlowers[0] = LoadPicture(ResourcePath("ブロック\\散りお邪魔.png"));
	Dc->PicFlowers[1] = LoadPicture(ResourcePath("ブロック\\散り黄.png"));
	Dc->PicFlowers[2] = LoadPicture(ResourcePath("ブロック\\散り紫.png"));
	Dc->PicFlowers[3] = LoadPicture(ResourcePath("ブロック\\散り青.png"));
	Dc->PicFlowers[4] = LoadPicture(ResourcePath("ブロック\\散り赤.png"));
	Dc->PicFlowers[5] = LoadPicture(ResourcePath("ブロック\\散り緑.png"));
}
void DcInit_LoadAllPictureSafe(void)
{
	// 変なハンドルを渡して無理やり表示させようとしてる訳だから、やっぱりマズいよね、、、
//	if(DrawGraph(0, 0, Dc->PicBlackWall, 0) != 0) // ? 表示出来ない -> 開放された。
	{
		DcInit_LoadAllPicture();
	}
}

static void LoadResrcScnrList(void)
{
	int fp = DFOpen(ResourcePath("シナリオ_Text.txt"));

	for(; ; )
	{
		char *line = DFReadLine(fp);

		if(line[0] == '\0')
		{
			break;
		}
		char *name = line;

		// -- 拡張子除去 --
		char *p = strchr(name, '.');

		errorCase(!p);
		p[0] = '\0';
		// ----

		// -- Add --
		errorCase(RESRC_SCNR_MAX <= Dc->ResrcScnrCount);

		strcpy(Dc->ResrcScnrList[Dc->ResrcScnrCount], name);
		Dc->ResrcScnrCount++;
		// ----
	}
	DFClose(fp);
}
void DcInit(void)
{
	Dc = (Dc_t *)memCalloc(sizeof(Dc_t));

	DcInit_LoadAllPicture();

	Dc->BGM_Title = LoadBGM(ResourcePath("Puzzle\\音\\MusMus-CT-NV-END_muon-100-100.mp3"));

	Dc->SeReady    = LoadSound(ResourcePath("音\\和太鼓でドドン.mp3"));
	Dc->SeStart    = LoadSound(ResourcePath("音\\ピューンと逃げる.mp3"));
//	Dc->SeFlower   = LoadSound(ResourcePath("音\\モノクロフラワー_少量.wav")); // 不使用
//	Dc->SeFlowerLot= LoadSound(ResourcePath("音\\モノクロフラワー_大量.wav")); // 不使用
	Dc->SeMove     = LoadSound(ResourcePath("音\\カーソル移動4.mp3"));
	Dc->SeRotate   = LoadSound(ResourcePath("音\\カーソル移動7.mp3"));
	Dc->SeErase    = LoadSound(ResourcePath("音\\パッ.mp3"));
	Dc->SeDecide   = LoadSound(ResourcePath("音\\パパッ.mp3"));
//	Dc->SeBomb     = LoadSound(ResourcePath("音\\必殺技.wav")); // 不使用

	Dc->SeIkuyo = LoadSound(ResourcePath("音\\小鼓(こつづみ).mp3"));
	Dc->SeUwa   = LoadSound(ResourcePath("音\\間抜け2.mp3"));
	Dc->SeEi    = LoadSound(ResourcePath("音\\クイズ正解2.mp3"));
	Dc->SeSore  = LoadSound(ResourcePath("音\\クイズ正解3.mp3"));
	Dc->SeHa    = LoadSound(ResourcePath("音\\クイズ正解4.mp3"));
//	Dc->SeGo    = LoadSound(ResourcePath("音\\ゴー.wav"));
//	Dc->SeSpCmd = LoadSound(ResourcePath("音\\スペシャルコマンド.wav")); // 必殺技_廃止
//	Dc->SeRucca = LoadSound(ResourcePath("音\\ルッカ.wav")); // 不使用
//	Dc->SeJReady= LoadSound(ResourcePath("音\\レディー.wav"));
	Dc->SeRen3  = LoadSound(ResourcePath("音\\時代劇演出1.mp3"));
	Dc->SeRen5  = LoadSound(ResourcePath("音\\金額表示.mp3"));
	Dc->SeRen7  = LoadSound(ResourcePath("音\\鳩時計1.mp3"));
//	Dc->SeWin   = LoadSound(ResourcePath("音\\勝利ボイス.wav")); // 廃止
//	Dc->SeLose  = LoadSound(ResourcePath("音\\敗北ボイス.wav")); // 廃止
//	Dc->SeHissOk= LoadSound(ResourcePath("音\\go.mp3")); // 必殺技_廃止

	Dc->SeBellWin  = LoadSound(ResourcePath("音\\ラッパのファンファーレ.mp3"));
	Dc->SeBellLose = LoadSound(ResourcePath("音\\間抜け4.mp3"));
//	Dc->SeBellHiss = LoadSound(ResourcePath("音\\スペシャルコマンドジングル.mp3")); // 必殺技_廃止

	// 旧初期値設定位置、現在この初期値は無効 -> InitSettings()
	Dc->BGMVolume = 0.45;
	Dc->SeVolume = 0.45;

	LoadResrcScnrList();
}
void DcFnlz(void)
{
	// この辺の開放は面倒なのでやめ
	/*
	UnloadPicture(Dc->PicBlackWall);
	memFree(Dc->BGMFile_Game);
	UnloadSound(Dc->SeReady);
	*/

	// memFree(Dc); // 終了間際にも、Dc 使うようになってしまったので、開放しちゃだめ!!
}
