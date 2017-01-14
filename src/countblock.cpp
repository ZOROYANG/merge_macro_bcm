//////////panhui1996.12.19 
//////////to count the number of block/////////////*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "elenode.h"
#include "countblock.h"
void CountBlock(char *filename,int *blockno, int *mediumno,int *vertexno,int *condno)
{
    FILE *fp;
    char ch,str[32];
    int block=0,medium=0,vertex=0,cond=0;
    int i;
    if ((fp=fopen(filename,"r"))==NULL)
    {
        printf("cannot open this file\n");
        exit(1);
        
    }
    ch=fgetc(fp);
    while (!feof(fp))
    {
       while ((ch==' ')||(ch=='\n')||(ch=='\r')||(ch=='\t'))
           { ch=fgetc(fp);}
       if ( feof(fp) ) break;
       i=0;
       do
       {
         str[i]=ch;
         ch=fgetc(fp);
         i++;
        }
       while ((ch<'z')&&(ch>'a'));
       if (i<32) str[i]='\0';
       if (strlen(str)==4)
       {
           if ((strcmp("endb",str)==0)||(strcmp("endl",str)==0)||
                   (strcmp("ends",str)==0)||(strcmp("endp",str)==0))
                block++;
           if ( strcmp("endm",str) == 0 )
                medium++;
           if ( strcmp("endc",str) == 0 )
                cond++;
       }
       else  if (strcmp("vertex",str)==0)
           do
           {
             if (ch==')') vertex++;
             ch=fgetc(fp);
             if (feof(fp)) break;
           }
           while ((ch>'z')||(ch<'a'));
        
    }
    fclose(fp);
    *blockno = block;
    *mediumno = medium;
    *vertexno = vertex;
    *condno = cond;
    if (block==0) iErrorFlag=1;
}  
