# Compiler - Lexical Analyzer

This project implements a **lexical analyzer (scanner)** as part of a Compiler Construction course.

The scanner reads source code from a file and converts it into a sequence of tokens such as identifiers, keywords, literals, and operators.

---

## 🚀 Overview

The system processes input step by step:

* Reads characters using a custom file handler
* Skips whitespace and comments
* Identifies tokens (identifiers, numbers, strings, etc.)
* Matches keywords and operators
* Returns structured token objects

---

## ⚙️ Features

* Custom file reader with line and character tracking (`FileDescriptor`)
* Token generation using a structured `Token` class
* Recognition of:

  * Identifiers
  * Integers and floating-point numbers
  * Strings
  * Keywords
  * Operators and symbols
* Skipping whitespace and comments
* Basic error handling for invalid tokens

---

## 🧠 Supported Keywords

```
program, var, constant, integer, bool, string, float,
true, false, if, then, else, while, do, read, write,
for, from, to, by
```

---

## ➕ Supported Operators & Symbols

```
+  -  *  /
:=  =  !=
<  <=  >  >=
( ) [ ] { }
; , . :
```

---

## 🛠️ Tech Stack

* C++
* Flex (for alternative scanner implementation)
* GCC / g++

---

## ⚙️ How to Run

### Compile

```bash
g++ scanner/FileDescriptor.cpp scanner/Scanner.cpp test/test_Scanner.cpp -o scanner
```

### Run

```bash
./scanner
```

---

## 📁 Project Structure

```
Compiler/
│
├── include/
│   ├── FileDescriptor.h
│   ├── Scanner.h
│   └── Token.h
│
├── scanner/
│   ├── FileDescriptor.cpp
│   ├── Scanner.cpp
│   └── flex scanner/
│       ├── flex_scanner.l
│       └── lex.yy.c
│
├── test/
│   ├── test_Scanner.cpp
│   ├── test_fileDescriptor.cpp
│   ├── test_flex_scanner.cpp
│   ├── test_input.comp
│   └── test_scanner.txt
│
├── output/
├── compiler_steps.md
```

---

## 🧪 Testing

The project includes multiple test files to verify:

* File reading functionality
* Token generation
* Flex-based scanner behavior

---

## ⚠️ Notes

* The `output/` folder contains compiled files and can be ignored.
* Some files (like `lex.yy.c`) are generated automatically by Flex.
* This project focuses only on the **lexical analysis phase** of a compiler.

---

## 👥 Contributors

* - @nebal24
  - @ahmaddaghlas55
