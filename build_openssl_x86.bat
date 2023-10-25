echo "请确保已经安装有perl"
echo "如果没有nasm, 在perl命令中加上no-asm参数"

set buildDir=%~dp0
cd openssl
mkdir build32
cd build32
perl ..\Configure VC-WIN32 no-shared --prefix=%buildDir%\openssl_output\x86
call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
nmake
nmake install