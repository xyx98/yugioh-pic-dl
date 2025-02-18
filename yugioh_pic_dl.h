#ifndef YPDL_H
#define YPDL_H

#define BASEURL "https://images.ygoprodeck.com/images/cards_cropped/"
#define MAXDLFAIL 20

#include <iostream>
#include <fstream>

int picDL(int32_t cid,std::string dlpath);

static size_t writeCallback(void* ptr, size_t size, size_t nmemb, std::ofstream* file);


#endif
