#include "WindowCommands.h"


void WindowCommands::GetCHCP(std::wstring& chcp, Other::threadBoolResult& hasFinished){
	system("chcp>ReadFromReports\\chcpData.txt");				//generating txt file for Other::Funx::GetCHCP() to fetch data from

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
	
	//starting threads to ping google and yandex
	std::thread pingGoogleThread(system, "ping Google.com > ReadFromReports\\NetConnectionReport1.txt");
	std::thread pingYandexThread(system, "ping Yandex.ru > ReadFromReports\\NetConnectionReport2.txt");

	//waiting until google and yandex threads have generated txt file
	pingGoogleThread.join();
	pingYandexThread.join();

	//getting generated file contents from previous threads
	for (unsigned int i = 0; i < 2; i++) {
		Other::Funx::GetTextFileContents(L"NetConnectionReport" + std::to_wstring(i + 1) + L".txt", pingReport[i]);
	}
	
	//checking if pingReport value generated properly
	if (Other::Funx::IsNumeric(L"" + pingReport[0][8][44]) && Other::Funx::IsNumeric(L"" + pingReport[1][8][44])) {
		//if packet loss was not 4 from google or yandex then returns true, if both google and yanndex packet loss was 4 then returns false
		if ((std::stoi(L"" + pingReport[0][8][44]) != 4) || (std::stoi(L"" + pingReport[1][8][44]) != 4)) {
			hasConnected = Other::threadBoolResult::True;
		}
		else {
			hasConnected = Other::threadBoolResult::False;
		}
	}
	//if report failed then not connected
	else {
		std::wcout << "failed to read\n";
		hasConnected = Other::threadBoolResult::False;
	}
}


void WindowCommands::GenerateUpdateReport(){
	system("winget upgrade > ReadFromReports\\updateReport.txt");																//using winget to get report and windows to put that report in txt file
}


void WindowCommands::UpdateProgramme(std::string id){
	std::string command = "winget upgrade " + id + " -h --force > ReadFromReports\\updatingAttempt.txt";	//building command to pass into windows console

	system(command.c_str());																																//using winget to update and windows to put that report in txt file
}


void WindowCommands::TerminateProcess(std::string processID){
	std::string command = "taskkill /pid " + processID;			//adding process id to kill command
	system(command.c_str());													//executing command
}


void WindowCommands::HasAdminPrivileges(Other::threadBoolResult& hasPrivleges){
	std::vector<std::wstring> fileContents;								//stores hasAdminPrivileges.txt contents

	/*
	generating output into txt and loading into memory
	*/
	system("net session> ReadFromReports\\adminTest.txt");
	Other::Funx::GetTextFileContents(L"adminTest", fileContents);
	system("cls");

	if (fileContents.size()) {
		hasPrivleges = Other::threadBoolResult::True;
	}
	else {
		hasPrivleges = Other::threadBoolResult::False;
	}
}


void WindowCommands::HasGuiLaunched(Other::threadBoolResult& hasLaunched){
	std::vector<std::wstring> hasGuiLaunchedReport;							//used to store report if Uninstaller is open

	//starting and waiting for finder thread to finish
	std::thread uninstallFinder(system, "tasklist /v | find \"Uninstall\" > ReadFromReports\\hasGuiLaunched.txt");
	uninstallFinder.join();

	//loading data into memory
	Other::Funx::GetTextFileContents(L"hasGuiLaunched.txt", hasGuiLaunchedReport);

	if (hasGuiLaunchedReport.size()) {
		hasLaunched = Other::threadBoolResult::True;

		Other::Funx::BreakSentenceIntoWords(hasGuiLaunchedReport[0], hasGuiLaunchedReport);

		std::wcout << "hasGuiLaunched found uninstaller running\n";
		for (unsigned int i = 0; i < hasGuiLaunchedReport.size(); i++) {
			std::wcout << ">" << hasGuiLaunchedReport[i] << "<\n";
		}
	}
	else{
		hasLaunched = Other::threadBoolResult::False;
	}	
}
