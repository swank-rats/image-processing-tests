Image processing tests
=======================

Installation and project setup
-------------------
To get this project running you need [OpenCV 2.4.9](http://opencv.org/) and [Poco C++ Libraries 1.5.4](http://pocoproject.org/). We developed on Windows 7 by using Visual Studio 2013 as IDE and the Microsoft Visual C++ Compiler 18.00.21005.1 for x86 platform. The installation instructions are for Visual Studio 2013 and Windows. 

It is necessary to add new system environment variables. So do not close the window, if you have opened it during the installation process.

1. Install OpenSSL
  1. Download [OpenSSL Installer for Windows](https://slproweb.com/products/Win32OpenSSL.html)
  2. Run installer
  3. Add a new system environment variable. To do so open Control Panel -> System -> Advanced system settings -> Environment variables.
  4. At system variables press the "new" button and add a variable with name "OPENSSL" and path to e.g. "C:\OpenSSL\" (or to your new location) (with "\" at the end!)
  5. OpenSSL installation is finished
2. Install Poco C++ Libraries
  1. Download [Poco C++ Libraries 1.5.4 (development version) all](http://pocoproject.org/download/index.html) - [direct link - download poco-1.5.4-all.zip](http://pocoproject.org/releases/poco-1.5.4/)
  2. Unpack the archive file to e.g. C:\Poco
  3. Navigate to the folder
  4. Open the file "components" and remove "CppUnit", "Data", "Data/SQLite", "Data/ODBC", "Data/MySQL", "Zip"
  5. You have to edit the path to your OpenSSL installation in the file "buildwin.cmd", if it is not "C:\OpenSSL"
  6. Double click build_vs120.cmd -> this command will build the needed files
  7. Make sure that the folder e.g."C:\Poco" contains a folder "bin" and "lib". 
  8. Again add a new system environment variable. 
  9. At system variables press the "new" button and add a variable with name "POCO" and path to "C:\Poco\" (or to your new location) (with "\" at the end!)
  10. Edit the variable PATH
  11. Add "C:\Poco\bin" at the end (between the last and the new entry must be a ";"!)
  12. Poco installation is finished.
3. Clone this repository
4. Open the solution with VS 2013
5. Build the project
6. Finish - now you can run the application!
