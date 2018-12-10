
set VS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community
set VCVARS=%VS%\VC\Auxiliary\Build\vcvars32.bat
echo %VCVARS%
call "%VCVARS%"
set VCTargetsPath=%VS%\Common7\IDE\VC\VCTargets
MSBuild.exe rpc.sln  /p:Configuration=Release /p:Platform=X86
MSBuild.exe rpc.sln  /p:Configuration=Debug /p:Platform=X86