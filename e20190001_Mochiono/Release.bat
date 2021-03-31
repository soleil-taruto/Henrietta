C:\Factory\Tools\RDMD.exe /RC out

COPY /B Mochiono\Release\Mochiono.exe out\Mochiono.exe
COPY /B doc\* out

C:\Dat\Henrietta\e20190001_DxLib\DxLib_VC3_15a\Tool\DXArchive\DXAEncode.exe -K:65536-524288 dat out\Resource.dxa

C:\Factory\SubTools\zip.exe /O out Mochiono
