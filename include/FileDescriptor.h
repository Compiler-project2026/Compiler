#ifndef FD_H
#define FD_H

#include <cstdio>
#include <string>

#define SET 1
#define UNSET 0
#define BUFFER_SIZE 256

class FileDescriptor {
private:
    FILE *fp;
    int line_number;      // Current line number (1-indexed)
    int char_number;      // Current character position in line (0-indexed)
    int flag;             // Flag for unget operation
    size_t buf_size;      // Current buffer size
    char *buffer;         // Line buffer (dynamically resized)
    char *file;           // Filename
    char unget_char;      // Character that was ungotten
    bool eof_reached;     // EOF status
    
    // Private helper methods
    void ResizeBuffer();
    void ReadLine();
    void ClearBuffer();
    
public:
    // Constructors and Destructor
    FileDescriptor(const char *FileName); // user defined constructor
    FileDescriptor(); // Constructors
    ~FileDescriptor(); //Destructor
    
    // Delete copy constructor and assignment operator (prevent copying)
    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;
    
    // File operations
    bool IsOpen() const;
    void Close();
    
    // Character operations
    char GetChar();
    void UngetChar(char c);
    char PeekChar(int offset=0)const;  // Look ahead without consuming
    
    // Line operations
    char *GetCurrLine() const;
    int GetLineNum() const;
    int GetCharNum() const;
    char *GetFileName() const;
    
    // Error reporting
    void ReportError(const char *msg) const;
    void ReportError(const char *msg, int line, int col) const;
    void ReportWarning(const char *msg) const;
    
    // Status
    bool IsEOF() const;
};

#endif // FD_H