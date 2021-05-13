C:\Factory\Tools\RDMD.exe /RC out

SET RAWKEY=9303eb89cf8b2c7a5ac704cf2484dedc

C:\Factory\SubTools\makeCluster.exe Picture.txt     out\Picture.dat     %RAWKEY% 110000000
C:\Factory\SubTools\makeCluster.exe Music.txt       out\Music.dat       %RAWKEY% 120000000
C:\Factory\SubTools\makeCluster.exe SoundEffect.txt out\SoundEffect.dat %RAWKEY% 130000000
C:\Factory\SubTools\makeCluster.exe Etcetera.txt    out\Etcetera.dat    %RAWKEY% 140000000

COPY /B Hako2\Release\Hako2.exe out
COPY /B AUTHORS out
C:\Factory\Tools\xcp.exe doc out

out\Hako2.exe /L
IF ERRORLEVEL 1 START ?_LOG_ENABLED

MD out\Map
COPY /B Resource\* out\Map

MD out\Replay
COPY /B Resource\* out\Replay

C:\Factory\SubTools\zip.exe %* /G out Hako2
