# Unix HW3 Q4

## How to use

This project contains two files: 

* q4.c 
* Makefile

Run the command `make` in order to compile the .cpp file.

Note: the Makefile compiles both programs in this homework.

Run `./q4` to execute the code.

## Description

This code creates a new directory, changes the directory to the current working directory (cwd), and then deletes the directory. 

Once the directory is succesfully deleted, it attempts to open the directory 3 different ways:

* '.'
* '../foo'
* '/tmp/foo'

Only opening the directory using '.' is successful.

This illustrates that the cwd is still maintained even though the folder is deleted.

## Expected output

[hertza@csci-gnode-03 hw3_AH]$ ./q4
running
folder removed successfully
opened directory with .
could not open directory using ../foo
could not open directory using /tmp/foo


## Execution Environment

This program has been texted on CentOS Linux Version 7 and is compiled using GCC.
