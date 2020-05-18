#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "zlib.h"

//#include "deflate.h"
//#include "C:\Users\Acer\Desktop\zlibDev\include\inflate.h"
/* zpipe.c: example of proper use of zlib's inflate() and deflate()
   Not copyrighted -- provided to the public domain
   Version 1.4  11 December 2005  Mark Adler */

/* Version history:
   1.0  30 Oct 2004  First version
   1.1   8 Nov 2004  Add void casting for unused return values
                     Use switch statement for inflate() return values
   1.2   9 Nov 2004  Add assertions to document zlib guarantees
   1.3   6 Apr 2005  Remove incorrect assertion in inf()
   1.4  11 Dec 2005  Add hack to avoid MSDOS end-of-line conversions
                     Avoid some compiler warnings for input and output buffers
 */
 #define OFFSETBITS 5
#define LENGTHBITS (8-OFFSETBITS)

#define OFFSETMASK ((1 << (OFFSETBITS)) - 1)
#define LENGTHMASK ((1 << (LENGTHBITS)) - 1)

#define GETOFFSET(x) (x >> LENGTHBITS)
#define GETLENGTH(x) (x & LENGTHMASK)
#define OFFSETLENGTH(x,y) (x << LENGTHBITS | y)

int main(){



FILE *fp=fopen("metin.txt","r");
FILE *fp1=fopen("encoded.txt","r");
FILE *fp2=fopen("decoded.txt","r");
char * buffer = 0;
long length;
char *b;
char *c;


if (fp)
{
  fseek (fp, 0, SEEK_END);
  length = ftell (fp);
  fseek (fp, 0, SEEK_SET);
  buffer = malloc (length);
  if (buffer)
  {
  	
    fread (buffer, 1, length, fp);
    //printf(buffer);
  }
  fclose (fp);
}

if (buffer)
{

    uLong ucompSize = strlen(buffer)+1; //
    uLong compSize = compressBound(ucompSize);
    b=malloc(compSize);
    c=malloc(compSize);

// Deflate
   compress((Bytef *)b, &compSize, (Bytef *)buffer, ucompSize);
   uncompress((Bytef *)c, &ucompSize, (Bytef *)b, compSize);
  //compress(buffer, &a, "testing", 7);
   printf("deflate:%lu\n%s\n",strlen(b),b);
   fprintf(fp1,"deflate:%lu\n%s\n",strlen(b),b);
   printf("inflate:%lu\n%s",strlen(c),c);
   fprintf(fp2,"inflate:%lu\n%s\n",strlen(c),c);
   

  // start to process your data / extract strings here...
}



    /*long a;
    char buffer[1024];
    a = 1024;
    compress(buffer, &a, "testing", 7);
    printf("%s",buffer);*/
    
    
   
}

