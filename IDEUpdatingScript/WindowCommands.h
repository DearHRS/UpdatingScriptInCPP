#pragma once
#include <vector>
#include <string>
#include <thread>
#include "Other.h"

#include <iostream> //remove, only testing

/*
call in thread, these launch exes to fetch data


includes:
	vector
	string
	fstream
	"Other.h"

Contains functions to:

	check internet conectivity
	get ids of programmes to update into a txt file
	update programmes by id and their result into a txt file
*/
class WindowCommands{
public:
	/*
		Generates code page of current system
	*/
	static void GetCHCP(std::wstring& chcp, Other::threadBoolResult& hasFinished);


	/*
	CALL IT AFTER WindowCommands::GETCHCP(), CHCP DATA ISN"T STORED ANYWHERE

	used for changing encoding (generally to 65001)
		
	so reports aren't generated as gibberish due to windows using 
	ANSI encoding for generating files that can't be changed
	*/
	static void SetCHCP(std::string encoding);


	/*
	wrapper function for WindowCommands::GetCHCP and WindowCommands::SetCHCP
	Calls them in correct order of operations to not lose any data
	*/
	static void GetSetCHCP(std::wstring& chcp, std::string encoding, Other::threadBoolResult& hasFinished);


	/*testing required
	pings google.com and yandex.ru to check if any of them reply

	if they do then user has connection and true is returned

	otherwise returns false
	*/
	static void IsConnectedToNet(Other::threadBoolResult& hasConnected);
	

	/*testing required
	uses winget tool to get programmes and ids to update
	*/
	static void GenerateUpdateReport();

	/*testing required
	updates programme using winget
	*/
	static void UpdateProgramme(std::string id);


	/*testing required
	terminates process based of their id
	*/
	static void TerminateProcess(std::string processID);

	
	/*
	checks if you got admin priviledges or not, return true if True
	else False
	*/
	static void HasAdminPrivileges(Other::threadBoolResult& hasPrivleges);


	/*testing required
		finds Uninstall and kills it, returns true

		didn't find, returns false
	*/
	static void HasGuiLaunched(Other::threadBoolResult& hasLaunched);


	/*
	starts a new cmd window and attempts winget upgrade, then result is added to txt file where original cmd is running from
	start cmd.exe /c "winget upgrade > "resettedWinget.txt""
	*/
};