#pragma once

#include <iostream>
#include <vector>
#include <map>

#define mapOfFiles  std::map<std::string, std::vector<std::string>>
#define strVec      std::vector<std::string>

class zParser;
class zParserSettings;

// Used as a struct, with default value (std++98 restriction)
class zParserSettings {
public:
    zParserSettings();
    std::string         CommentChars;
    bool                removeArgs;
    bool                ignoreComments;
    bool                skipEmptyLines;
    bool                returnEmptyStringOnErr;
    bool                removeLeadingWhitespace;
    bool                removeTrailingWhitespace;
    zParserSettings&    operator=(const zParserSettings& other);
    bool                operator==(const zParserSettings& other);
};

// Parser Class
class zParser {
public:
    zParser();
    std::string                 ParseFile(const char* filename, const char* argname, const bool hotReload = false);
    zParserSettings             Settings;
private:
    mapOfFiles                  _T;
    zParserSettings             _lastSettings;
    bool                        _doesFileExist(const char* filename);
    bool                        _doesHaveRight(const char* filename);
    void                        _trimWhiteSpace(size_t& index, std::string& str);
    strVec                      _readFile(const char* filename);
};
