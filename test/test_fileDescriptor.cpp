#include "../include/FileDescriptor.h"
#include <iostream>
#include <cstring>

/*
to run the test 
linux :
cd path && g++ -std=c++11 scanner/FileDescriptor.cpp test/test_fileDescriptor.cpp -I include -o test_fd && ./test_fd
windows:
cd path && g++ -std=c++11 scanner\FileDescriptor.cpp test\test_fileDescriptor.cpp -I include -o test_fd.exe && test_fd.exe
*/ 




// Test helper
void PrintSeparator() {
    std::cout << "\n" << std::string(60, '=') << "\n" << std::endl;
}

// Test 1: Basic reading
void Test_BasicReading() {
    std::cout << "TEST 1: Basic File Reading" << std::endl;
    PrintSeparator();
    
    // Create test file
    FILE *f = fopen("test1.txt", "w");
    fprintf(f, "Hello World\n");
    fprintf(f, "Second line\n");
    fprintf(f, "Third line");
    fclose(f);
    
    FileDescriptor fd("test1.txt");
    
    if (!fd.IsOpen()) {
        std::cerr << "Failed to open test file!" << std::endl;
        return;
    }
    
    char c;
    while ((c = fd.GetChar()) != EOF) {
        std::cout << c;
    }
    
    std::cout << "\n\nFinal position - Line: " << fd.GetLineNum() 
            << ", Char: " << fd.GetCharNum() << std::endl;
}

// Test 2: Long line (buffer resizing)
void Test_LongLine() {
    std::cout << "TEST 2: Long Line (Buffer Resizing)" << std::endl;
    PrintSeparator();
    
    // Create file with very long line
    FILE *f = fopen("test2.txt", "w");
    for (int i = 0; i < 1000; i++) {
        fprintf(f, "A");
    }
    fprintf(f, "\nShort line\n");
    fclose(f);
    
    FileDescriptor fd("test2.txt");
    
    if (!fd.IsOpen()) {
        std::cerr << "Failed to open test file!" << std::endl;
        return;
    }
    
    int char_count = 0;
    char c;
    while ((c = fd.GetChar()) != EOF) {
        if (c != '\n') char_count++;
        if (fd.GetLineNum() == 1 && c == '\n') {
            std::cout << "First line has : " << char_count << " characters" <<" <<done by GetChar()"<< std::endl;
        }
    }
    
    std::cout << "Successfully read long line!" << std::endl;
}

// Test 3: Error reporting
void Test_ErrorReporting() {
    std::cout << "TEST 3: Error Reporting" << std::endl;
    PrintSeparator();
    
    FILE *f = fopen("test3.txt", "w");
    fprintf(f, "int main() {\n");
    fprintf(f, "    printf(\"Hello\")\n");  // Missing semicolon
    fprintf(f, "    return 0;\n");
    fprintf(f, "}\n");
    fclose(f);
    
    FileDescriptor fd("test3.txt");
    
    char c;
    int line = 0;
    while ((c = fd.GetChar()) != EOF) {
        if (fd.GetLineNum() == 2 && c == ')') {
            fd.ReportError("Expected ';' after statement");
        }
        if (c == '\n') line++;
    }
}

// Test 4: Unget functionality
void Test_Unget() {
    std::cout << "TEST 4: UngetChar and PeekChar" << std::endl;
    PrintSeparator();
    
    FILE *f = fopen("test4.txt", "w");
    fprintf(f, "123 456 789\n");
    fclose(f);
    
    FileDescriptor fd("test4.txt");
    
    std::cout << "Reading with peek:" << std::endl;
    char c;
    while ((c = fd.PeekChar()) != EOF && c != '\n') {
        char actual = fd.GetChar();
        std::cout << "Peeked: '" << c << "', Got: '" << actual << "'" << std::endl;
        
        if (actual == ' ') {
            std::cout << "  -> Space detected!" << std::endl;
        }
    }
}

// Test 5: Multiple lines with tracking
void Test_Tracking() {
    std::cout << "TEST 5: Line and Character Tracking" << std::endl;
    PrintSeparator();
    
    FILE *f = fopen("test5.txt", "w");
    fprintf(f, "Line 1\n");
    fprintf(f, "Line 2 is longer\n");
    fprintf(f, "L3\n");
    fclose(f);
    
    FileDescriptor fd("test5.txt");
    
    char c;
    while ((c = fd.GetChar()) != EOF) {
        if (c == '\n') {
            std::cout << "End of line " << (fd.GetLineNum() - 1) 
                    << " (was at char " << fd.GetCharNum() << ")" << std::endl;
        }
    }
}

// Test 6: Empty file
void Test_EmptyFile() {
    std::cout << "TEST 6: Empty File" << std::endl;
    PrintSeparator();
    
    FILE *f = fopen("test6.txt", "w");
    fclose(f);
    
    FileDescriptor fd("test6.txt");
    
    char c = fd.GetChar();
    if (c == EOF) {
        std::cout << "Correctly detected empty file (EOF)" << std::endl;
    } else {
        std::cout << "ERROR: Expected EOF but got: " << c << std::endl;
    }
}

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         FILE DESCRIPTOR COMPREHENSIVE TEST SUITE           ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    Test_BasicReading();
    PrintSeparator();
    
    Test_LongLine();
    PrintSeparator();
    
    Test_ErrorReporting();
    PrintSeparator();
    
    Test_Unget();
    PrintSeparator();
    
    Test_Tracking();
    PrintSeparator();
    
    Test_EmptyFile();
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                     ALL TESTS COMPLETE                     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    // Cleanup
    remove("test1.txt");
    remove("test2.txt");
    remove("test3.txt");
    remove("test4.txt");
    remove("test5.txt");
    remove("test6.txt");
    
    return 0;
}