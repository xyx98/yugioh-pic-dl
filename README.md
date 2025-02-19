# yugioh-pic-dl

download yugioh card's picture from https://ygoprodeck.com/ 

using cards.cdb in ygopro to get card id.

****
## usage:  
    yugioh-pic-dl --db cards.cdb --output pics
        -h,--help                        show help.
        -d,--db <string>                 path of cards.cdb used in ygopro.
        -o,--output <string>             path to save downloaded pics.
        -f,--max-fail <int>              stop after target fails,default is 0, disabled.
        -t,--threads <int>               thread for download,default is 8.
***
## complie:
need sqlite3, curl, openmp 

    meson setup build
    ninja build