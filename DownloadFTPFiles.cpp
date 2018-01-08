/*
*author:CHRT
* this project used to download continuous date 
*igsg ionex map file from ftp
*/

#include <stdio.h>  
#include <windows.h>  
#include <wininet.h>  
#include <process.h>  
#include <string>
#include<atlconv.h>
#include<fstream>
#pragma comment(lib, "WININET.LIB") 

using namespace std;


int main()
{
	/**************************/
	//change time there and doy is day of year from 1 to 365or366
	int yearStart = 0;
	int yearEnd = 0;
	int doyStart = 0;
	int doyEnd = 0;

	//change destination folder here and the filename is yyyyddd.Z
	std::string dst = "E://data//";
	
	//log file
	ofstream outf("E://dataDownload.log");
	/***********************/


	char filename[255] = "\0";
	char newFile[255] = "\0";
	
	HINTERNET hIntSession;
	HINTERNET hFtpSession;
	LPCSTR sever = "198.118.242.40";
	LPCSTR username = "anonymous";
	LPCSTR password = "";
	DWORD dErrorCode = 101;
	LPCSTR lpcwstrFilename, lpcwstrSaveFilename;
	int countPrint = 0;
	BOOL bSuccess;

	printf("try to open intSession...\n");
	hIntSession = InternetOpen("downloading"/*L for trans char* to LPCWSTR*/,
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hIntSession == NULL)
	{
		printf("can not open intSession!\n");
		return -1;
	}
	printf("intSession is open!\n");

	printf("try to open ftpSession...\n");
	hFtpSession = InternetConnect(hIntSession, sever,
		INTERNET_DEFAULT_FTP_PORT, username,
		password, INTERNET_SERVICE_FTP, 0, 0);
	if (hFtpSession == NULL)
	{
		InternetCloseHandle(hIntSession);
		printf("can not open ftpSession!\n");
		return -1;
	}
	printf("ftpSession is open!\n");

	printf("try to get file...\n");
	
	for (int year = 2014; year < 2015; year++) {
		for (int doy = 252; doy < 256; doy++) {
			sprintf(filename, 
				"//gnss//products//ionex//%d//%d//igsg%d0.%02di.Z", 
				year, doy, doy, year - 2000);
			sprintf(newFile, "%s%d%d.Z", dst, year, doy);
			
			//transform char* to LPCWSTR
			USES_CONVERSION;
			lpcwstrFilename = filename;
			lpcwstrSaveFilename = newFile;
			
			//try to download target file 3 times if failed 
			//the error code will be logged in log file you named upside
			for (int i = 0; i < 3; i++) {
				bSuccess = FtpGetFile(
					hFtpSession, lpcwstrFilename, lpcwstrSaveFilename,
					TRUE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 0);
				if (bSuccess) {
					printf("%s | ", lpcwstrSaveFilename);
					outf << lpcwstrSaveFilename << " | ";
					countPrint++;
					if (countPrint % 5 == 0) {
						outf << "\n";
						printf("\n");
					}
					break;
				}
			}

			if (!bSuccess) {
				dErrorCode = GetLastError();
				outf << "\n " << lpcwstrSaveFilename << " download unsuccessful===error code:" << dErrorCode << endl;
				printf("%s download unsuccessful\n", lpcwstrSaveFilename);
			}
		}
	}


	outf.close();
	//InternetCloseHandle(hFind);  
	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hIntSession);

	return 0;
}
