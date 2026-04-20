#include <iostream>
#include<cstring>
#include "../include/FileDescriptor.h"

//constructor with file name
FileDescriptor ::FileDescriptor(const char *FileName):fp(nullptr),line_number(1),char_number(0),flag(UNSET),
buf_size(BUFFER_SIZE),buffer(nullptr),file(nullptr),unget_char('\0'),eof_reached(false)
{
if(FileName == nullptr || strlen(FileName)==0){
    std::cerr <<"Error: Invalid File Name Provied"<<std::endl;
    return;
}

//allocate & copy filename
file = new char [strlen(FileName)+1];
strcpy(file,FileName);


//initail buffer 
buffer = new char[buf_size]; // buf_size : current buffer size 
ClearBuffer();

//Open file
fp = fopen(FileName,"r"); // open file to read only 
if (fp == nullptr){
    std ::cerr<<"Error: Can't Open file"<<FileName<<"."<<std::endl;
        perror(">"); // like e.getMessage() in java
    }
}
//constructor
FileDescriptor::FileDescriptor():fp(nullptr),line_number(1),char_number(0),flag(UNSET),
buf_size(BUFFER_SIZE),buffer(nullptr),file(nullptr),unget_char('\0'),eof_reached(false)
{
    buffer = new char [buf_size];
    ClearBuffer();
}
//Destrcutor
FileDescriptor::~FileDescriptor(){
    Close();
    delete[] buffer;
    delete[] file;
}

//clear buffer
void FileDescriptor::ClearBuffer(){
if(buffer != nullptr){
    memset(buffer,0,buf_size);
    }
}
void FileDescriptor::ResizeBuffer(){
    size_t new_size = buf_size*2; // dup buffer
    char *new_buffer = new char[new_size];

    //copy content
    memcpy(new_buffer,buffer,buf_size);
    memset(new_buffer+buf_size,0,buf_size);//zero out new space


    //replace old
    delete[]buffer;
    buffer = new_buffer;
    buf_size = new_size;

    #ifdef DEBUG 
    std::cerr<<"Debug: Buffer resized to"<<buf_size
    <<"bytes at line"<<line_number<<std::endl;
    #endif
}

//read a full line and save to buffer (with dynamic resizing)
void FileDescriptor::ReadLine()
{
    if(fp == nullptr ||eof_reached){
        return;
    }

    ClearBuffer();
    char_number =0;
    size_t pos = 0;
    int c ; 

    while ((c= fgetc(fp))!=EOF && c != '\n'){
   
        if (c == '\r') continue;///////////
        if (pos >= buf_size -2){
        ResizeBuffer();
    }
    buffer[pos++] =(char)c;
    }

        // add new line 
    if (c == '\n'){
        if (pos >= buf_size-2){
            ResizeBuffer();
        }
        buffer[pos++]='\n';
   }
   else if (c == EOF)
   {
    eof_reached = true;
   }
   buffer[pos]='\0';
}






//get the next char
char FileDescriptor::GetChar(){
    

    if (fp == nullptr){
        return EOF;
    }

    //Handle ungotten chars
    if(flag == SET){
        flag = UNSET;
        char_number++;
        return unget_char;
    }

    // If we're at the start or end of buffer, read new line
    if(char_number == 0 || buffer[char_number] == '\0') {
        
        // If we just finished a line (last char was '\n'), increment line number
        if (char_number > 0 && buffer[char_number - 1] == '\n') {
            line_number++;
        }
        
        // ✅ ADD THIS CHECK - If already at EOF, don't try to read more!
        if (eof_reached) {
            return EOF;
        }
        
        ReadLine();
        char_number = 0;

        // Check if we've reached EOF with no data
        if (buffer[0] == '\0' && eof_reached) {
            return EOF;
        }
    }
    
    char c = buffer[char_number];
    char_number++;
    
    return c;
    
}


//put a char back 
void FileDescriptor::UngetChar(char c)
{
if(char_number >0)
    {
    char_number--;
    unget_char = c;
    flag = SET;
    }
}


//peek at next char
char FileDescriptor::PeekChar(int offset) const {
    if (!buffer) return EOF;
    int pos = char_number + offset;
    if (pos < (int)strlen(buffer))
        return buffer[pos];
    return EOF;
}




//retun current line buffer
char *FileDescriptor::GetCurrLine()const 
{
    return buffer;
}

//rettun line number
int FileDescriptor::GetLineNum()const
{
    return line_number;
}

// Get current character position
int FileDescriptor::GetCharNum()const
{
    return char_number;   
}

// Get filename
char *FileDescriptor::GetFileName() const {
    return file;
}

// Check if file is open
bool FileDescriptor::IsOpen() const {
    return fp != nullptr;
}

// Check if EOF reached
bool FileDescriptor::IsEOF() const {
  return eof_reached && (buffer[char_number] == '\0' || char_number >= (int)strlen(buffer));
}


// Close the file
void FileDescriptor::Close() {
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
    }
}

// Report error at current position
 void FileDescriptor::ReportError(const char *msg) const
{
    ReportError(msg,line_number,char_number);
}


//report error at a specific pos
void FileDescriptor::ReportError(const char *msg,int line,int col)const
{
    std::cerr<<"\n=====ERROR===="<<std::endl;

    if(file != nullptr){
        std::cerr<<"File:"<<file<<std::endl;
    }
    
    std::cerr << "Line: " << line << ", Column: " << col << std::endl;
    std::cerr << "Message: " << msg << std::endl;

    if(buffer != nullptr && buffer[0] !='\0')
    {
        std::cerr << "\nContext:" << std::endl;
        std::cerr << "  " << buffer;
    
    // Don't add extra newline if buffer already ends with one
        if (buffer[strlen(buffer) - 1] != '\n') {
            std::cerr << std::endl;
        }
        
        // Print error pointer
        std::cerr << "  ";
        for (int i = 0; i < col - 1 && i < (int)strlen(buffer); i++) {
            if (buffer[i] == '\t') {
                std::cerr << '\t';
            } else {
                std::cerr << ' ';
            }
        }
        std::cerr << "^" << std::endl;
    }
    std::cerr << "=============" << std::endl;
}


// Report warning
void FileDescriptor::ReportWarning(const char *msg) const {
    std::cerr << "\n--- WARNING ---" << std::endl;
    
    if (file != nullptr) {
        std::cerr << "File: " << file << std::endl;
    }
    
    std::cerr << "Line: " << line_number << ", Column: " << char_number << std::endl;
    std::cerr << "Message: " << msg << std::endl;
    std::cerr << "---------------" << std::endl;
}
