C:\Factory\Tools\RDMD.exe /RC out

SET RAWKEY=6def9100bea64878b9f2a10d9c8d75b9

C:\Factory\SubTools\makeCluster.exe Picture.txt     out\Picture.dat     %RAWKEY% 110000000
C:\Factory\SubTools\makeCluster.exe Music.txt       out\Music.dat       %RAWKEY% 120000000
C:\Factory\SubTools\makeCluster.exe SoundEffect.txt out\SoundEffect.dat %RAWKEY% 130000000
C:\Factory\SubTools\makeCluster.exe Etcetera.txt    out\Etcetera.dat    %RAWKEY% 140000000

COPY /B Hako\Release\Hako.exe out
COPY /B AUTHORS out
C:\Factory\Tools\xcp.exe doc out

out\Hako.exe /L
IF ERRORLEVEL 1 START ?_LOG_ENABLED

MD out\Map
COPY /B Resource\* out\Map

MD out\Replay
COPY /B Resource\* out\Replay

C:\Factory\SubTools\zip.exe %* /G out Hako
