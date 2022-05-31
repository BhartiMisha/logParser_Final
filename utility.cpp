#include "utility.h"
#include <regex>
// int indexx;

bool logLine::isValid()
{
    return logVal.size() == 6;
}

logLine::logLine(const string &line)
{
    vector<string> result;
    string temp = "";

    int i = 0, count = 0;
    while (line[i] != '\0')
    {
        if (line[i] == ' ' && temp != "")
        {
            result.push_back(temp);
            temp = "";
            i++;
            count++;
        }
        else if (line[i] != ' ')
        {
            temp += line[i++];
        }
        else
        {
            i++;
        }
    }
    if (temp != "")
        result.push_back(temp);

    temp = "";
    for (int i = 5; i < result.size(); i++)
    {
        temp += result[i] + " ";
    }
    for (int i = result.size() - 1; i >= 5; i--)
    {
        result.pop_back();
    }
    temp = temp.substr(0, temp.size() - 1);
    result.push_back(temp);
    this->logVal = result;
}

enum indexVal getIndex(const string &parameterType)
{
    enum indexVal column;
    if (parameterType == "process_id")
    {
        column = Process_id;
    }
    else if (parameterType == "thread_id")
    {
        column = Thread_id;
    }
    else if (parameterType == "date range" || parameterType == "date") // date range: mm/dd-mm/dd
    {
        column = Date;
    }
    else if (parameterType == "time range" || parameterType == "time") // time range: hh:mm:ss-hh:mm:ss
    {
        column = Time;
    }
    else if (parameterType == "job_id")
    {
        column = Job_id;
    }
    else if (parameterType == "message")
    {
        column = Message;
    }
    return column;
}

string Operation::Search(const string &inputStr, Operation *operate)
{
    string resultFinal;

    map<string, string>::iterator itr;

    stringstream localStream;
    localStream << inputStr;

    string leftDate, rightDate;
    string leftTime, rightTime;
    int count = 0;
    for (itr = operate->parameters.begin(); itr != operate->parameters.end(); ++itr)
    {
        if (itr->first == "date range")
        {
            leftDate = itr->second.substr(0, 5);
            rightDate = itr->second.substr(6, 5);
            count++;
            continue;
        }
        if (itr->first == "time range")
        {
            leftTime = itr->second.substr(0, 8);
            rightTime = itr->second.substr(9, 8);
            count++;
            continue;
        }

        if (count == 2)
        {
            break;
        }
    }

    tm t1 = {};
    tm t2 = {};

    istringstream s2(leftDate + " " + leftTime);
    istringstream s1(rightDate + " " + rightTime);

    s1 >> std::get_time(&t1, "%m/%d %H:%M:%S");
    s2 >> std::get_time(&t2, "%m/%d %H:%M:%S");

    t1.tm_year=99;
    t2.tm_year=99;
    string newInput = "";
    time_t leftEpochRange = mktime(&t1);
    time_t rightEpochRange = mktime(&t2);
    if (count == 2)
    {
        string line;
        newInput = "";
        while (getline(localStream, line))
        {
            logLine *log;
            if (line.size() == 0)
            {
                continue;
            }
            log = new logLine(line);
            if (!log->isValid())
            {
                continue;
            }

            string currentStringTime = log->logVal[2] + " " + log->logVal[3];
            tm t_current = {};

            istringstream s_current(currentStringTime);

            s_current >> std::get_time(&t_current, "%m/%d %H:%M:%S");
            t_current.tm_year=99;

            time_t currentEpoch = mktime(&t_current);
            if (leftEpochRange >= currentEpoch && rightEpochRange <= currentEpoch)
            {
                newInput += line + "\n";
            }

        }
        operate->parameters.erase("time range");
        operate->parameters.erase("date range");

        localStream = stringstream();
        localStream << newInput;

        resultFinal = newInput;
    }


    for (itr = operate->parameters.begin(); itr != operate->parameters.end(); ++itr)
    {
        cout << itr->first << endl;
        resultFinal = "";
        string line, result;
        enum indexVal column = getIndex(itr->first);
        while (getline(localStream, line))
        {
            logLine *log;
            if (line.size() == 0)
            {
                continue;
            }
            log = new logLine(line);
            if (!log->isValid())
            {
                continue;
            }

            smatch m;
            if (itr->first == "time range")
            {
                string leftRange, rightRange;
                leftRange = itr->second.substr(0, 8);
                rightRange = itr->second.substr(9, 8);

                if (log->logVal[column].compare(leftRange) > 0 && log->logVal[column].compare(rightRange) < 0)
                {
                    result += line + "\n";
                }
            }
            else if (itr->first == "date range")
            {
                string leftRange, rightRange;
                leftRange = itr->second.substr(0, 5);
                rightRange = itr->second.substr(6, 5);

                if (log->logVal[column].compare(leftRange) > 0 && log->logVal[column].compare(rightRange) < 0)
                {
                    result += line + "\n";
                }
            }
            else if (itr->first == "message")
            {
                auto messageField = log->logVal[column];
                auto flags = regex_constants::ECMAScript;
                auto toMatch = itr->second;
                if(operate->useRegex)
                {
                    if(operate->ignoreCase) flags |= regex_constants::icase;
                    if (regex_search(log->logVal[column], m, regex(itr->second, flags)))
                    {
                        result += line + "\n";
                    }
                }
                else
                {
                    if(operate->ignoreCase)
                    {
                        transform(messageField.begin(), messageField.end(), messageField.begin(), ::tolower);
                        transform(toMatch.begin(), toMatch.end(), toMatch.begin(), ::tolower);
                    }
                    if(messageField.find(toMatch) != string::npos)
                    {
                        result += line + "\n";
                    }
                }
            }
            else if (!log->logVal[column].compare(itr->second))
            {
                result += line + "\n";
            }
            else
            {
                // do nothing
            }
        }

        localStream = stringstream();
        localStream << result;
        resultFinal = result;
    }

    return resultFinal;
}

bool compare(const pair<logLine, string> &pairA, const pair<logLine, string> &pairB)
{
    if (pairA.first.logVal[2] == pairB.first.logVal[2])
    {
        return pairA.first.logVal[3] < pairB.first.logVal[3];
    }
    return pairA.first.logVal[2] < pairB.first.logVal[2];
}

string Operation::Sequence(const string &inputStr, Operation *operate)
{
    map<string, string>::iterator seq = operate->parameters.begin();
    enum indexVal column = getIndex(seq->first);

    multimap<string, string> resultant_Log;
    multimap<string, string>::iterator itr;

    stringstream localStream;
    localStream << inputStr;

    string result = "";
    // in case of time
    if (column == 3)
    {
        vector<pair<logLine, string>> logs;
        string line;
        while (getline(localStream, line))
        {
            if (line.size() == 0)
            {
                continue;
            }
            logLine temp(line);
            logs.push_back(make_pair(temp, line));
        }
        sort(logs.begin(), logs.end(), compare);
        vector<pair<logLine, string>>::iterator itr;
        for (itr = logs.begin(); itr != logs.end(); ++itr)
        {
            result += itr->second + "\n";
        }
    }
    else
    {
        string line;
        while (getline(localStream, line))
        {
            if (line.size() == 0)
            {
                continue;
            }
            logLine logs(line);
            resultant_Log.insert(make_pair(logs.logVal[column], line));
        }

        for (itr = resultant_Log.begin(); itr != resultant_Log.end(); ++itr)
        {
            result += itr->second + "\n";
        }
    }

    return result;
}
