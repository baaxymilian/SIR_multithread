# SIR_multithread

[![Build Status](https://travis-ci.com/baaxymilian/SIR_multithread.svg?branch=main)](https://travis-ci.com/baaxymilian/SIR_multithread)


SIR-type algorithm application using custom windows api port for multithreading

<img src="https://media.giphy.com/media/my3kMiPuhDR7n1xvgM/giphy.gif" width="600">

## Downloading and building project

Recommended way to build this project is via Makefile with yours favourite IDE.

You can also use builder commands provided in .travis.yml file.

If your system does not have freeglut installed, simply put bin/freeglut.dll file in output folder after successful build.

Check your compiler. You may need to edit Makefile to change lib folder to lib/x64.

Important info: project utilizes Windows API and OpenGL and was designed to work on Windows OS.

