#include <iostream>
#include <vector>
#include <fstream>
#include <string>


#include "KDB.hpp"



int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {

        KDB kdb(argv[i]);
        kdb.outputData();
        std::cout << "\n\n";

    }
    return 0;
 }