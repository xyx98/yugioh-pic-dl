#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <getopt.h>

#include <curl/curl.h>
#include <omp.h>

#include "dbquery.h"
#include "yugioh_pic_dl.h"

namespace fs = std::filesystem;

int picDL(int32_t cid,std::string dlpath){
    fs::path basedir(dlpath);
    if (!fs::exists(basedir)) fs::create_directory(basedir);

    std::string picName = std::to_string(cid)+".jpg";
    CURL *curl = curl_easy_init();
    if (curl == nullptr) return -1;
    fs::path picPath = basedir / picName;

    if (fs::exists(picPath)) return 1; //check if downloaded 

    std::ofstream file(picPath, std::ios::binary);

    curl_easy_setopt(curl, CURLOPT_URL, (BASEURL+picName).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK){
        file.close();
        if (fs::exists(picPath)) fs::remove(picPath);
        return -2;
    };
    file.close();
    curl_easy_cleanup(curl);
    return 0;
}


static size_t writeCallback(void* ptr, size_t size, size_t nmemb, std::ofstream* file) {
        size_t totalSize = size * nmemb;
        file->write(reinterpret_cast<char*>(ptr), totalSize);
        return totalSize;
}

inline void usage(){
    std::cout << "usage:  yugioh-pic-dl --db cards.cdb --output pics" << std::endl 
        << "\t-h,--help \t\t\t show help." << std::endl 
        << "\t-d,--db <string> \t\t path of cards.cdb used in ygopro." << std::endl 
        << "\t-o,--output <string> \t\t path to save downloaded pics." << std::endl
        << "\t-f,--max-fail <int> \t\t stop after target fails,default is 0, disabled." << std::endl
        << "\t-t,--threads <int> \t\t thread for download,default is " << DTHREAD <<"." << std::endl;
}

int main(int argc, char *argv[]){
    // argparse
    int opt;
    int option_index = 0;

    bool help = false;
    std::string dbPath;
    std::string picPath;
    int max_fail = 0;
    int thread = DTHREAD;


    const char *optstring = "hd:o:f:t:";
    const struct option long_options[] ={
        {"help",    no_argument,        NULL,  'h'},
        {"db",      required_argument,  NULL,  'd'},
        {"output",  required_argument,  NULL,  'o'},
        {"max-fail",required_argument,  NULL,  'f'},
        {"threads" ,required_argument,  NULL,  't'}
    };
    while((opt =getopt_long(argc,argv,optstring,long_options,&option_index))!= -1){  
        switch(opt) {
            case 'h':
                help = true;
                break;
            case 'd':
                dbPath = optarg;
                break;
            case 'o':
                picPath = optarg;
                break;
            case 'f':
                max_fail = atoi(optarg);
                break;
            case 't':
                thread = atoi(optarg);
                break;
            default:
                help = true;
       }
    } 

    if(help || (dbPath.empty()||picPath.empty())){
        usage();
        return 0;
    }
    // end argparse
    
    dbquery query=dbquery(dbPath.c_str());
    if(!query.isOK){
        std::cerr<<"fail to open database." <<std::endl;
        return 0;
    }
    
    omp_set_num_threads(thread);
    
    volatile int fail=0;
    std::vector<int32_t> cids;
    while(true){
        int32_t cid=query.nextID();
        if (cid>=0) cids.push_back(cid);
        else break;
    }

    #pragma omp parallel for shared(fail)
    for (int i=0;(i< cids.size()) ;i++){
        if (max_fail > 0 && fail >= max_fail) continue;
        int32_t cid=cids[i];
        if (picDL(cid,picPath) < 0){
            std::cerr << "download " << cid << "fail" << std::endl;
            fail +=1 ;
        }
    } 

    return 0;
}