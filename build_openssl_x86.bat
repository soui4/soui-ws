echo "��ȷ���Ѿ���װ��perl"
echo "���û��nasm, ��perl�����м���no-asm����"

set buildDir=%~dp0
cd openssl
mkdir build32
cd build32
perl ..\Configure VC-WIN32 no-shared --prefix=%buildDir%\openssl_output\x86
call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
nmake
nmake install