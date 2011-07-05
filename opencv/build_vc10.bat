@rem
@rem This is a batch file to build OpenCV-rtcs. 
@rem
@rem Using:
@rem   + Please install the following tools beforehand. 
@rem     -- cmake
@rem     -- vc2010
@rem     -- OpenCV
@rem   + Please set path of cmake and vc2010.  
@rem

@set OPENCV_ROOT=C:\distribution\OpenCV2.2
@set WindowsSdkDir=
@for /F "tokens=1,2*" %%i in ('reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows" /v "CurrentInstallFolder"') DO (
	if "%%i"=="CurrentInstallFolder" (
		set "WindowsSdkDir=%%k"
	)
)

@set PATH="C:\Program Files\CMake 2.8\bin";"C:\Program Files\Microsoft Visual Studio 10.0\VC\BIN";"c:\WINDOWS\Microsoft.NET\Framework\v4.0.30319";"C:\Program Files\Microsoft Visual Studio 10.0\VC\vcpackages";%PATH%

echo Generating SoLutio files / WorkSpace.
cmake -G "Visual Studio 10"

msbuild /M:2 /t:rebuild /p:configuration=release ImageProcessing_opencv.sln

