# Simple Computer

Simple Computer is a model of a computer system that can process commands stored in the memory. It provides an implementation of the central processing unit (CPU) and supports two translators: one for the Simple Assembler language and another for the Simple Basic language.

## Introduction

The Simple Computer project aims to create a computer model capable of processing commands stored in the memory. The project includes an implementation of the central processing unit (CPU) and provides two translators for the Simple Assembler and Simple Basic languages.

## Features
- Ability to process commands stored in the memory
- Implementation of the central processing unit (CPU)
- Translators for the Simple Assembler and Simple Basic languages

## Usage

To use the Simple Computer model, you can follow these steps:

Clone the repository:

    git clone <repository-url>
    
Navigate to the project directory:

    cd SimpleComputer
    
Compile the source code:

    make

Run the program:

    ./SimpleComputer

or

    make run

# Translators

The Simple Computer project includes two translators:
one for the Simple Assembler language and another for the Simple Basic language. These translators allow you to write programs in these languages and convert them into the binary format readable by the Simple Computer model.

## Simple Assembler Translator (SAT)

The Simple Assembler translator converts programs written in Simple Assembler language into a binary format that can be executed by the Simple Computer model. To run the translator, use the following command:

    bash

    sat <input-file.sa> <output-file.o>

Where `input-file.sa` is the file containing the Simple Assembler program, and `output-file.o` is the resulting binary file.


## Simple Basic Translator (SBT)

The Simple Basic translator enables programming in the Simple Basic language for the Simple Computer model. It converts Simple Basic programs into Simple Assembler code, which can then be translated into the binary format using the Simple Assembler translator.

To use the Simple Basic translator, follow these steps:
- Write your Simple Basic program in a file (e.g., program.sb).
- Run the translator to generate the Simple Assembler code:
        
        sbt <input-file.sb> <output-file.sa>

Where `input-file.sb` is the file containing the Simple Basic program, and `output-file.sa` is the resulting Simple Assembler code.

Use the Simple Assembler translator to convert the Simple Assembler code into the binary format, as described earlier.

## Conclusion

The Simple Computer project provides a computer model that can process commands stored in the memory. It includes an implementation of the central processing unit (CPU) and offers translators for the Simple Assembler and Simple Basic languages. By using these tools, you can write and execute programs for the Simple Computer model, exploring the world of computer architecture and programming.

## LICENSE

This repository is licensed under the UNLICENSE. Feel free to use the code examples for anything purposes, modify them, and distribute them as needed.
