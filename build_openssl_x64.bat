echo "请确保已经安装有perl"
echo "如果没有nasm, 在perl命令中加上no-asm参数"

set buildDir=%~dp0
cd openssl
mkdir build64
cd build64
perl ..\Configure VC-WIN64A no-shared --prefix=%buildDir%\openssl_output\x64
call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86_amd64
nmake
nmake install