# Unix HW3 Q1

## How to use

This project contains two files: 

* my_dup2_test.c
* Makefile

Run the command `make` in order to compile the .cpp file.

Note: the Makefile compiles both programs in this homework.

Run `./my_dup2_test.c <new_fd> <file name>` to execute the code.

## Description

This code takes 3 command line arguments. It uses a new file descriptor and file name entered by the user. The file name is used to grab the file descriptor of the existing file of the name entered. Then, that file descriptor is swapped with the file descriptor given.

## Expected output

[hertza@csci-gnode-03 hw3_AH]$ ./my_dup2_test 7 output
old fd: 3
requested new duped fd: 7
/proc/self/fd/7: new fd file path: 2011701824

## Known issues

The readlink() does not display the file name, only the allocated address space.

## Execution Environment

This program has been texted on CentOS Linux Version 7 and is compiled using GCC.
