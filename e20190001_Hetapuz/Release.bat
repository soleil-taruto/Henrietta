C:\Factory\Tools\RDMD.exe /RC out

COPY /B Hetapuz\Release\Hetapuz.exe out\Hetapuz.exe
COPY /B doc\* out

C:\Dat\Henrietta\e20190001_DxLib\DxLib_VC3_22c\Tool\DXArchive\DXAEncode.exe -K:65536-524288 dat out\Resource.dxa

C:\Factory\SubTools\zip.exe /O out Hetapuz
