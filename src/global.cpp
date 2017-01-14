#include "sdef.h"
#include "elenode.h"

ushort ** usElements,** usTempEle, **usTempEleCon;
int  Conblk,Curblk,iDivideDef=0,iRefineLevel=0,iErrorFlag=0;

/*
    The header file containing the definition of the shareing structure.
    Any code inserted into this file must be completely C style.
*/
