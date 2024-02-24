#include "zParser.h"

#include <fstream>      // open/close
#include <unistd.h>     // access
#include <sys/stat.h>   // stat

#define vecIT       std::vector<std::string>::iterator

// +++++++++++++++ Parser Class +++++++++++++++

// Public
zParser::zParser(){}

std::string                         zParser::ParseFile(const char* filename, const char* argname, const bool hotReload) {

	if (!filename)  { return Settings.returnEmptyStringOnErr ? "" : "[INVALID FILENAME]"; }
    if (!argname)   { return Settings.returnEmptyStringOnErr ? "" : "[INVALID ARGUMENT]"; }

    // If Data that require hot reloading has been changed, hot reload
    if (!(Settings == _lastSettings)){
        _T.clear();
        _lastSettings = Settings;
    }

    // Reopen File
    else if (hotReload) {
        // std++98
        if (_T.find(filename) != _T.end()) {
            _T.erase(filename);
        }
    }

    // if the file has never been read, read it
    if (_T.find(filename) == _T.end()) {

        // Try to read file & gather its content
        try { 
            _T[filename] = _readFile(filename); 
        }
        catch (const int& e) {
            if (!Settings.returnEmptyStringOnErr) {
                switch (e){
                    case 0: return      "[FILE DONT EXIST]";
                    case 1: return      "[NO FILE RIGHT]";
                    default: return     "[CRITICAL ERROR]";
                }
            }
            return "";
        }
        
    }

    // Check there's something to read from the file
    if (_T[filename].empty()) {
        return Settings.returnEmptyStringOnErr ? "" : "[FILE EMPTY]";
    }

    // Loop each lines in the file
    std::string toFind = argname;
    for (vecIT it = _T[filename].begin(); it != _T[filename].end(); it++) {

        std::string str = *it;

        // Attempt to find the Arg on this line
        size_t atIndex = str.find(toFind);
        if (atIndex == std::string::npos) { continue; }

        // Ignore Comment
        if (Settings.ignoreComments) {
            size_t pos = str.find(Settings.CommentChars);
            if (pos != std::string::npos) {
                str.resize(pos);
            }
        }

        // Skip arg
        if (Settings.removeArgs) { atIndex += toFind.size(); }

        // Trim Trailing whitespace
        _trimWhiteSpace(atIndex, str);

        // Found arg but has nothing to return
        if (str[atIndex] == '\0') { return Settings.returnEmptyStringOnErr ? "" : "[FOUND KEY BUT NO VALUE]"; }    

        return &str[atIndex];
    }
    return Settings.returnEmptyStringOnErr ? "" : "[ARG NOT IN FILE]";
}

// Private
bool                                zParser::_doesFileExist(const char* filename) { return (access(filename, F_OK) == 0); }
bool                                zParser::_doesHaveRight(const char* filename) {
    struct stat fileInfo;
    if (stat(filename, &fileInfo) != 0 || (fileInfo.st_mode & S_IRUSR) == 0) { return false; }
    return true;
}
void                                zParser::_trimWhiteSpace(size_t& index, std::string& str) {

    // Skip leading whitespace
    if (Settings.removeLeadingWhitespace) {
        while (index < str.size() && isspace(str[index])) {
            index++;
        }
    }

    // Remove trailer whitesapce
    if (Settings.removeTrailingWhitespace) {
        for (size_t i = str.size() - 1; i > 0 &&  isspace(str[i]); i--) {
            str[i] = '\0';
        }
    }
}
std::vector<std::string>            zParser::_readFile(const char* filename) {
    std::vector<std::string> content;

    if (!_doesFileExist(filename)) {
        throw 0;
    }

    if (!_doesHaveRight(filename)) {
        throw 1;
    }

    // Read stuff
    std::string line;
    std::ifstream file(filename);
    while (std::getline(file, line)) {

        // Skip empty line flag ( whitespace isnt considered empty )
        if (Settings.skipEmptyLines && line.empty()) {
            continue;
        }

        content.push_back(line);
    }

    file.close();
    return content;
}

// +++++++++++++++ Setting Class +++++++++++++++
zParserSettings::zParserSettings() : CommentChars("#"), removeArgs(true), ignoreComments(true), skipEmptyLines(true), returnEmptyStringOnErr(true), removeLeadingWhitespace(true), removeTrailingWhitespace(true) {}
bool                                zParserSettings::operator==(const zParserSettings& other) {

    // Used to trigger hot reloading, so far, only this one is filtered during first read
    if (skipEmptyLines != other.skipEmptyLines)                        return false;

    // if (removeArgs != other.removeArgs)                             return false;
    // if (returnEmptyStringOnErr != other.returnEmptyStringOnErr)     return false;
    // if (removeTrailingWhitespace != other.removeTrailingWhitespace) return false;
    // if (removeLeadingWhitespace != other.removeLeadingWhitespace)   return false;
    // if (ignoreComments != other.ignoreComments)                     return false;
    // if (CommentChars != other.CommentChars)                         return false;
    return true;
}
zParserSettings&                    zParserSettings::operator=(const zParserSettings& other) {
        if (this != &other) {
            removeArgs = other.removeArgs;
            CommentChars = other.CommentChars;
            skipEmptyLines = other.skipEmptyLines;
            ignoreComments = other.ignoreComments;
            returnEmptyStringOnErr = other.returnEmptyStringOnErr;
            removeTrailingWhitespace = other.removeLeadingWhitespace;
            removeLeadingWhitespace = other.removeLeadingWhitespace;
        }
        return *this;
    }