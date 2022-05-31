#include "utility.h"

void Help()
{
    cout << "TO INPUT ARGUMENTS THROUGH COMMAND LINE, ONE MUST FOLLOW THE SPECIFIC RULES: " << endl
         << endl;

    cout << "ARGUMENTS:" << endl;
    cout << "\t-> 1st argument is always supposed to be thename of the log file" << endl;
    cout << "\t-> The following arguments will contain the operations" << endl;
    cout << "\t-> Use of \"AND\" is necessary in case of multi-operation problem" << endl;
    cout << "Example 1: filename.txt Operation1 AND Operation2" << endl
         << endl;

    cout << "OPERATIONS: " << endl;
    cout << "There are two operations supported by the system: " << endl;
    cout << "\t1. search" << endl;
    cout << "\t2. sequence" << endl;

    cout << "NOTE: the above words are keywords used for performing that operation, and are case-sensitive" << endl
         << endl;

    cout << "SEARCH: " << endl;
    cout << "Keyword: search" << endl;
    cout << "Syntax: search [-i] [-r] parameterType dataValue" << endl;
    cout << "Here, parameterType is considered for searching of logs" << endl;
    cout << "and, dataValue is the value passed which is to be searched" << endl;
    cout << "-i will search for messages by ignoring the case" << endl;
    cout << "-r will search for messages by treating the input as a regex" << endl;
    cout << "Note that -i and -r are only applicable to parameterType message" << endl;
    cout << "Example: search job_id 2" << endl << endl;

    cout << "SEQUENCE: " << endl;
    cout << "Keyword: sequence" << endl;
    cout << "Syntax: sequence parameterType" << endl;
    cout << "Here, parameterType is considered for sorting the logs" << endl;
    cout << "Example: sequence job_id" << endl << endl;

    cout << "Few syntaxes including both search and sequence operation: " << endl;
    cout << "\tfilename.txt search message \"SOME_MESSAGE\" AND sequence job_id" << endl;
    cout << "\tfilename.txt sequence message AND search process_id 13348" << endl;
    cout << "\tfilename.txt sequence message" << endl << endl;

    cout << "PARAMETER-TYPE: " << endl;

    cout << "parameterType for \"search\": " << endl;
    cout << "\tprocess_id" << endl;
    cout << "\tthread_id" << endl;
    cout << "\tdate" << endl;
    cout << "\ttime" << endl;
    cout << "\tdate range" << endl;
    cout << "\ttime range" << endl;
    cout << "\tjob_id" << endl;
    cout << "\tmessage" << endl << endl;

    cout << "parameterType for \"sequence\": " << endl;
    cout << "\tprocess_id" << endl;
    cout << "\tthread_id" << endl;
    cout << "\tdate" << endl;
    cout << "\ttime" << endl;
    cout << "\tjob_id" << endl;
    cout << "\tmessage" << endl << endl;

    cout << "NOTE: all the parameters are case-sensitive, treat them as a keyword" << endl;

}

/*
arguments like:
search filename paramemeterType searchValue
sequence filename parameterType


for search, parameter type includes(case-sesnitive):
1. process_id
2. job_id
3. thread_id
4. date
5. date range
6. time
7. time range
8. message

for sequence, parameter type includes(case-sensitive):
1. process_id
2. job_id
3. thread_id
4. date
5. time
6. message

*/
