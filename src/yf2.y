%{
#include <fstream>
#include <string.h>
#include "shape.h"
#include "elenode.h"
#include <iostream>
#include <vector>
using namespace std;
char *ReservedWordsList[]={
                       (char *)"all",
                       (char *)"axis1",
                       (char *)"axis2",
                       (char *)"axis3",
                       (char *)"at",
                       (char *)"begin",
		       (char *)"block",
                       (char *)"capacitance",
                       (char *)"chrg",
                       (char *)"conductor",
                       (char *)"const",
                       (char *)"cylinder",
                       (char *)"cut",
                       (char *)"debug", 
                       (char *)"diel",	
                       (char *)"element",
		       (char *)"elementgap",
                       (char *)"end",
                       (char *)"endb",
                       (char *)"endc",
                       (char *)"endl",
                       (char *)"endm",
                       (char *)"ends",
                       (char *)"endp",
                       (char *)"for",
                       (char *)"in",
		       (char *)"inloop",
		       (char *)"layernum",
                       (char *)"local",
                       (char *)"mastergap",
                       (char *)"medium",
                       (char *)"merge",
		       (char *)"outloop",
                       (char *)"override",
                       (char *)"plane",
                       (char *)"poly",
                       (char *)"print",
                       (char *)"radius",
                       (char *)"resistance",
                       (char *)"scale",
                       (char *)"sphere",
		       (char *)"touch",	
                       (char *)"vertex",
		       (char *)"volt",
                       (char *)"window",
		       (char *)"with",
                       (char *)"B3D_Format",
                       (char *)"point",		//yuwj, 2013.3
					   (char *)"vtxlist",	//add by [whquan]
					   (char *)"outloop3d",	//add by [whquan]
					   (char *)"medium_interface",	//add by [whquan]
					   (char *)"within"		//add by [whquan]
            };
const int TokenLen=sizeof(ReservedWordsList)/sizeof(ReservedWordsList[0]);
const int ConstPos=10;
const char RemarkChar=0x2f;


#define point_maxno 200

typedef struct tagOBJECTFIELD {
  char Name[32];
}OBJECTFIELD;

/*panhui1996.12.24*/
OBJECTFIELD *ObjField;
static WORD ObjNum;

/*panhui1996.12.24*/
char **ConstVarList;

double *ConstValList;
static int ConstVarNum;

TPOINT2D *pddlPoint2DBuf;
TPOINT *pddlPoint3DBuf;	//add by [whquan]
TPOINT2D *pddlOutLoopPoint2DBuf;
TPOINT *pddlOutLoopPoint3DBuf;	//add by [whquan]
TPOINT *pddlInterfacePoint3DBuf;	//add by [whquan]
TPOINT **pddlInterfacePoint3DBufMul;	//add by [whquan]
 WORD   pddlPoint2DNum;
 WORD   pddlPoint3DNum;	//add by [whquan]
 WORD   pddlInterfacePoint3DNum;	//add by [whquan]
 WORD   pddlOutLoopPoint2DNum;
 WORD   pddlOutLoopPoint3DNum;	//add by [whquan]

TPOINT observePoint;		/* yuwj 2013.3 */
//add by [whquan]
#define NULL_TRANS_AREA			0
#define CUBE_TRANS_AREA 		1
#define PRISM_TRANS_AREA  		2
#define PRISM_TSV_TRANS_AREA 	3
#define FRUSTUM_TRANS_AREA		4

//ADD BY [whquan]
extern TBDELEINFO **gBdElements;
extern TGEOMETRYINFO *tempblock2;	//to save every block geometry info

vector<CONDUCTORINFO> conductors;
CONDUCTORINFO cond;

extern int trans_area_type[10];
extern void point2Dto3D(int);
extern void point3D_save(int);
extern int pointInMediumId;

extern ifstream *tpddlin;
extern int block_number;
extern int cond_number;
extern bool bcm;
extern bool frw;
int InterfaceNum; 

extern uint Curmed;
ushort miline;
char tmp[32];
double *tempelec;
double lu_diel;
int layer;
char * Seperate=(char *)"-------------------------------------------\n";
/*
 *function prototype define here!
 */
int  yylex();
void yyerror(char *s);// Shenbd
BOOL IsAlpha(char);
BOOL IsDigit(char);
int  DoAlpha(ifstream *);
int  DoDigit(ifstream *);

%}
%start program
%union {
   
	double fval;
   int ival;
   TVECTOR       vector;
   TPOINT2D      pt2D;
   TPOINT		 pt3D;
   TGEOMETRYINFO tgGeomInfo;
   TGEOMETRYINFO2 tgGeomInfo2;
   TELECTRICITYINFO teElecInfo;
   TBOUNDELEINFO tbBouEleInfo;
}

%token  ALL       300
%token  AXIS1     301 
%token  AXIS2     302 
%token  AXIS3     303 
%token  AT        304 
%token  BEGIN     305 
%token  BLOCK     306 
%token  CAPACITANCE     307 
%token  CHRG     	308
%token  CONDUCTOR  	309 
%token  CONST    	310
%token  CYLINDER     	311
%token  CUT    		312
%token  DEBUG1    	313
%token  DIEL    	314
%token  ELEMENT  	315
%token  ELEMENTGAP      316
%token  END       317
%token  ENDB      318
%token  ENDC      319
%token  ENDL      320
%token  ENDM      321
%token  ENDS      322
%token  ENDP      323
%token  FOR       324
%token  In        325
%token  INLOOP    326
%token  LAYERNUM     327
%token  LOCAL     328
%token  MASTERGAP 329
%token  MEDIUM    330
%token  MERGE     331
%token  OUTLOOP   332
%token  OVERRIDE  333
%token  PLANE     334
%token  POLY      335
%token  PRINT     336
%token  RADIUS    337
%token  RESISTANCE 338
%token  SCALE      339
%token  SPHERE     340
%token  TOUCH      341
%token  VERTEX     342
%token  VOLT       343
%token  WINDOW     344
%token  WITH       345
%token  B3D_FORMAT        346
%token  POINT      347
%token  VTXLIST      348
%token	OUTLOOP3D	349
%token	MEDIUM_INTERFACE	350
%token	WITHIN	351

%token  CONSTVAR1  	400
%token  CONSTVAR2  	401

%token <ival> NEWOBJ 	500
%token <ival> OLDOBJ    501
 
%token <ival> MYEOF     600	
%token <fval> FREG      601

/*-----------------------------------------
 *define return types
 *---------------------------------------*/

%type  <ival>  CONSTVAR1
%type  <ival>  CONSTVAR2
%type  <ival>  OBJ
%type  <ival>  OBJEQUDEF
%type  <fval> express
%type  <fval> Factor
%type <vector> VECTOR
%type <pt2D>   POINT2D
%type <pt3D>   POINT3D	
%type <tgGeomInfo> LOCALCOORDEF
%type <tgGeomInfo> OBJGEOMDEF
%type <tgGeomInfo2> OBJGEOMDEF2
%type <tbBouEleInfo> OBJBOUNDDEF
%type <vector> POINTDEF
%type <teElecInfo>   OBJELECDEF
%type <fval>	OBJSCALEDEF
%type  <ival>  ELECOBJ
%type <ival> DIELDEF
%type <ival> LAYER
%left '+' '-'
%left '*' '/'
%left UMINUS
%type  <ival>  HEADER;

%%
program:begin LINES end
       ;
begin: BEGIN
    {
		ConstVarNum=0;
		ObjNum=0;
		iErrorFlag=0;
		miline=0;
		Curmed=0;
		Curblk=0;
		Conblk=0;
		InterfaceNum = 0 ; 
		int i;
		tempelec = new double [block_number];

		usTempEle = new ushort * [block_number];
		if (cond_number > 0){
			usTempEleCon = new ushort * [cond_number];
		}
		for (i=0;i<block_number;i++)
			usTempEle[i] = new ushort [3];
		for (i=0;i<cond_number;i++)
			usTempEleCon[i] = new ushort [3];
		//add by [whquan]
		tempblock2 = new TGEOMETRYINFO[block_number];
		if(cond_number>=1){
			if(block_number!=1){
				cout << "Wrong input block number!" << endl;
			}
		}
	
		gBdElements = new TBDELEINFO * [block_number];
		
		ObjField = new OBJECTFIELD [4*(block_number+cond_number)];
		ConstVarList = new char * [2*(block_number+cond_number)];
		for (i=0;i<2*(block_number+cond_number);i++)
		   ConstVarList[i] = new char [32];
		if(ConstVarList==NULL) 
			printf("ERROR: not enough memory!\n");
		ConstValList = new double [2*(block_number+cond_number)];   

		pddlPoint2DBuf = new TPOINT2D [point_maxno];
		pddlPoint3DBuf = new TPOINT [point_maxno];	//add by [whquan]
		pddlOutLoopPoint2DBuf = new TPOINT2D [point_maxno];
		pddlOutLoopPoint3DBuf = new TPOINT [point_maxno];	//add by [whquan]
		//add by [whquan]
		pddlPoint3DNum=0;
		pddlOutLoopPoint3DNum=0;
    }
    ;
LINES:
     |  LINES LINE
     ;
end: END
    {
		int i;
		delete[] pddlPoint2DBuf;
		delete[] pddlPoint3DBuf;	//add by [whquan]
		delete[] pddlOutLoopPoint2DBuf;
		delete[] pddlOutLoopPoint3DBuf;	//add by [whquan]

		for (i=0;i<2*(block_number+cond_number);i++)
		{
			delete[] ConstVarList[i];
		}
		delete[] ConstVarList;
		delete[] ConstValList;
		delete[] ObjField;
    }
LINE:MEDIUMDEF
      |POINTDEF
	|MEDIUM_INTERFACEDEF
		|CONDUCTORDEF
    ;

//MEDIUMDEF:MEDIUMTAG ELECOBJ SHAPEDEFS ENDM
//modified by [whquan]
MEDIUMDEF:MEDIUMTAG ELECOBJ SHAPEDEFS DIELDEF ENDM
		{
			Curmed++;
		}
	;
MEDIUMTAG:MEDIUM
	 
/* added by lutt */
/* to get the netno and the mediuno in the conductor and medium line */
ELECOBJ:OBJ

DIELDEF: DIEL express
		{
			lu_diel=$2;
			tempelec[Curmed]=lu_diel;
		}
	;

SHAPEDEFS:SHAPEDEF
	 |SHAPEDEFS SHAPEDEF
	 ;

SHAPEDEF:BLOCK_DEF
        |POLYDEF
	|VTXLIST_DEF
        ;

express:Factor
	   {
		$$=$1;
           }
 	 |express  '+' express 
	   {
	      $$=$1+$3;
           }
         |express  '-' express 
	   {
		$$=$1-$3;
           }
          ;
Factor:FREG
         {
            $$=$1;
         }
       |CONSTVAR2
	 {
	    $$=ConstValList[$1];
         }
       |'(' express ')'
	{
		$$=$2;
        }
       |Factor '*' Factor
	{
	   $$=$1*$3;
        }
       |Factor '/' Factor
	{
	    $$=$1/$3;
        }
       |'-' Factor %prec UMINUS
	{
	  $$=-$2;
        }
       ;

VECTOR:'(' express  ',' express  ',' express  ')'
   {
    $$.x=$2;
    $$.y=$4;
    $$.z=$6;
   }
   ;

OBJ:NEWOBJ
	     {
		$$=-$1-1;
	     }
   |OLDOBJ
	     {
		$$=$1;
	     }
   ;

OBJGEOMDEF:AT VECTOR AXIS1 VECTOR AXIS2 VECTOR AXIS3 VECTOR
	    {
			$$.basePoint=$2;
			$$.xAxis=$4;
			$$.yAxis=$6;
			$$.zAxis=$8;
        }
         ;

OBJELECDEF:DIEL express
		{
			$$.mask=ENABLE_DIEL;
			$$.dielVal=$2;
		}
	|
		{
			$$.mask=ENABLE_NONE_VALUE;
		}
          ;
OBJBOUNDDEF:ELEMENT VECTOR
		{
			$$.mask=ENABLE_3D;
            $$.xAxis=(long)($2.x);
			$$.yAxis=(long)($2.y);
			$$.zAxis=(long)($2.z);
		}
	      |
		{
			$$.mask=ENABLE_NONE;
			$$.xAxis=(long)(0);
			$$.yAxis=(long)(0);
			$$.zAxis=(long)(0);
		}
	    ;
		  
POINTDEF:POINT VECTOR
		{
			$$.x= (double)($2.x);
			$$.y= (double)($2.y);
			$$.z= (double)($2.z);
			observePoint= $$;
		}
		|POINT VECTOR WITHIN express
		{
			$$.x= (double)($2.x);
			$$.y= (double)($2.y);
			$$.z= (double)($2.z);
			observePoint= $$;
			pointInMediumId = (int)$4;
		}
		;
MEDIUM_INTERFACEDEF:MEDIUM_INTERFACE POINT3D POINT3DMORE
		{
			//add by [whquan]
			if(frw){
				pddlInterfacePoint3DNum = pddlPoint3DNum;
				pddlInterfacePoint3DBuf = new TPOINT[pddlInterfacePoint3DNum];
           			 for(int i = 0 ; i < static_cast<int>(pddlPoint3DNum) ; i++) 
					pddlInterfacePoint3DBuf[i] = pddlPoint3DBuf[i]; 
         				   pddlPoint3DNum = 0; 
			}if(bcm){ // add by [zhangbl]
				pddlInterfacePoint3DNum = pddlPoint3DNum;
				pddlInterfacePoint3DBufMul = new TPOINT * [Curblk-1];
				for (int i = 0 ; i < Curblk-1 ; i++){
					pddlInterfacePoint3DBufMul[i] = new TPOINT [pddlInterfacePoint3DNum];
				}
				for ( int i = 0 ; i < static_cast<int>(pddlPoint3DNum) ; i ++){
					pddlInterfacePoint3DBufMul[InterfaceNum][i]= pddlPoint3DBuf[i];
					pddlPoint3DNum = 0 ;
					 
				}
				InterfaceNum = InterfaceNum + 1;
			}	
		}

OBJSCALEDEF:
		{
			$$=1.0;
		}
           |SCALE express
		{
			$$=$2;
  	        }
           ;		
OBJEQUDEF:
		{
			$$=-1;
		}
         |'=' OLDOBJ
		{
			$$=$2;
		}
	;
	 
BLOCK_DEF:BLOCK  OBJ OBJEQUDEF LOCALCOORDEF OBJGEOMDEF OBJELECDEF OBJBOUNDDEF ENDB
		{
			//ADD BY [whquan]
			trans_area_type[Curblk] = CUBE_TRANS_AREA;
			tempblock2[Curblk] = $5;
			usTempEle[Curblk][0]=(ushort)$7.xAxis;
   			usTempEle[Curblk][1]=(ushort)$7.yAxis;
   			usTempEle[Curblk][2]=(ushort)$7.zAxis;
			Curblk++;
	    }
        ;

POLYDEF:POLY OBJ OBJEQUDEF LOCALCOORDEF OBJGEOMDEF OBJSCALEDEF VERTEXDEFS OBJELECDEF OBJBOUNDDEF ENDP
		{			
			//add by [whquan]
			tempblock2[Curblk] = $5;
			point2Dto3D(Curblk);
			usTempEle[Curblk][0]=(ushort)$9.xAxis;
			usTempEle[Curblk][1]=(ushort)$9.yAxis;
			usTempEle[Curblk][2]=(ushort)$9.zAxis;
			Curblk++;
			pddlPoint2DNum=0;
		}
	;

VTXLIST_DEF:VTXLIST OBJ LOOPDEF OBJBOUNDDEF ENDP
		{	//add by [whquan]
			trans_area_type[Curblk] = FRUSTUM_TRANS_AREA;
			point3D_save(Curblk);
			usTempEle[Curblk][0] = (ushort)$4.xAxis;
			usTempEle[Curblk][1] = (ushort)$4.yAxis;
			usTempEle[Curblk][2] = (ushort)$4.xAxis;
			Curblk++;
		};

LOCALCOORDEF:
		;
	
OBJGEOMDEF2:
		;

HEADER:
		;

LOOPDEF:OUTLOOPDEF

OUTLOOPDEF:OUTLOOP OBJ VERTEXDEFS  
	   {
		   pddlOutLoopPoint2DNum = pddlPoint2DNum;
		   int i;
		   for(i = 0 ; i < static_cast<int>(pddlPoint2DNum) ; i++) 
			  pddlOutLoopPoint2DBuf[i] = pddlPoint2DBuf[i]; 
		   pddlPoint2DNum = 0; 
	   }
	   |OUTLOOP3D OBJ VERTEXDEFS
	   {
		//add by [whquan]
            pddlOutLoopPoint3DNum = pddlPoint3DNum;
			int i;
            for( i = 0 ; i < static_cast<int>(pddlPoint3DNum) ; i++) 
				pddlOutLoopPoint3DBuf[i] = pddlPoint3DBuf[i]; 
            pddlPoint3DNum = 0; 
	   };

VERTEXDEFS:VERTEXDEF VERTEXMORE

VERTEXMORE:
	  |VERTEXMORE VERTEXDEF
VERTEXDEF:VERTEX POINT2D POINT2D POINT2D POINT2DMORE
	 {
			//add by [whquan]
			trans_area_type[Curblk] = PRISM_TRANS_AREA;
	 }
	 |VERTEX POINT2D POINT2D POINT2D POINT2DMORE ';' POINT2D POINT2D POINT2D POINT2DMORE
	 {
			//add by [whquan] to recognize tsv	
			trans_area_type[Curblk] = PRISM_TSV_TRANS_AREA;		
	 }
	 |VERTEX POINT3D POINT3DMORE
	 {
		//add by [whquan]
		//do something you want to do
	 }
POINT3DMORE:
		|POINT3D
			|POINT3DMORE POINT3D
		;
POINT3D:VECTOR
		{
			//add by [whquan]
			$$=$1;
			pddlPoint3DBuf[pddlPoint3DNum++]=$$;
		}
POINT2DMORE:
	   |POINT2D
           |POINT2DMORE POINT2D
	   ;
POINT2D:'('express','express')'
		{
			$$.x=$2;
			$$.y=$4;
			pddlPoint2DBuf[pddlPoint2DNum++]=$$;
		}
CONDUCTORDEF:CONDUCTOR OBJ OBJEQUDEF LOCALCOORDEF OBJGEOMDEF OBJBOUNDDEF LAYER ENDC
		{//add by [zhangbl]
			cond.layerNum = $7;
			cond.geo = $5;
			cond.xEle=(ushort)$6.xAxis;
   			cond.yEle=(ushort)$6.yAxis;
   			cond.zEle=(ushort)$6.zAxis;
			conductors.push_back(cond);
	    }
        ;
LAYER:LAYERNUM express
{
	$$=$2;
}

%%

int yylex()         
{
  char ch;

  do{
    ch=tpddlin->get();
    if (ch==EOF)
      return EOF;
    if(ch==RemarkChar){
	char NextChar;

	NextChar=tpddlin->get();
	if(NextChar==RemarkChar){
		while(NextChar!='\n')
			NextChar=tpddlin->get();
		ch=tpddlin->get();
	}
	else
		if(NextChar=='*') {
			char FirstChar;
			
			NextChar=tpddlin->get();
			do {
				FirstChar=NextChar;
				NextChar =tpddlin->get();
			} while(!(FirstChar=='*' && NextChar==RemarkChar));
			ch=tpddlin->get();
		}
		else
			tpddlin->putback(NextChar);
   }
		
    if (ch=='\n')
      miline++;
   }while ((ch==' ')||(ch=='\n')||(ch=='\t')||(ch=='\r'));
    tpddlin->putback(ch);

    if (IsAlpha(ch))
       return DoAlpha(tpddlin);
    else
        if (IsDigit(ch))
	    return DoDigit(tpddlin);
        else 
	     return tpddlin->get(); 
}

BOOL IsAlpha(char ch)
{
     if ((ch>='A')&&(ch<='Z'))
       return TRUE;
     if ((ch>='a')&&(ch<='z'))
        return TRUE;
     if  (ch=='_')
	 return TRUE;

     return FALSE;
}
BOOL IsDigit(char ch)
{
     if ((ch>='0')&&(ch<='9'))
	return TRUE;

      return FALSE;
}

int DoAlpha(ifstream *input)
{
   int i,pos;
   static int ConstFlag=0; 

   char str[32],ch;

   ch=input->get();
   pos=0;
   while(IsAlpha(ch)||IsDigit(ch)){
	str[pos++]=ch;
	ch=input->get();
   }
   str[pos]=0;
   input->putback(ch);
   
   for (i=0;i<TokenLen;i++)
      if(strcmp(str,ReservedWordsList[i])==0)
	{
		yylval.ival=i;
		if(i==ConstPos)/*if it is 'const'*/
			ConstFlag=1;

		return 300+i;
	}

   for(i=0;i<static_cast<int>(ObjNum);i++)
      if(strcmp(str,ObjField[i].Name)==0)
	{
		yylval.ival=i;
		return OLDOBJ;
	}

  for (i=0;i<ConstVarNum;i++)
      if(strcmp(str,ConstVarList[i])==0)
	{
		yylval.ival=i;
		return CONSTVAR2;
	}

   if(ConstFlag){
	strcpy(ConstVarList[ConstVarNum],str);
	yylval.ival=ConstVarNum;
	ConstVarNum++;
	ConstFlag=0;
	return CONSTVAR1;
    }
   else {
     	strcpy(ObjField[ObjNum].Name,str);
	strcpy(tmp,str);
       	yylval.ival=ObjNum;
     	ObjNum++;
       	return NEWOBJ;
    }
}

int DoDigit(ifstream *input)
{
    double i;
    *input>>i;
    yylval.fval=i; 

    return FREG;
}

void yyerror(char *s)  // Shenbd
{
  iErrorFlag=1;
}
