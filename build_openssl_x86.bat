echo "��ȷ���Ѿ���װ��perl"
echo "���û��nasm, ��perl�����м���no-asm����"

set buildDir=%~dp0
cd openssl
mkdir build32
cd build32
perl ..\Configure VC-WIN32 --prefix=%buildDir%\openssl_output\x86_dll
call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
nmake
nmake install