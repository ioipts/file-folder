/*
 * Copyright (c) 2023 Pit Suwongs, พิทย์ สุวงศ์ (Thailand)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 * products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * By Pit Suwongs <admin@ornpit.com>
 */
 
#ifndef AXISFILE_H_
#define AXISFILE_H_

/**
* @file axisfile.h
* @brief file & folder manipulation
*
* requirement
* - C++11 
*
* features
* - written in C++11
* - support multi-platform
* 
*/

#include <string.h>
#include <stdio.h>		
#include <stdlib.h>   
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>

#define TIME time_t
#define FILEPTR FILE*
#define FOPEN fopen
#define FWRITE fwrite
#define FREAD fread
#define FFLUSH fflush
#define FCLOSE fclose
#define FEOF feof

#if defined(_MSC_VER)
 //we do not want the warnings about the old deprecated and unsecure CRT functions 
 //since these examples can be compiled under *nix as well
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX
#include <Windows.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <stdint.h>

#pragma warning( disable : 4996 )				// disable deprecated warning

#if defined(_M_X64) 
#define X64
#define FSEEK _fseeki64
#define FTELL _ftelli64
#else
#define X32
#define FSEEK fseek
#define FTELL ftell
#endif

#define SEPERATORCHAR '\\'
#define SEPERATORSTR "\\"
#define PACKED

#else 

#if defined(__APPLE__)
#include <sys/stat.h>
#include <sys/time.h>
#include <utime.h>
#include <unistd.h>     
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

#else	
//linux
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/mman.h>	
#include <utime.h>
#include <unistd.h>  
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

#endif

#if defined(__arm__) || defined(__aarch64__)
#define X64
#define FTELL ftello
#define FSEEK fseeko
#else
#define X32
#define FTELL ftell
#define FSEEK fseek
#endif

#define SEPERATORCHAR '/'
#define SEPERATORSTR "/"
#define PACKED

#endif

#pragma pack(push,1)

/**
* If file exists
*/
bool isFileExists(const char* file);

/**
* get last modified time
*/
TIME getLastModifiedTime(const char* filename);

/**
* set last modified time
*/
bool setLastModifiedTime(const char* filename, TIME t);

/**
* move file
*/
bool deleteFile(const char* file);

/**
* move file
*/
bool copyFile(const char* filename, const char* newfilename);

/**
* move file 
*/
bool moveFile(const char* filename, const char* newfilename);

/**
* list all files in given path
*/
std::vector<std::string> listFiles(const char* path);

/**
* If the folder exists
*/
bool isFolderExists(const char* path);

/**
* Recursively create folder
*/
bool makeFolder(const char* path);

/**
* Recursively delete folder
*/
bool deleteFolder(const char* path);

/**
* list folder for the given path
*/
std::vector<std::string> listFolders(const char* path);


#endif
