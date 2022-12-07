
	
xcopy			/y			.\Engine_JH\Bin\Engine_JH.lib		.\Reference\Librarys\
xcopy			/y			.\Engine_JH\ThirdPartyLib\*.lib		.\Reference\Librarys\
xcopy			/y			.\Engine_JH\Bin\Engine_JH.dll		.\Client_JH\Bin\Debug
xcopy			/y			.\Engine_JH\Bin\Engine_JH.dll		.\Client_JH\Bin\Release
xcopy			/y/s			.\Engine_JH\public\*.*				.\Reference\Headers\