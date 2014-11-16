#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

#include <string>
#include <map>
#include <fstream>


class CConfigFile
{
protected:
	std::map<std::string, std::string> configs;
protected:
	void GetNewLine(std::ifstream &inFile, std::string &line);
public:
	CConfigFile(void);
	~CConfigFile(void);

	int ParseConfigFile(const std::string &filename);
	std::string operator[] (const std::string &key);

};

#endif

