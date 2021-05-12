CALL Build.bat

rem	CALL Release.bat /B
	CALL Release.bat /V 100

C:\Factory\Petra\UpdatingCopy.exe out C:\be\Web\DocRoot\Henrietta\*
C:\Factory\Petra\RunOnBatch.exe C:\be\Web Upload.bat
