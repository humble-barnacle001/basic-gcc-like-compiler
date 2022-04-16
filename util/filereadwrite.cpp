#include <bits/stdc++.h>
#include "filereadwrite.h"

using namespace std;

void FileReadWrite::prepareRead(ifstream &file, const char *fileName)
{
    file.open(fileName, ios::in);
    if (!file.is_open())
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
}

void FileReadWrite::prepareWrite(ofstream &file, const char *fileName)
{
    file.open(fileName, ios::out);
    if (!file.is_open())
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
}
