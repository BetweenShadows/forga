# forga

## About the project
This is a simple file organizer built with C++17. The program filters files with specific extensions 
to other folders. It's also showcases the filesystem library of C++. 

## Getting started
Before anything, make sure that GCC v8.0 or above is installed. This is due to the usage
of the filesystem library. If you are using any other compiler, check that it supports 
the library too. To easily change the compiler, you can use the CXX environment variable to change
compilers. 

Example of compilation:
```text
CXX="g++-9" cmake .
make 
```
