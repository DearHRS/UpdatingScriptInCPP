#include "Other.h"


Other::ProgrammesToUpdate::ProgrammesToUpdate(std::wstring name, std::wstring id, std::wstring fromVersion, std::wstring toVersion) {
	this->name = name;
	this->id = id;
	this->fromVersion = fromVersion;
	this->toVersion = toVersion;
}


bool Other::Funx::IsNumeric(std::wstring givenWstring){
    unsigned int commaPointCounter = 0;                                         //used for counting amount of commas met during std::wstring inspection

    //if easily detectable incorrect format
    if (givenWstring.size() == 0 || givenWstring[0] == L'.' || givenWstring[givenWstring.size() - 1] == L'.' || givenWstring[0] == L',' || givenWstring[givenWstring.size() - 1] == L',') {
        return false;
    }

    //in-depth analysis of given string
    for (unsigned int i = 0; i < givenWstring.size(); i++) {
        switch (givenWstring[i]) {
        //checking if minus symbol is met
        case '-':
            //allowed position for minus symbol
            if (i == 0) {
                continue;
            }
            return false;

        //characters allowed regardless of their postions
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            continue;

        //character allowed but can not be present more than once
        case '.':
        case ',':
            commaPointCounter++;
            if (commaPointCounter > 1) {
                return false;
            }
            continue;

        //not allowed characters
        default:
            return false;
        }
    }

    //std::wstring passes as numeric
    return true;
}


void Other::Funx::BreakSentenceIntoWords(std::wstring inputSentance, std::vector<std::wstring>& outputArray){
    std::wstring tempWstring;                   //stores current word being processed

    //going over every wchar in wstring
    for (unsigned int i = 0; i < inputSentance.size(); i++) {
        //if encountered space then inputting formed word into given std::vector<std::wstring>
        if (inputSentance[i] == L' ') {
            if (tempWstring != L"") {
                outputArray.push_back(tempWstring);
                tempWstring = L"";
            }            
        }

        //word hasn't formed yet, adding wchar to tempWstring
        else {
            tempWstring += inputSentance[i];
        }
    }

    //adding last word if last wchar was not space (previous for loop would have skipped this part)
    if (inputSentance[inputSentance.size() - 1] != L' ') {
        outputArray.push_back(tempWstring);
    }
}


void Other::Funx::GetTextFileContents(std::wstring filename, std::vector<std::wstring>& outputArray){
    std::wfstream readFromFile(L"ReadFromReports\\" + filename + L".txt");      //loading file into memory from ReadFromReports folder
    std::wstring tempWstring;                                                                                   //used to create a line from file being read
    wchar_t wcharFromFile;                                                                                      //used to load a single symbol from file

    //pulling 1 symbol at a time from file
    while (readFromFile >> std::noskipws >> wcharFromFile) {
        //if symbol is newline then upload it to vector and restart making line
        if (wcharFromFile == L'\n') {
            outputArray.push_back(tempWstring);

            tempWstring = L"";
        }
        //any other symbol used to form line
        else {
            tempWstring += wcharFromFile;
        }
    }

    //last line doesn't end with \n character, so catching it here
    outputArray.push_back(tempWstring);

    readFromFile.close();                                                                                       //unloading file from memory
}


std::wstring Other::Funx::GetCHCP(){
    std::wfstream readFromFile("ReadFromReports\\chcpData.txt");        //loading file generated by WindowCommands::GetCHCP() into memory
    std::wstring resultWstring;                                                                     //std::wstring used for getting file contents and later returned with correct result

    //getting last element of result
    while (readFromFile >> resultWstring) {
        //unnecessary if statement
        if (Other::Funx::IsNumeric(resultWstring)) {
            break;
        }
    }

    readFromFile.close();                                                                           //unloading file from memory
    return resultWstring;
}


bool Other::Funx::GetProcessID(std::string& processID){
    std::vector<std::wstring> fileData;         //used to store notepads data

    //loading files into memory
    Other::Funx::GetTextFileContents(L"tasklist", fileData);

    //checking if txt file has any contents
    if (fileData.size() > 1) {
        //breaking gotten sentance into only words
        Other::Funx::BreakSentenceIntoWords(fileData[0], fileData);
        
        //fetching process id
        processID = std::string(fileData[2].begin(), fileData[2].end());

        return true;
    }

    //if txt file has no contents
    return false;
}


void Other::Funx::GetToUpdateList(std::vector<Other::ProgrammesToUpdate>& updateList, Other::threadBoolResult& isThereProgrammes){
    std::vector<std::wstring> fileData;                                                 //used to store file data from txt file

    Other::Funx::GetTextFileContents(L"updateReport", fileData);    //loading file data into memory

    //if nothing found
    if (fileData.size() == 1) {
        isThereProgrammes = Other::threadBoolResult::False;
    }
    //deciphering txt file
    else {
        for (unsigned int i = 0; i < fileData.size() - 1; i++) {
            std::vector<std::wstring> brokenLine;                                               //used to break fetched data in fileData vector
            std::wstring name = L"";                                                                    //used to form name from fetched data in fileData vector
            
            Other::Funx::BreakSentenceIntoWords(fileData[i], brokenLine);    //breaking sentance from fileData vector

            //forming name
            for (unsigned int j = 0; j < brokenLine.size() - 4; j++) {
                name += brokenLine[j] + L" ";
            }

            updateList.push_back(Other::ProgrammesToUpdate(name, brokenLine[brokenLine.size() - 4], brokenLine[brokenLine.size() - 3], brokenLine[brokenLine.size() - 2]));
        }

        isThereProgrammes = Other::threadBoolResult::True;
    }
}

