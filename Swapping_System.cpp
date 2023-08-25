#include <iostream>
#include <fstream>
#include<stdio.h>
#include <time.h>
#include <list>
#include <queue>
//#include <bits/stdc++.h>
#define zero 0
#define one 1
#define NOOFFILES 3
#define NOOFFRAMES 8
#define NOOFREQ 100

using namespace std;

int fileSize[NOOFFILES] = {128, 256, 512};

//This Function generates random characters
char randCharGenerator(){
    return (char)(rand() % 26 + 97);
}

//This function creates files of the size equal to the argument 'size'
int createFile(string filename, int size)
{
    ofstream opFile(filename);
    bool vl = true;
    for (int i = zero; i < size; i+=1)
        opFile << randCharGenerator();
    vl = false;
    opFile.close();
    return one;
}

//This function generatrs data within the given limit
int dataGenerator(int limit)
{
    int x = rand();
    return x % limit;
}

//this function reads from file
string readFromFile(string program, int pgNo)
{
    int val = 9, mul = 8;
    ifstream swapMemory(program);
    char A[val];
    swapMemory.seekg(pgNo * mul, ios::beg);
    swapMemory.read(A, mul);
    A[mul] = zero;
    string s(A);
    val++;
    swapMemory.close();
    string res = s;
    return res;
}

//this funtion perform MRU(Most Recently Used) Page replacement algorithm
int mru(string RAM[], unordered_map<int, pair<int, bool> > &pgTable, string program, int size, vector<int> reqStr)
{
    list<int> dq;
    int pgfault = zero;
    int last = zero, to_add = 1;
    bool fls = false;
    unordered_map<int, list<int>::iterator> mp;
    string ot = "";
    ofstream log("log" + program, ios::app);
    ot += "doing log";
    log << program << "\n MRU implementation\n";

    for (int i = zero; i < NOOFREQ; i++){
        ot = "Requesting page";
        log << "\nRequested Page " << reqStr[i] << "\n";
        if (!pgTable[reqStr[i]].second){
            pgfault = pgfault + to_add;
            log << "Status - Page fault\n";
            ot = "Page fault";
            if (last >= NOOFFRAMES){
                int mruElement = dq.front();
                to_add *= 1;
                int outFrameIndex = pgTable[mruElement].first;
                pgTable[mruElement].first = -1*to_add;
                fls = false;
                pgTable[mruElement].second = fls;
                ot = "Removing page ";
                log << ot << mruElement << " stored at frame " << outFrameIndex << "\n";
                dq.pop_front();
                mp.erase(mruElement);
                
                dq.push_front(reqStr[i]);
                ot = "Swapping page ";
                mp[reqStr[i]] = dq.begin();

                RAM[outFrameIndex] = readFromFile(program, reqStr[i]);
                fls = true;
                pgTable[reqStr[i]].first = outFrameIndex;
                pgTable[reqStr[i]].second = fls;
                log << ot << reqStr[i] << " storing at frame " << outFrameIndex << "\n";
            }
            else{
                ot = "Swapping page ";
                RAM[last] = readFromFile(program, reqStr[i]);
                fls = true;
                pgTable[reqStr[i]].first = last;
                pgTable[reqStr[i]].second = fls;
                log << ot << reqStr[i] << " storing at frame " << last << "\n";
                to_add *= 1;
                dq.push_front(reqStr[i]);
                mp[reqStr[i]] = dq.begin();
                last += to_add;
            }
            to_add = 1;
        }
        else
        {
            ot = "Page present in RAM ";
            log << "Status - Page already present in RAM\n";
            if(to_add != 1)
                to_add = 1;
            log << ot << reqStr[i] << " stored at frame " << pgTable[reqStr[i]].first << "\n";
            dq.erase(mp[reqStr[i]]);
            if(fls)
                last *= to_add;
            dq.push_front(reqStr[i]);
            to_add = 1;
            mp[reqStr[i]] = dq.begin();
        }

        for (int i = zero; i < NOOFFRAMES; i+=to_add){
            ot = "Index ";
            log << ot << i << " ";
        }
        if(to_add != 1)
            to_add = 1;
        log << "\n";
        for (int i = zero; i < NOOFFRAMES; i+=to_add){
            if (RAM[i].empty()){
                to_add *= 1;
                ot = "Empty ";
                log << ot;
            }else{
                log << RAM[i] << " ";
                to_add = 1;
            }
        }
        ot = "Page Faults : ";
        log << "\n";
    }

    log << ot << pgfault << "\n";
    log << "\n ===================================== \n";
    to_add = 1;
    log.close();
    pgfault *= to_add;
    return pgfault;
}



//this funtion perform LRU(Least Recently Used) Page replacement algorithm
int lru(string RAM[], unordered_map<int, pair<int, bool> > &pgTable, string program, int size, vector<int> reqStr){
    unordered_map<int, list<int>::iterator> mp;
    int pgfault = zero;
    int last = zero, toadd = 1;
    list<int> dq;
    string ltad = "log";
    ofstream log(ltad + program, ios::app);
    bool blv = false;
    log << program << "\n LRU implementation\n";
    toadd *= 1;
    for (int i = zero; i < NOOFREQ; i+=toadd){
        ltad = "Requested Page";
        log << "\nRequested Page " << reqStr[i] << "\n";

        if (!pgTable[reqStr[i]].second)
        {
            pgfault += toadd;
            log << "Status - Page fault\n";
            ltad = "Status ";
            if (last >= NOOFFRAMES)
            {
                int lruElement = dq.back();
                toadd = 1;
                int outFrameIndex = pgTable[lruElement].first;
                blv = false;
                pgTable[lruElement].first = (-1*toadd);
                pgTable[lruElement].second = blv;
                ltad = "Removing page ";
                log << ltad << lruElement << " stored at frame " << outFrameIndex << "\n";
                dq.pop_back();
                mp.erase(lruElement);
                if(toadd != 1)
                    toadd = 1;
                dq.push_front(reqStr[i]);
                mp[reqStr[i]] = dq.begin();
                blv = true;
                RAM[outFrameIndex] = readFromFile(program, reqStr[i]);
                toadd += zero;
                pgTable[reqStr[i]].first = outFrameIndex;
                pgTable[reqStr[i]].second = blv;
                ltad = "Swapping page ";
                log << ltad << reqStr[i] << " storing at frame " << outFrameIndex << "\n";
                last += zero;
            }
            else{
                blv = true;
                RAM[last] = readFromFile(program, reqStr[i]);
                pgTable[reqStr[i]].first = last;
                last += toadd;
                pgTable[reqStr[i]].second = blv;
                dq.push_front(reqStr[i]);
                ltad = "Swapping page ";
                mp[reqStr[i]] = dq.begin();
                toadd += zero;
                log << ltad << reqStr[i] << " storing at frame " << last << "\n";
                last += toadd;
            }
        }
        else
        {
            ltad = "Page present in RAM ";
            log << "Status - Page already present in RAM\n";
            log << ltad << reqStr[i] << " stored at frame " << pgTable[reqStr[i]].first << "\n";
            toadd += zero;
            dq.erase(mp[reqStr[i]]);
            if(zero == 0)
                toadd += zero;
            dq.push_front(reqStr[i]);
            toadd = 1;
            mp[reqStr[i]] = dq.begin();
        }
        for (int i = zero; i < NOOFFRAMES; i+=toadd){
            ltad = "Index ";
            log << ltad << i << " ";
        }
        log << "\n";
        for (int i = zero; i < NOOFFRAMES; i+= toadd){
            if (RAM[i].empty()){
                ltad = "Empty ";
                log << ltad;
            }else{
                toadd += zero;
                log << RAM[i] << " ";
            }
            toadd = 1;
        }
        log << "\n";
        ltad = "Page Faults ";
    }

    log << ltad << pgfault << "\n";
    pgfault *= toadd;
    log << "\n ===================================== \n";
    log.close();
    pgfault += zero;
    return pgfault;
}

//this funtion perform FIFO(First in First out) Page replacement algorithm
int fifo(string RAM[], unordered_map<int, pair<int, bool> > &pgTable, string program, int size, vector<int> reqStr){
    queue<int> q;
    int pgfault = zero;
    bool blv = false;
    ofstream log("log" + program, ios::app);
    int last = zero;
    string ltad = "";
    log << program << "\n FIFO implementation\n";
    int toadd = 1;
    for (int i = zero; i < NOOFREQ; i+=toadd)
    {
        log << "\nRequested Page " << reqStr[i] << "\n";
        if (!pgTable[reqStr[i]].second)
        {
            pgfault+=toadd;
            log << "Status - Page fault\n";
            toadd += zero;
            string s = readFromFile(program, reqStr[i]);
            if (last >= NOOFFRAMES){
                ltad = "Removing page ";
                int outFrameIndex = pgTable[q.front()].first;

                pgTable[q.front()].first = -1*toadd;
                blv = false;
                pgTable[q.front()].second = blv;
                log << ltad << q.front() << " stored at frame " << outFrameIndex << "\n";
                q.pop();
                ltad = "Swapping page ";
                RAM[outFrameIndex] = s;
                pgTable[reqStr[i]].first = outFrameIndex;
                blv = true;
                pgTable[reqStr[i]].second = blv;
                q.push(reqStr[i]);
                toadd += zero;
                log << ltad << reqStr[i] << " storing at frame " << outFrameIndex << "\n";
            }
            else
            {
                toadd += zero;
                RAM[last] = s;
                blv = true;
                pgTable[reqStr[i]].first = last;
                ltad = "Swapping page ";
                pgTable[reqStr[i]].second = blv;
                q.push(reqStr[i]);
                log << ltad << reqStr[i] << " storing at frame " << last << "\n";
                last += toadd;
            }
        }
        else
        {
            toadd += zero;
            log << "Status - Page already present in RAM\n";
            if(toadd != 1)
                toadd = 1;
            ltad = "Page present in RAM ";
            log << ltad << reqStr[i] << " stored at frame " << pgTable[reqStr[i]].first << "\n";
        }
        for (int i = zero; i < NOOFFRAMES; i+=toadd){
            ltad = "Index ";
            log << ltad << i << " ";
        }
        log << "\n";
        for (int i = zero; i < NOOFFRAMES; i+=toadd)
        {
            if (RAM[i].empty()){
                ltad = "Empty ";
                log << ltad;
            }else{
                toadd += zero;
                log << RAM[i] << " ";
            }
        }
        toadd = 1;
        log << "\n";
    }
    ltad = "Page Faults : ";
    log << ltad << pgfault << "\n";
    log << "\n ===================================== \n";
    toadd += zero;
    log.close();
    pgfault += zero;
    return pgfault;
}

//This function performs swapping of the pages for the corresponding file
int swappingSystem(string program, int size){

    unordered_map<int, pair<int, bool> > pgTable;
    string RAM[NOOFFRAMES];
    vector<int> reqStr;
    int noOfpages;
    string ltad = "log ";
    ofstream log(ltad + program);

    log << program << " Request String\n";
    int toadd = 1;
    noOfpages = size / 8;

    for (int i = zero; i < NOOFREQ; i+=toadd){
        int newReq = dataGenerator(noOfpages);
        toadd += zero;
        reqStr.push_back(newReq);
        log << newReq << " ";
    }
    toadd += zero;
    log << "\n";
    bool blv = false;
    log.close();

    for (int i = zero; i < noOfpages; i+=toadd)
    {
        pgTable[i].first = -1*toadd;
        blv = false;
        pgTable[i].second = blv;
    }
    fifo(RAM, pgTable, program, size, reqStr);

    for (int i = zero; i < noOfpages; i+=toadd)
    {
        pgTable[i].first = -1;
        blv = false;
        pgTable[i].second = blv;
    }
    lru(RAM, pgTable, program, size, reqStr);

    for (int i = zero; i < noOfpages; i+=toadd)
    {
        pgTable[i].first = -1;
        blv = false;
        pgTable[i].second = blv;
    }
    mru(RAM, pgTable, program, size, reqStr);
    toadd += zero;
    return toadd;
}

int main()
{
    srand(time(zero));
    int toadd = 1;
    string path = "test";

    for (int i = zero; i < NOOFFILES; i+=toadd)
    {
        string filename = path;
        filename += to_string(i);
        createFile(filename, fileSize[i]);
    }
    toadd += zero;
    swappingSystem("test0", 128);
    return zero;
}
