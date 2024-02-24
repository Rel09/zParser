# zParser - Easy-to-Use C++ Parser

zParser is a lightweight C++ parser designed for ease of use. It efficiently stores the content of a file in a map, and multiple calls to the same file won't re-open it unless the hotreload argument is specified. The parser also provides customizable settings through the settings class within the parser object.

## Settings

- **Comment Characters:** Modify the string (or char) considered as comment characters in the file.
- **Remove Arguments:** Control whether to exclude the argument being searched for in the line.
- **Ignore Comments:** Option to remove comments from the string output.
- **Skip Empty Lines:** Choose not to store lines that are empty.
- **Return Empty String on Error:** Decide to return an empty string instead of an error message on error.
- **Remove Whitespace:**
  - Remove leading whitespace.
  - Remove trailing whitespace.

## Usage

To use zParser, instantiate the parser object and configure settings through the settings class. Here's a basic example:

// Config File
![Settings](https://i.imgur.com/VVZcnds.png)

// Output
![Settings](https://i.imgur.com/Cly7Oqp.png)

// Settings
![Settings](https://i.imgur.com/q2tdLdN.png)

// Main Example
![Settings](https://i.imgur.com/jDC37RL.png)

