# CS163 Data Structure Visualizer Project

- [CS163 Data Structure Visualizer Project](#cs163-data-structure-visualizer-project)
  - [Project Overview](#project-overview)
  - [Technical Requirement](#technical-requirement)
  - [Installation](#installation)
  - [Demo](#demo)
  - [Licenses](#licenses)

## Project Overview

This data structure visualizer app is designed to develop a tool for illustrating several fundamental and popular data structures, such as **Hash Tables, Linked Lists, Trees, and Graphs**. The primary focus is on time-based animations and robust state management to facilitate the understanding of complex transformations.
    
In addition, the program is projected to give users a smooth experience with such minimal designs to enlighten the highlights of both the visualizations and the pseudocodes.
    
Another necessary feature is the ability to traverse the entire snapshot history. The playback controller is designed to be user-friendly, with contrasting colors and specific indications to distinguish the moments when users are blocked from interactions or not.

## Technical Requirement
- Platform: Windows 11 64-bit
- Programming Language: C++17 standard
- Build System: CMake Version 3.22+
- Compiler: MSYS2
- IDE: Visual Studio Code, in case the expected installation went south.

## Installation
Please follow the instructions in order and remind that it's required to **NOT** modify anything from the default settings (except the optional sections).

- Download [Visual Studio Code](https://code.visualstudio.com).
- Download [CMake](https://cmake.org/download/) by choosing the latest version of Windows x64 Installer (.msi file).
- Install MSYS2 compiler by following the instructions of the most voted comment in [StackOverFlow](https://stackoverflow.com/questions/30069830/how-can-i-install-mingw-w64-and-msys2).
- Open MSYS2 UCRT64 command prompt ```C:\msys64\ucrt64.exe``` and paste this command: ```pacman -S mingw-w64-ucrt-x86_64-raylib```. This allows the build system to access the graphics library Raylib.
- Download [the source code release](https://github.com/TrendBattles/VisualizerProject/releases) and unzip the folder.
- Execute ```run.bat```, the command prompt should print debug infos during the build process.
- Open ```build/DSVisualizer.exe``` and voila! The app is ready for your experiences.

## Demo
Currently in progress...

## Licenses

It’s an open educational tool—feel free to use, study, and build upon the code. See the [LICENSE](LICENSE) file for the full legal text.