#include "utility.h"
#include <string.h>

bool compare(const Operation *a, const Operation *b);

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        cout << "No arguments passed!!" << endl;
        char choice;
        cout << "Do you wish to open the \"HELP\" manual? (y/n)" << endl;
        cout << "\nYou can also put the command line as \"./executable HELP\"" << endl;
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            Help();
        }

        exit(0);
    }

    if(!strcmp(argv[1], "HELP"))
    {
        Help();
        exit(1);
    }
    string filename = argv[1];

    vector<Operation *> operations;
    int sequenceCount = 0;

    for(int i = 2; i <= argc;)
    {
        if(!strcmp(argv[i], "search"))
        {
            bool ignoreCase = false;
            bool useRegex = false;
            int idxParam = i+1;

            while(idxParam < argc)
            {
                if(!strcmp(argv[idxParam], "-i"))
                {
                    ignoreCase = true;
                    idxParam++;
                    continue;
                }
                if(!strcmp(argv[idxParam], "-r"))
                {
                    useRegex = true;
                    idxParam++;
                    continue;
                }
                break;
            }
            map<string, string> tempParameters;
            for(; idxParam < argc && (strcmp(argv[idxParam], "AND")); idxParam += 2 )
            {
                tempParameters.insert(make_pair(argv[idxParam], argv[idxParam + 1]));
            }

            auto operation = new Operation(argv[i], tempParameters);
            operation->useRegex = useRegex;
            operation->ignoreCase = ignoreCase;
            operations.push_back(operation);
            i = idxParam+1;

        }
        else if(!strcmp(argv[i], "sequence"))
        {
            map<string, string> tempParameters;
            tempParameters.insert(make_pair(argv[i + 1], ""));
            operations.push_back(new Operation(argv[i], tempParameters));
            i += (tempParameters.size()) + 2;

            if(++sequenceCount > 1)
            {
                cout << "Sequence operation cannot be performed more than once" << endl;
                cout << "Try looking for HELP" << endl;
                exit(1);
            }
        }
        else
        {
            cout << "Invalid operation!" << endl;
            cout << "You might wanna check out \"HELP\"" << endl;
            exit(1);
        }
    }

    sort(operations.begin(), operations.end(), compare);

    ifstream myFile(filename);
    if(!myFile.good())
    {
        cout << "Couldn't open file!" << endl;
        exit(1);
    }

    stringstream buffer;
    buffer << myFile.rdbuf();
    myFile.close();

    string inputStr = buffer.str();
    string result;
    int searchIndex = 0;
    if(operations[0]->operationType == "sequence")
    {
        result = Operation::Sequence(inputStr, operations[0]);

        delete operations[0];
        searchIndex = 1;
        inputStr = result;
    }




    for(; searchIndex < operations.size(); searchIndex++)
    {
        string result = Operation::Search(inputStr, operations[searchIndex]);
        cout << endl;
        cout << "RESULTTTTTTTT: " << endl;
        cout << result << endl;

        delete operations[searchIndex];
    }

}

bool compare(const Operation *a, const Operation *b)
{
    return a->operationType > b->operationType;
}
