**This is simple C++ chat application written using boost/asio library for learning purposes.**

Build and installation
----------------------
To build this project you need:
 - Cmake version 3.29 (https://cmake.org/download/)
 - Boost version 1.84.0 (https://www.boost.org/users/history/version_1_84_0.html)
Build using Cmake GUI:
 1. Choose where the source code is located.
 2. Choose where Cmake will generate the build.
 3. Press Configure.
 4. Specify the generator for this project (Optimal for Windows: Visual Studio, for Linux: Unix Makefile or Ninja).
 5. Press Generate.
 6. Open the folder you choose in step 2.
 7. For Windows Visual Studio: Open the solution and press build.  
For Unix Makefile: Open the terminal and type: make -f Makefile.  
For Ninja: Open the terminal and type: ninja.

After this, if everything is done correctly there will be created two executable files: Server and Client.    
#### To run project locally: Open Server then Client using terminal.  
