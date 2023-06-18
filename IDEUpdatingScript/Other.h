#pragma once
#include <fstream>
#include <string>
#include <vector>


namespace Other {
	/*
	contains structs and functions used around the code

	includes:
		fstream
		string
		vector
	*/


	/*
	set it to notFinished by default, threads will reply with True (everything went successfully) or False (otherwise)
	used in threads
	*/
	enum threadBoolResult{notFinished, True, False};


	/*
	predefined programme stages

	-maybe change stage's name to what they do
	*/
	enum ProgrammeStage { 
		stageGetSetCHCP, stageCheckAdminPrivileges, stageCheckNet, stageCheckSourcemsstore, stageSourcemsstoreFailed, 
		stageGenerateUpdateListInitializer, stageGenerateUpdateList, stageUseUpdateList, stageStartUpdatingInitializer, stageStartUpdating,
		stageCloseWindow};


	//used to store contents for programmes to update
	struct ProgrammesToUpdate{
		std::wstring name;								//stores programme's name
		std::wstring id;									//stores programme's id
		std::wstring fromVersion;					//stores programme's current version in system
		std::wstring toVersion;						//stores programme's current version available to update

		ProgrammesToUpdate();
		ProgrammesToUpdate(std::wstring name, std::wstring id, std::wstring fromVersion, std::wstring toVersion);
	};


	class Funx {
	public:
		/*
		returns true if given std::wstring is numeric
		else false
		*/
		static bool IsNumeric(std::wstring givenWstring);


		/*
		breaks sentance into words

		and adds those words into given std::vector<std::wstring>&
		*/
		static void BreakSentenceIntoWords(std::wstring inputSentance, std::vector<std::wstring>& outputArray);


		/*
		currently has bugs with character outside of utf 16 range
		reads from given file name and copys them into given std::vector<std::wstring>&
		*/
		static void GetTextFileContents(std::wstring filename, std::vector<std::wstring>& outputArray);



		/*
		deciphers file and returns system encoding already fetched by WindowCommands::GetCHCP();
		*/
		static std::wstring GetCHCP();


		/*
		deciphers file 
		returns true if process Id was found
		else false
		*/
		static bool GetProcessID(std::string& processID);


		/*
		deciphers file
		returns list of programmes that require updates
		*/
		static void GetToUpdateList(std::vector<Other::ProgrammesToUpdate>& updateList, Other::threadBoolResult& isThereProgrammes);
	};
}