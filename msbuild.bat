set VCTargetsPath="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\VCTargets"
MSBuild.exe rpc.sln  /p:Configuration=Release /p:Platform=X86
MSBuild.exe rpc.sln  /p:Configuration=Debug /p:Platform=X86