#ifndef _POSITION_H_
#define _POSITION_H_

#include "frwbcm.h"
#include "release.h"
#include <vector>

using namespace std;
extern int type;
//extern std::vector<vector<DPLANE> > dconductors;
//extern std::vector<vector<DPLANE> > dblocks;
void printrange(RANGE &range){
    std::cout << "start =  " << range.start << std::endl;
    std::cout << "end = " << range.end << std::endl;
    std::cout << "insert =  " << range.insert << std::endl;
    std::cout << "layerid  " << range.layerid << std::endl;
    std::cout << "conductorid " << range.conductorid << std::endl;
}

std::vector<vector<RANGE> > mrange;

void RangeSet(RANGE &range, size_t start, size_t end, size_t insert, int layerid, int conductorid){
    
    range.start = start;
    range.end = end;
    range.insert = insert;
    range.layerid = layerid;
    range.conductorid = conductorid;
}

void PositionRange(){

    int blockssize = dblocks.size();
    RANGE range;
    //printrange(range);
    std::vector<RANGE> Range;
    size_t tempstart = 0 ;
    size_t  tempend = 0;
    size_t layerid;
    size_t  conductorid;
    size_t insert;
    //bool isconductor;
    size_t five = 0;
    size_t tlayerid;
    size_t tconductorid;
    unsigned int xEle;
    unsigned int yEle;
    if(type == 0 || type == 1){
        for(int i = 0 ; i < blockssize ; i++){
            five = 0;
            tempstart = tempend + 1;	
        //	std::cout << "dconductors size "  << i << " " << dconductors[i].size() << std::endl;
            if(!dconductors[i][0].isempty){
                for(int j = 0; j < static_cast<int>(dconductors[i].size()) ; j++){
                
                    tlayerid = dconductors[i][j].windows.layerid;
                    tconductorid = dconductors[i][j].windows.conductorid;
                    xEle = dconductors[i][j].windows.xEle;
                    yEle = dconductors[i][j].windows.yEle;
                    if(j == 0){
                        layerid = tlayerid;
                        conductorid = tconductorid;
                        tempend = tempstart + xEle * yEle - 1;
                        five = five + 1;
                    }else if(j > 0){
                        if(layerid == tlayerid && conductorid == tconductorid){
                            tempend = tempend + xEle * yEle;
                            five = five + 1;
                        }else if(((layerid != tlayerid)||(layerid == tlayerid && 
                                conductorid != tconductorid))){
                            insert = MAXNUM;
                            if(five == 5){
                                insert = tempend - xEle * yEle + 1;
                            }else if( i!=0 && five == 1){
                                for(size_t k = 0 ; k < mrange[i-1].size();k++){
                                //	std::cout << mrange[i-1].size() << std::endl;
                                    if(layerid == static_cast<size_t>(mrange[i-1][k].layerid) && 
                                    conductorid == static_cast<size_t>(mrange[i-1][k].conductorid)){
                                        insert = mrange[i-1][k].insert;
                                    //	std::cout << " insert ----- " << insert << std::endl;
                                    }
                                }
                            }
                            RangeSet(range, tempstart, tempend, insert, layerid, conductorid);
                            Range.push_back(range);
                            layerid = tlayerid;
                            conductorid = tconductorid;
                            tempstart = tempend + 1;
                            tempend = tempend + xEle*yEle;
                            five = 1 ;
                        }
                    }if( j == static_cast<int>(dconductors[i].size())-1){
                //		std::cout << " '<<<<....." << i << " "<< five << " " <<j << std::endl;
                        insert = MAXNUM;
                        if(five == 5){
                            insert = tempend - xEle * yEle + 1;
                        }if( i != 0 && five == 1){
                            for(int k = 0 ; k < static_cast<int>(mrange[i-1].size());k++){
                                if(static_cast<size_t>(mrange[i-1][k].layerid) == layerid && 
                                static_cast<size_t>(mrange[i-1][k].conductorid) == conductorid){
                                    insert = mrange[i-1][k].insert;
                                }
                            }
                        }
                        RangeSet(range, tempstart, tempend, insert, layerid, conductorid);
                        Range.push_back(range);
                    }
                }	
            }
            tempstart = tempend + 1;
            //std::cout << "dblock size " << dblocks[i].size() << std::endl;
            for( int j = 0 ; j < static_cast<int>(dblocks[i].size()); j++){
                tlayerid = dblocks[i][j].windows.layerid;
                tconductorid = dblocks[i][j].windows.conductorid;
                xEle = dblocks[i][j].windows.xEle;
                yEle = dblocks[i][j].windows.yEle;
            //	std::cout << "position  " << i << " " << j << " " << xEle << " " << yEle <<std::endl;;
                if( static_cast<size_t>(i + 1) == tlayerid && tconductorid == MAXNUM){
                    tempend = tempend + xEle * yEle;
                }
            }
            RangeSet(range,tempstart, tempend, MAXNUM, i+1, MAXNUM);
            Range.push_back(range);
            mrange.push_back(Range);
            std::vector<RANGE>().swap(Range);
        }
    }else if (type == -1){
        int inverse_i = 0;
        for(int i = blockssize - 1 ; i >= 0 ; i--){
            five = 0;
            inverse_i = abs(i - blockssize + 1);
        //	std::cout << "inverse _ i " << i << " " << inverse_i << std::endl;
            tempstart = tempend + 1;
            if(!dconductors[i][0].isempty){
                for(int j = 0; j < static_cast<int>(dconductors[i].size()) ; j++){
                
                    tlayerid = dconductors[i][j].windows.layerid;
                    tconductorid = dconductors[i][j].windows.conductorid;
                    xEle = dconductors[i][j].windows.xEle;
                    yEle = dconductors[i][j].windows.yEle;
                    if(j == 0){
                        layerid = tlayerid;
                        conductorid = tconductorid;
                        tempend = tempstart + xEle * yEle - 1;
                        five = five + 1;
                    }else{
                        if(layerid == tlayerid && conductorid == tconductorid){
                            tempend = tempend + xEle * yEle;
                            five = five + 1;
                        }if((layerid != tlayerid)||(layerid == tlayerid &&
                            conductorid != tconductorid)){
                            insert  = MAXNUM;
                            if(five == 1){
                                insert = tempend + 1;
                            }if(inverse_i != 0 && five == 5){
                                for(int k = 0 ; k < static_cast<int>(mrange[inverse_i-1].size());k++){
                                    if(static_cast<size_t>(mrange[inverse_i-1][k].layerid) == layerid && 
                                    static_cast<size_t>(mrange[inverse_i-1][k].conductorid) == conductorid){
                                        insert = mrange[inverse_i-1][k].insert;
                                    }
                                }
                            }
                            RangeSet(range, tempstart, tempend, insert, layerid, conductorid);
                            Range.push_back(range);
                            layerid = tlayerid;
                            conductorid = tconductorid;
                            tempstart = tempend + 1;
                            tempend = tempend + xEle*yEle;
                            five = 1;
                        }
                    }if( j == static_cast<int>(dconductors[i].size())-1){
                            insert = MAXNUM;
                            if(five == 1){
                                insert = tempend  + 1;
                            }if(inverse_i!=0 && five == 5){
                                for(int k = 0 ; k < static_cast<int>(mrange[inverse_i-1].size());k++){
                                    if(static_cast<size_t>(mrange[inverse_i-1][k].layerid) == layerid && 
                                    static_cast<size_t>(mrange[inverse_i-1][k].conductorid) == conductorid){
                                        insert = mrange[inverse_i-1][k].insert;
                                    }
                                }
                            }
                        //	std::cout << "five " << five  << " " << insert << std::endl;
                            RangeSet(range, tempstart, tempend, insert, layerid, conductorid);
                            Range.push_back(range);
                    }
                }	
            }
            tempstart = tempend + 1;
            for( int j = 0 ; j < static_cast<int>(dblocks[i].size()); j++){
                tlayerid = dblocks[i][j].windows.layerid;
                tconductorid = dblocks[i][j].windows.conductorid;
                xEle = dblocks[i][j].windows.xEle;
                yEle = dblocks[i][j].windows.yEle;
                if( static_cast<size_t>(inverse_i + 1) == tlayerid && tconductorid == MAXNUM){
                    tempend = tempend + xEle * yEle;
                }
            }
            RangeSet(range,tempstart, tempend, MAXNUM, inverse_i+1, MAXNUM);
            Range.push_back(range);
            mrange.push_back(Range);
            std::vector<RANGE>().swap(Range);	
        }
    }
    //release_vector();
/*std::cout << "mrange.size " << mrange.size() <<std::endl;;
    for(int i = 0 ; i< mrange.size();i++){
        for(int j = 0 ; j < mrange[i].size();j++){
            std::cout << "mrange[i].size " << i << " "  << mrange[i].size()<<std::endl;;
            std::cout << "---------******------" << std::endl;
            printrange(mrange[i][j]);
            std::cout << "-------------------" << std::endl;
        }
    }*/
}

#endif	
