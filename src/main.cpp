
#include <sys/times.h>
#include <sys/time.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include "elenode.h"
#include "shape.h"	//added by yuwj

using namespace std;

char *ErrMsg=NULL;
char *fbuf;
char filen[120];

ifstream *tpddlin;

int Changefg=0,Loadflg=0,textDisplay=0;
int iNewZZFlag=0;
int displayElements = 1;
int vol_no;
int block_number=0;//panhui1996.12.19
int cond_number=0; //add by zhangbl
int medium_number=0;
int vertex_number=0;

uint Curvar=0;
uint Curcon=0;
uint Curmed;//1996.11.28.pm
unsigned int ErrLocate;
bool multiCal=false;  //add by caix 2013.7

extern TPOINT observePoint; //yuwj. 2013.3
extern bool needWeight;	//add by [whquan]
extern bool useFDM; // add by [ym]

extern void CountBlock(char *,int *, int *,int *,int*);//panhui1996.12.19
//add by [whquan]
extern void tablegen();
extern void bcmgen(const bool cohesion, const bool standard); //add by zhangbl
extern int yyparse(void);

bool standard_v = false;
bool standard_h = false;
bool standard = false;

bool bcm = false; 
bool frw = false; // add by zhangbl
bool binary = true;
bool readable = true;
bool cohesion = false;
std::string filename;

int main(int  argc,char *argv[] )
{
    std::cout << "*****************************************************" << std::endl;
    std::cout << "/   Boundary Capacitance Matrix Generating Program  /" << std::endl; 
    std::cout << "*****************************************************" << std::endl;
    int i;
    float second;
    struct tms  ti1,ti2;
    
    if(argc<2)
    {
        printf("Systax:Program Unfinished!\n");
        exit(1);
    }
    
    //add by [whquan]
    needWeight = false;

    std::string method_tag(argv[3]); //add by zhangbl
    if(method_tag == "-BCM" || method_tag == "-bcm"){
        std::cout << " Generating Boundary Capacitance Matrix " <<  std::endl; 
        bcm = true;	// add by zhangbl
        if (argc > 5){
            std::string datatype(argv[4]);
            if (datatype == "-b"){
                readable = false;
            }if (datatype == "-r"){
                binary = false;
            }
        }
        std::string cohesionp(argv[argc-1]);
        if(cohesionp == "-cohesion"){
            cohesion = true;
        }
        
        // add by [ym]
        std::string useFDMp(argv[argc-2]);
        if(useFDMp == "-fdm"){
            useFDM = true;
            std::cout << "Calculate Boundary Capacitance Matrix using FDM..." << std::endl;
        }
    }else if(method_tag == "-frw" || method_tag == "-FRW"){
        frw =true;
        for(i=4;i<argc;i++)
        {
            if(argv[i][0]=='-')
            {
                //add by [whquan]
                if(argv[i][1] == 'w' || argv[i][1] == 'W')
                    needWeight = true;
                else if(argv[i][1] =='m'||argv[i][1]=='M')
                    multiCal =true;
        }
        }
    }
        
    std::string stand(argv[argc-1]);
    if(stand == "--standard-h"){
        standard_h = true;
        standard = true;
    }
    if(stand == "--standard-v"){
        standard_v = true;
        standard = true;
    }
//	std::cout << argv[2]<<std::endl;
    ifstream tpddlin1;
        tpddlin1.open(argv[2]);

    if(!tpddlin1)
    {
        printf("Can not open file %s!\n",argv[2]);
        exit(1);
    }
    
    string totalpath(argv[2]);
    size_t fp = totalpath.rfind("/");
    string FileName = totalpath.substr(fp+1);
    size_t fp1 = FileName.find(".");	
    filename = FileName.substr(0,fp1);
    cout << "dirname is: " << filename << endl;
    
    struct timeval timestart;
    struct timeval timeend;
    double timeuse;
    gettimeofday(&timestart,NULL);
    
    tpddlin=&tpddlin1;
    CountBlock(argv[2],&block_number,&medium_number,&vertex_number,&cond_number);

    yyparse();
    printf("Yacc. Complete!\n");
    tpddlin->close();
    if(iErrorFlag){
        std::cout << " Error in Yacc" <<std::endl;
        exit(1);
    }
    cout<<"Medium Number:"<<Curmed<<endl;
    cout <<"Counductor Number:"<< cond_number << endl;

    //add by [whquan]
    if(frw){
        tablegen();
    }
    if(bcm){//add by zhangbl
    //	cout <<"Counductor Number:"<< cond_number << endl;
        bcmgen(cohesion, standard);
    }
    
    gettimeofday(&timeend,NULL);
    timeuse = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
    timeuse = timeuse / 1000000;
    std::cout << "Total time: " << timeuse << std::endl;	
    return(0);
}
