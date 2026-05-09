#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "zlib.h"
#include "defzlib.h"
#include "defrReader.h"

/*
 * Private functions:
 */
size_t defGZip_read(FILE* file, char* buf, size_t len) {
  return (gzread((gzFile)file, buf, len));
}

/*
 * Public functions:
 */
defGZFile 
defGZipOpen(const char *gzipPath, const char* mode) {
  defGZFile fptr;

  if (!gzipPath)
    return NULL;

  if (fptr = gzopen(gzipPath, mode)) {
    /* successfully open the gzip file */
    /* set the read function to read from a compressed file */
    defrSetReadFunction(defGZip_read);
    return (defGZFile)fptr;
  } else
    return NULL;
}

int defGZipClose(defGZFile filePtr) {
  return (gzclose((gzFile)filePtr));
}

int defrReadGZip(defGZFile file, const char* gzipFile, defiUserData uData) {
  return defrRead((FILE*)file, gzipFile, uData, 1);
}
