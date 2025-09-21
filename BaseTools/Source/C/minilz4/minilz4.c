/** @file
  LZ4 Compress/Decompress tool (minilz4) Based on LZ4 Library 

  Copyright (c) 2017,2018, Qualcomm Technologies INC. All rights reserved.<BR>
  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Sdk/C/lz4.h"
#include "Sdk/C/lz4hc.h"
#include "Sdk/C/Types.h"
#include "CommonLib.h"
#define MAXBUFLEN 1000000

typedef enum {
  NoConverter, 
  X86Converter,
  MaxConverter
} CONVERTER_TYPE;

const char *kCantReadMessage = "Can not read input file";
const char *kCantWriteMessage = "Can not write output file";
const char *kCantAllocateMessage = "Can not allocate memory";
const char *kDataErrorMessage = "Data error";

static Bool mQuietMode = False;
static CONVERTER_TYPE mConType = NoConverter;

#define UTILITY_NAME "minilz4"
#define UTILITY_MAJOR_VERSION 0
#define UTILITY_MINOR_VERSION 2
#define QCOM_COPYRIGHT \
  "Copyright (c) 2017 Qualcomm Technologies Inc. All rights reserved."
void PrintHelp(char *buffer)
{
  // strcat(buffer,
  //     "\n" UTILITY_NAME " - " QCOM_COPYRIGHT "\n"
  //     "\nUsage:  minilz4 -e|-d [options] <inputFile>\n"
  //            "  -e: encode file\n"
  //            "  -d: decode file\n"
  //            "  -o FileName, --output FileName: specify the output filename\n"
  //            "  --f86: enable converter for x86 code\n"
  //            "  -v, --verbose: increase output messages\n"
  //            "  -q, --quiet: reduce output messages\n"
  //            "  --debug [0-9]: set debug level\n"
  //            "  --version: display the program version and exit\n"
  //            "  -h, --help: display this help text\n"
  //            );
}
int File_GetLength(FILE *p, UInt64 *length)
{
  
  long pos = ftell(p);
  int res = fseek(p, 0, SEEK_END);
  *length = ftell(p);
  fseek(p, pos, SEEK_SET);
  return res;
  
}
int SeqInStream_Read2(FILE *stream, void *buf, size_t size, int errorType)
{
  if(stream != NULL)
  {
    fread(buf,size,1,stream);
  }
  
  return SZ_OK;
}

int SeqInStream_Read(FILE *stream, void *buf, size_t size)
{
  return SeqInStream_Read2(stream, buf, size, SZ_ERROR_INPUT_EOF);
}
int PrintError(char *buffer, const char *message)
{
  // strcat(buffer, "\nError: ");
  // strcat(buffer, message);
  // strcat(buffer, "\n");
  return 1;
}

int PrintErrorNumber(char *buffer, int val)
{
  // sprintf("fileSize","%d %d",val,val);
  return 1;
}

int PrintUserError(char *buffer)
{
  return PrintError(buffer, "Incorrect command");
}

void PrintVersion(char *buffer)
{
  // sprintf (buffer, "%s Version %d.%d %s ", UTILITY_NAME, UTILITY_MAJOR_VERSION, UTILITY_MINOR_VERSION, __BUILD_VERSION);
}


int main2(int numArgs, const char *args[], char *rs)
{
  int encodeMode = 0;
  Bool modeWasSet = False;
  const char *inputFile = NULL;
  const char *outputFile = "file.tmp";
  int param;
  UInt64 fileSize = 0;

  if (numArgs == 1)
  {
    PrintHelp(rs);
    return 0;
  }

  for (param = 1; param < numArgs; param++) {
  
    if (strcmp(args[param], "-e") == 0 || strcmp(args[param], "-d") == 0) {
      encodeMode = (args[param][1] == 'e');
      modeWasSet = True;
    } else if (strcmp(args[param], "--f86") == 0) {
      mConType = X86Converter;
    } else if (strcmp(args[param], "-o") == 0 ||
               strcmp(args[param], "--output") == 0) {
      if (numArgs < (param + 2)) {
        return PrintUserError(rs);
      }
      outputFile = args[++param];
    } else if (strcmp(args[param], "--debug") == 0) {
      if (numArgs < (param + 2)) {
        return PrintUserError(rs);
      }
      //
      // For now we silently ignore this parameter to achieve command line
      // parameter compatibility with other build tools.
      //
      param++;
    } else if (
                strcmp(args[param], "-h") == 0 ||
                strcmp(args[param], "--help") == 0
              ) {
      PrintHelp(rs);
      return 0;
    } else if (
                strcmp(args[param], "-v") == 0 ||
                strcmp(args[param], "--verbose") == 0
              ) {
      //
      // For now we silently ignore this parameter to achieve command line
      // parameter compatibility with other build tools.
      //
    } else if (
                strcmp(args[param], "-q") == 0 ||
                strcmp(args[param], "--quiet") == 0
              ) {
      mQuietMode = True;
    } else if (strcmp(args[param], "--version") == 0) {
      PrintVersion(rs);
      return 0;
    } else if (inputFile == NULL) {
      inputFile = args[param];
    } else {
      return PrintUserError(rs);
    }
  }
 
  if ((inputFile == NULL) || !modeWasSet) {
    return PrintUserError(rs);
  }
  {
    size_t t4 = sizeof(UInt32);
    size_t t8 = sizeof(UInt64);
    if (t4 != 4 || t8 != 8)
      return PrintError(rs, "Incorrect UInt32 or UInt64");
  }

  FILE *fpInput = fopen(inputFile, "rb"); 
  FILE *fpOutput = fopen(outputFile, "wb+");

  if(fpInput == NULL)
    return PrintError(rs, "Can not open input file");
  if (fpOutput == NULL)
    return PrintError(rs, "Can not open output file");
  File_GetLength(fpInput, &fileSize);
 
  char *source = malloc (fileSize);
  char *dest = malloc(fileSize);
  if(source == NULL || dest == NULL)
    return PrintError(rs, "Reading Failed");
  if (SeqInStream_Read(fpInput, source, fileSize) != SZ_OK) {
  PrintError(rs, "error in reading file");
  }
  if (encodeMode)
  { 
   // fileSize = LZ4_compress_fast(source, dest, fileSize, fileSize,9);
   fileSize = LZ4_compress_default(source, dest, fileSize, fileSize);

   if(fileSize <= 0)
    { 
        PrintError(rs, "\n\ncompression Failed\n\n");
    }
  
    if(fwrite(dest, 1, fileSize, fpOutput) != fileSize)
    {
        PrintError(rs, "Failed to write bytes to file\n");
        exit(1);
    }
  }
  else
  {

    fileSize = LZ4_decompress_safe(source, dest, fileSize, MAXBUFLEN);

    if(fwrite(dest, 1, fileSize, fpInput) != fileSize)
    {
        printf("ERROR - Failed to write bytes to file\n");
        exit(1);
    }

  }
  free(source);
  free(dest);
  fclose(fpInput);
  fclose(fpOutput);
return 0;
}

int MY_CDECL main(int numArgs, const char *args[])
{
  char rs[2000] = { 0 };
  int res = main2(numArgs, args, rs);
  if (strlen(rs) > 0) {
    puts(rs);
  }
  return res;
}
