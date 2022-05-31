#ifndef uitlity_H
#define utility_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <map>
#include <locale>
#include <iomanip>

using namespace std;

class logLine
{
public:
    vector<string> logVal;

    logLine(const string &line);
    bool isValid();
};

class Operation
{
public:
    string operationType;
    map<string, string> parameters;

    static string Search(const string &inputStr, Operation *op);
    static string Sequence(const string &inputStr, Operation *op);

    Operation(const string &operation, map<string, string> &parameter)
    {
        this->operationType = operation;
        this->parameters = parameter;
    }
};

enum indexVal
{
    Process_id,
    Thread_id,
    Date,
    Time,
    Job_id,
    Message
};

void Help();

#endif

/*
regex

( *[a-f0-9]{1,4})
([a-f0-9]{4}| [a-f0-9]{3}|  [a-f0-9]{2}|   [a-f0-9]{1})

(\d){5} ( ){0,3}([a-f0-9]){1,4} \d\d/\d\d \d\d:\d\d:\d\d
logutiltiy search Hello\nworld
hello
world

*/