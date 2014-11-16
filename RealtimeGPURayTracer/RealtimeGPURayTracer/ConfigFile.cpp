#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>

#include "ConfigFile.h"
#include "error.h"

using namespace std;

CConfigFile::CConfigFile(void)
{
}

CConfigFile::~CConfigFile(void)
{
}

void CConfigFile::GetNewLine(std::ifstream &inFile, std::string &line)
{
	getline(inFile, line);
	line.erase(line.begin(), find_if_not(line.begin(), line.end(), std::isspace));
	line.erase(find_if_not(line.rbegin(), line.rend(), std::isspace).base(), line.end());
}

int CConfigFile::ParseConfigFile(const std::string &filename)
{
	ifstream inFile(filename, ios_base::in);
	if (!inFile) {
		cerr << "config file " << filename << " no found" << endl;
		return GET_ERROR_RETURN(ET_CONFIG_FILE_NO_FOUND);
	}

	int lineNumber = 0;
	while (!inFile.eof()) {
		string line;

		GetNewLine(inFile, line);
		lineNumber++;

		if (line.empty() || line[0] == '#')
			continue;

		if (line[0] == '@') {
			string key = line.substr(1);

			if (inFile.eof()) {
				cerr << "config file " << filename << " format error at line " << lineNumber << endl;
				inFile.close();
				return GET_ERROR_RETURN(ET_CONFIG_FILE_FORMAT_ERROR);
			} else {
				GetNewLine(inFile, line);
				lineNumber++;
				configs.insert(make_pair(key, line));
			}
		}
	}

	inFile.close();

	return ET_SUCCESS;
}

string CConfigFile::operator[] (const string &key)
{
	auto iter = configs.find(key);
	if (iter != configs.end()) {
		return std::move(iter->second);
	} else {
		return "";
	}
}
