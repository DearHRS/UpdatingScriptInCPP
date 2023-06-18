#include "WindowCommands.h"


void WindowCommands::GetCHCP(std::wstring& chcp, Other::threadBoolResult& hasFinished){
	system("chcp>ReadFromReports\\chcpData.txt");				//generating txt file for Other::Funx::GetCHCP() to fetch data from
	std::this_thread::sleep_for(std::chrono::seconds(1));			//giving 1second for command to generate data

	chcp = Other::Funx::GetCHCP();										//using Other::Funx::GetCHCP() to fetch result
	hasFinished = Other::threadBoolResult::True;
}


void WindowCommands::SetCHCP(std::string encoding){
	std::string command = "chcp " + encoding;						//building command to pass into windows console

	system(command.c_str());													//windows command to achieve task
}

void WindowCommands::GetSetCHCP(std::wstring& chcp, std::string encoding, Other::threadBoolResult& hasFinished){

	std::thread threadGetCHCP(WindowCommands::GetCHCP, std::ref(chcp), std::ref(hasFinished));		//wrapping WindowCommands::GetCHCP() inside a thread
	threadGetCHCP.join();																																//waiting until threadGetCHCP finishes

	WindowCommands::SetCHCP(encoding);																								//setting encoding to utf-8
}


void WindowCommands::IsConnectedToNet(Other::threadBoolResult& hasConnected){
	std::vector<std::wstring> pingReport[2];															//stores ping reports from google and yandex
	
	//pinging google and yandex
	system("start /min cmd /c \"chcp 65001 && ping Yandex.ru > ReadFromReports\\NetConnectionReport2.txt\" & ping Google.com > ReadFromReports\\NetConnectionReport1.txt");
	std::this_thread::sleep_for(std::chrono::seconds(1));		//waiting until txt files are generated

	//getting generated file contents from previous threads
	for (unsigned int i = 0; i < 2; i++) {
		Other::Funx::GetTextFileContents(L"NetConnectionReport" + std::to_wstring(i + 1), pingReport[i]);
	}

	//if not connected to net
	if (pingReport[0].size() == 2 || pingReport[1].size() == 2 || pingReport[0].size() == 1 || pingReport[1].size() == 1) {
		hasConnected = Other::threadBoolResult::False;
	}
	else {
		std::wstring potentialNumericValue[2] = { L"", L"" };
		potentialNumericValue[0] += pingReport[0][8][44];
		potentialNumericValue[1] += pingReport[1][8][44];

		if (Other::Funx::IsNumeric(potentialNumericValue[0]) && Other::Funx::IsNumeric(potentialNumericValue[1])) {

			//if packet loss was not 4 from google or yandex then returns true
			if ((std::stoi(potentialNumericValue[0]) != 4) || (std::stoi(potentialNumericValue[1]) != 4)) {
				hasConnected = Other::threadBoolResult::True;
			}
			//if both google and yanndex packet loss was 4 then returns false
			else {
				hasConnected = Other::threadBoolResult::False;
			}
		}
	}
}


void WindowCommands::IsSourcemsstoreAllowed(Other::threadBoolResult& isAllowed){
	std::string processID;																				//stores process id of launched cmd
	std::vector<std::wstring> resettedWingetData;										//stores data from resettedWinget.txt

	system("start \"IDEUpdatingScript-ToBeTerminated\" /min cmd /c \"chcp 65001 && winget update > \"ReadFromReports\\resettedWinget.txt\"\"");
	std::this_thread::sleep_for(std::chrono::seconds(4));								//waiting till msstore source has been decided or not

	WindowCommands::FindProcess("IDEUpdatingScript-ToBeTerminated", processID);
	WindowCommands::TerminateProcess(processID);
	std::this_thread::sleep_for(std::chrono::seconds(2));								//waiting till txt file is filled

	//getting file contents and checking if msstore is allowed
	Other::Funx::GetTextFileContents(L"resettedWinget", resettedWingetData);
	if (resettedWingetData[resettedWingetData.size() - 1] == L"[Y] Yes  [N] No: ") {
		isAllowed = Other::threadBoolResult::False;
	}
	else {
		isAllowed = Other::threadBoolResult::True;
	}
}


void WindowCommands::GenerateUpdateReport(std::vector<Other::ProgrammesToUpdate>& updateList, Other::threadBoolResult& hasFinished){
	system("winget update --accept-source-agreements | find \"winget\" > ReadFromReports\\updateReport.txt");		//using winget to get report and windows to put that report in txt file
	
	Other::Funx::GetToUpdateList(updateList, hasFinished);
}


bool WindowCommands::FindProcess(std::string processName, std::string& processID){
	std::string command = "tasklist /v | find \"" + processName + "\" > ReadFromReports\\tasklist.txt";
	
	system(command.c_str());
	std::this_thread::sleep_for(std::chrono::seconds(4));
	
	return Other::Funx::GetProcessID(processID);
}


void WindowCommands::TerminateProcess(std::string processID){
	std::string command = "taskkill /pid " + processID + " & cls";	//adding process id to kill command
	system(command.c_str());															//executing command
}


void WindowCommands::HasAdminPrivileges(Other::threadBoolResult& hasPrivileges){
	std::vector<std::wstring> fileContents;								//stores hasAdminPrivileges.txt contents

	/*
	generating output into txt and loading into memory
	*/
	system("net session> ReadFromReports\\adminTest.txt");
	Other::Funx::GetTextFileContents(L"adminTest", fileContents);
	system("cls");

	if (fileContents.size() - 1) {
		hasPrivileges = Other::threadBoolResult::True;
	}
	else {
		hasPrivileges = Other::threadBoolResult::False;
	}
}


void WindowCommands::HasGuiLaunched(Other::threadBoolResult& hasLaunched){
	std::vector<std::wstring> hasGuiLaunchedReport;							//used to store report if Uninstaller is open

	/*
	redundant part
	*/
	//starting and waiting for finder thread to finish
	std::thread uninstallFinder(system, "tasklist /v | find \"Uninstall\" > ReadFromReports\\hasGuiLaunched.txt");
	uninstallFinder.join();

	//loading data into memory
	Other::Funx::GetTextFileContents(L"hasGuiLaunched.txt", hasGuiLaunchedReport);
	/*
	redundant part
	*/
	if (hasGuiLaunchedReport.size()) {
		hasLaunched = Other::threadBoolResult::True;

		Other::Funx::BreakSentenceIntoWords(hasGuiLaunchedReport[0], hasGuiLaunchedReport);
	}
	else{
		hasLaunched = Other::threadBoolResult::False;
	}	
}


void WindowCommands::UpdateProgramme(std::vector<GuiItems::ListItem>& listOfProgrammes, std::wstring& menuDislayText, Other::threadBoolResult& hasFinished) {
	for (unsigned int i = 0; i < listOfProgrammes.size(); i++ ) {
		menuDislayText = L"обновляется программа : " + listOfProgrammes[i].programme.name;

		if (listOfProgrammes[i].IsSelected()) {
			std::vector<std::wstring> updateAttempResult;			//holds result after attempting update
			unsigned int beginReadFrom;										//starting value from where result of update attempt is read
			std::wstring resultToOutput = L"";								//result that is send to gui to display

			//building command to pass into windows console
			std::string command = "winget upgrade --id \"" + std::string(listOfProgrammes[i].programme.id.begin(), listOfProgrammes[i].programme.id.end()) + "\" --silent --accept-package-agreements --force > ReadFromReports\\updatingAttempt.txt";

			system(command.c_str());		//using winget to update and windows to put that report in txt file

			Other::Funx::GetTextFileContents(L"updatingAttempt", updateAttempResult);

			//if result has more than 3 lines then trying to limit result to last 3 lines
			if (updateAttempResult.size() > 3) {
				beginReadFrom = updateAttempResult.size() - 3;
			}
			//if 3 lines or less than outputting what we got
			else {
				beginReadFrom = 0;
			}

			for (beginReadFrom; beginReadFrom < updateAttempResult.size(); beginReadFrom++) {
				resultToOutput = resultToOutput + L" " + updateAttempResult[beginReadFrom] + L"\n";
			}

			listOfProgrammes[i].setTextUpdateResult(resultToOutput);
		}
		else {
			listOfProgrammes[i].setTextUpdateResult(L"Пропущен, не был выбран");
		}
	}

	hasFinished = Other::threadBoolResult::True;
}
