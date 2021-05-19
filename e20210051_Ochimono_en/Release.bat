C:\Factory\Tools\RDMD.exe /RC out
C:\Factory\Tools\RDMD.exe /RM tmp

COPY /B Ochimono\Release\Ochimono.exe out\Ochimono-NT.exe
COPY /B doc\* out

ROBOCOPY dat tmp /MIR
Tools\RemoveUnreleaseResource.exe /Unsafe tmp
C:\Dat\Henrietta\e20190001_DxLib\DxLib_VC3_22c\Tool\DXArchive\DXAEncode.exe -K:65536-524288 tmp out\Resource.dxa

C:\Factory\SubTools\zip.exe %* /G out Ochimono-NT
