echo "��ȷ���Ѿ���װ��perl"
echo "���û��nasm, ��perl�����м���no-asm����"

set buildDir=%~dp0
cd openssl
mkdir build64
cd build64
perl ..\Configure VC-WIN64A no-shared --prefix=%buildDir%\openssl_output\x64
call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86_amd64
nmake
nmake install