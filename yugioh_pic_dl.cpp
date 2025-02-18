#include "dbquery.h"
#include <iostream>
#include <fstream>

#define DBPATH "/home/xyx98/gitrepo/yugioh-pic-dl/cards.cdb"
#define DLPATH "/home/xyx98/gitrepo/yugioh-pic-dl/pics"

int main(){
    dbquery query=dbquery(DBPATH);
    if(!query.isOK){
        std::cout<<"fail to open database." <<std::endl;
        return 0;
    }
    std::ofstream file("id.txt");
    int32_t cid=0;
    while(true){
        cid=query.nextID();
        if (cid>=0) file<<cid<<std::endl;
        else break;
    }
    return 0;
}