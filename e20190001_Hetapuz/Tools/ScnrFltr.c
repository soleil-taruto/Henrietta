/*
	ScnrFltr.exe /d <dir>   -- dir内をチェック
	ScnrFltr.exe /d         -- ドロップしたdir内をチェック
	ScnrFltr.exe            -- ドロップしたファイルをチェック
*/

#include "C:\Factory\Common\all.h"

#define CHARANAME_LKAKKO "【"
#define CHARANAME_RKAKKO "】"

#define STRFILE_GRAPH_FILES "..\\dat\\シナリオ_画像.txt"
#define STRFILE_SOUND_FILES "..\\dat\\シナリオ_音.txt"

#define STRFILE_RESULT "C:\\temp\\結果.txt"

static int IsZenLine(char *line)
{
	char *p;

	if(strlen(line) % 2 != 0)
	{
		return 0;
	}
	for(p = line; *p; p += 2)
	{
		if(!_ismbblead(p[0]) || !_ismbbtrail(p[1]))
		{
			return 0;
		}
	}
	return 1;
}

static int IsExistResource(char *name, char *listFile, char *ext)
{
	autoList_t *lines;
	char *line;
	uint index;
	int retval;

	if(!*name) return 0;
//	if(!IsZenLine(name)) return 0;

	lines = readLines(listFile);

	name = addExt(strx(name), ext);
	retval = findJLineICase(lines, name) < getCount(lines);
	memFree(name);

	releaseDim(lines, 1);
	return retval;
}
static int IsExistGraph(char *name)
{
	return IsExistResource(name, STRFILE_GRAPH_FILES, "png");
}
static int IsExistSound(char *name)
{
	return IsExistResource(name, STRFILE_SOUND_FILES, "mp3");
}

typedef struct Line_st
{
	uint Row;
	char *Line;
}
Line_t;
static Line_t *Lines;
static uint LineCount;

static FILE *OutFp;
static void Warning(uint row, char *message)
{
	fprintf(OutFp, "警告(行番号: %u): %s\n", row, message);
}
static void ScnrFltrEx(char *scnrFile)
{
	autoList_t *lines = readLines(scnrFile);
	char *line;
	uint index;
	Line_t *i;

	errorCase(!OutFp);
	fprintf(OutFp, "シナリオ \"%s\" チェック結果\n", scnrFile);

	LineCount = getCount(lines);
	Lines = (Line_t *)memAlloc(LineCount * sizeof(Line_t));

	foreach(lines, line, index)
	{
		i = Lines + index;
		i->Row = index + 1;
		i->Line = line;
	}
	releaseAutoList(lines);

restart:
	for(index = 0; index < LineCount; index++) // コメントを除外
	{
		i = Lines + index;
		if(i->Line[0] == ';')
		{
			LineCount--;
			copyBlock(i, Lines + index + 1, (LineCount - index) * sizeof(Line_t)); // g
			goto restart;
		}
	}
	for(index = 0; index < LineCount; index++)
	{
		i = Lines + index;
		line = i->Line;

		if(line[0] == '@') // ? 命令
		{
			autoList_t *tokens = tokenize(line, ' ');

			if(getCount(tokens) == 1)
			{
				char *cmd = refLine(tokens, 0);

				     if(!strcmp(cmd, "@--")) {} // フェードアウト効果
				else if(!strcmp(cmd, "@++")) {} // ホワイトアウト効果
				else if(!strcmp(cmd, "@RS")) {} // ルート選択
				else
				{
					Warning(i->Row, "認識できない命令です。文字列に誤りはありませんか、空白を入れ忘れていませんか。(tkn=1)");
				}
			}
			else if(getCount(tokens) == 2)
			{
				char *cmd = refLine(tokens, 0);
				char *option = refLine(tokens, 1);

				if(!strcmp(cmd, "@BG")) // 背景変更
				{
					if(!IsExistGraph(option) && _stricmp(option, "off"))
					{
						Warning(i->Row, "画像が見つかりません。画像の名前を確認して下さい、画像リストが最新であるか確認して下さい。");
					}
				}
				else if(!strcmp(cmd, "@BGM")) // 曲変更
				{
					if(!IsExistSound(option) && _stricmp(option, "off"))
					{
						Warning(i->Row, "サウンドが見つかりません。サウンドの名前を確認して下さい、サウンドリストが最新であるか確認して下さい。");
					}
				}
				else if(!strcmp(cmd, "@SE")) // 効果音
				{
					if(!IsExistSound(option))
					{
						Warning(i->Row, "効果音が見つかりません。効果音の名前を確認して下さい、効果音リストが最新であるか確認して下さい。");
					}
				}
				else
				{
					Warning(i->Row, "認識できない命令です。文字列に誤りはありませんか、空白を入れ忘れていませんか。(tkn=2)");
				}
			}
			else if(getCount(tokens) == 3)
			{
				char *cmd = refLine(tokens, 0);
				char *option1 = refLine(tokens, 1);
				char *option2 = refLine(tokens, 2);

				     if(!strcmp(option1, "1")) {}
				else if(!strcmp(option1, "2")) {}
				else if(!strcmp(option1, "3")) {}
				else if(!strcmp(option1, "4")) {}
				else                           Warning(i->Row, "立ち位置の番号が不正です。(1～4)");

				if(!strcmp(cmd, "@DISP")) // キャラ表示
				{
					if(!IsExistGraph(option2) && _stricmp(option2, "off"))
					{
						Warning(i->Row, "画像が見つかりません。画像の名前を確認して下さい、画像リストが最新であるか確認して下さい。");
					}
				}
				else if(!strcmp(cmd, "@EFFE")) // キャラエフェクト
				{
					if(!strcmp(option2, "揺れる")) {}
					else
					{
						Warning(i->Row, "認識できないエフェクトです。文字列に誤りはありませんか。");
					}
				}
				else
				{
					Warning(i->Row, "認識できない命令です。文字列に誤りはありませんか、空白を入れ忘れていませんか。(tkn=3)");
				}
			}
			else
			{
				Warning(i->Row, "命令の単語数が多すぎるか少なすぎます。不用な空白はありませんか。");
			}
			releaseDim(tokens, 1);
		}
		// ? キャラ名
		else if(
			line[0] == CHARANAME_LKAKKO[0] &&
			line[1] == CHARANAME_LKAKKO[1]
			)
		{
			uint len = strlen(line);

			if(!IsZenLine(line))
			{
				Warning(i->Row, "キャラ名に不正な文字があります。半角とかだめよ。");
			}
			else if(
				line[len - 2] != CHARANAME_RKAKKO[0] ||
				line[len - 1] != CHARANAME_RKAKKO[1]
				)
			{
				Warning(i->Row, "キャラ名が閉じられていません。【】←これ");
			}
		}
		else if(line[0]) // ? メッセージ
		{
			if(!IsZenLine(line))
			{
				Warning(i->Row, "メッセージに不正な文字があります。半角とかだめよ。");
			}
		}
		else // ? 空行
		{}
	}
	if(LineCount == 0)
	{
		Warning(0, "このシナリオファイルは空か、コメントしかありません。空のシナリオは処理できません。");
	}
	else // 空ではない、、、
	{
		if(Lines[0].Line[0] == '\0') // ? 最初の有効な行が空行
		{
			Warning(0, "最初のメッセージや命令の前に空行があります。きっと正しく処理できません。");
		}
	}
	for(index = 1; index < LineCount; index++)
	{
		Line_t *i1 = Lines + index - 1;
		Line_t *i2 = Lines + index - 0;

		if(i1->Line[0] == '\0' && i2->Line[0] == '\0')
		{
			Warning(i2->Row, "連続する空行があります。シナリオの終端と見なされる場合があります。");
		}
	}

	for(index = 0; index < LineCount; index++)
	{
		i = Lines + index;
		if(i->Line[0])
		{
			goto foundEntity;
		}
	}
	Warning(0, "有効なメッセージや命令が一つもありません。シナリオは少なくとも一つはメッセージや命令がなければなりません。");
foundEntity:

	1; // Lines: g!
}
static void ScnrFltr(char *scnrFile)
{
	OutFp = fileOpen(STRFILE_RESULT, "wt");

	ScnrFltrEx(scnrFile);

	fileClose(OutFp);
	OutFp = NULL;
	execute_x(xcout("START %s", STRFILE_RESULT)); // NB
}
int main(int argc, char **argv)
{
	if(argIs("/D")) // Dir mode
	{
		autoList_t *files;
		char *file;
		uint index;

		cout("DIR mode\n");
		files = lsFiles(hasArgs(1) ? nextArg() : dropDir());

		OutFp = fileOpen(STRFILE_RESULT, "wt");

		foreach(files, file, index)
		{
			cout("%s\n", file);
			ScnrFltrEx(file);
		}
		fileClose(OutFp);
		OutFp = NULL;
		execute_x(xcout("START %s", STRFILE_RESULT)); // NB

		return;
	}

	for(; ; )
	{
		ScnrFltr(dropFile());
	}
}
