#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

int main(){
    
    string data;

    ifstream file("csv_as_text/Turntable1.txt");
    
    while(getline(file, data)){
        file>>data;
    }

    file.close();

    int x = sizeof(string);
    printf("%d\n",x);

    return 0;
}