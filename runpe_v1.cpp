#include <cstdio>
#include <iostream> // Standard C++ library for console I/O
#include <stdio.h>
#include <Windows.h>
#include <fstream> // for std::fstream
#include <sys/stat.h>
#include <TlHelp32.h> // Process API

static int PeLdrLoadImage(PE_LDR_PARAM *pe)
{
	HANDLE	hFile = NULL;
	HANDLE	hMap = NULL;
	BOOL	ret = FALSE;
	_PPEB	peb;

	if (!pe)
		goto out;
	
	DMSG("Creating Map View of File");

	HRSRC myResource = ::FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
	unsigned int myResourceSize = ::SizeofResource(NULL, myResource);
	HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
	void* pMyBinaryData = ::LockResource(myResourceData);

	char* buffer = new char[myResourceSize];
	memcpy(buffer, pMyBinaryData, myResourceSize);

	int key = 128;

	for (int i = 0; i < myResourceSize; i++) {
		buffer[i] ^= key;
	}

	pe->dwImageSizeOnDisk = myResourceSize;

	pe->dwImage = (DWORD)buffer;
	if (!pe->dwImage) {
		DMSG("Failed to obtain a map view of PE File");
		goto out;
	}
	

	DMSG("Map View of File created");

	pe->pDosHeader = (PIMAGE_DOS_HEADER)pe->dwImage;
	if (pe->pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		DMSG("DOS Signature invalid");
		goto out;
	}

	pe->pNtHeaders = (PIMAGE_NT_HEADERS)(((DWORD)pe->dwImage) + pe->pDosHeader->e_lfanew);
	if (pe->pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
		DMSG("NT Signature mismatch");
		goto out;
	}

	peb = (_PPEB)__readfsdword(0x30);
	pe->dwLoaderBase = (DWORD)peb->lpImageBaseAddress;

	ret = TRUE;

out:

	return ret;
}

int main(int argc, char **argv) {

}