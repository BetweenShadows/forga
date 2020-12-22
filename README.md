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

## Configuration file 
The configuration file is config.xml. In case the files doesn't exists,
the program will create a new one. 

The basic structure of config.xml is:

```xml 
<?xml version="1.0" encoding="UTF-8"?>
<forga>
	<filters>
	</filters>
</forga>
```

Inside filters tag, you can add a filter tag, like this:

```xml
<filter>
    <from>~/from</from>
    <to>~/to</to>
    <extensions>
        <ext>.docx</ext>
        <ext>.doc</ext>
        <ext>.xls</ext>
    </extensions>
</filter>
```

The from tag takes as value the directory which you want to extract the files from. The to tag contains the target
folder that the files will be stored in. The extensions tag contains many ext tags which you can specify which 
files from the source will go the the target folder.