# Cosmic
<p align="center">
    <a target="_blank" rel="noopener noreferrer"><img width="300" src="./doc/img/logo.png" alt="Cosmic Logo"></a>
<br></p>


----


<p align="center">
<a href="https://travis-ci.org/clbx/Cosmic"><img src="https://travis-ci.org/clbx/Cosmic.svg?branch=master"/></a>
<a href="https://www.codefactor.io/repository/github/clbx/cosmic"><img src="https://www.codefactor.io/repository/github/clbx/cosmic/badge" alt="CodeFactor" /></a>
<img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/clbx/Cosmic">
</br>
<a href="https://codescene.io/projects/6415/jobs/latest-successful/results"><img src="https://codescene.io/projects/6415/status.svg" /></a>
</p>

Cosmic is a fully simulated computer architecture that provides a full environment to use the Cosmic Processor, specialized devices, ROM, RAM, graphics, and more. The cosmic processor can also be used on a Raspberry Pi to interface with real-world devices. The cosmic system is all entirely accessible in a rich GUI interface that allows for full customization of the system environment and code execution.*

<p align="center">
    <a target="_blank" rel="noopener noreferrer"><img width="800" src="./doc/img/cosmicGUI.png" alt="Cosmic GUI"></a>
<br></p>

Cosmic is designed to act similar to a real, physical chip but take advantage of it being software-based and leaving some of the nuances of the physical world behind.*

With an instruction set that makes sense (and is fun), and simple memory-mapped I/O, Cosmic is an excellent "chip" to write for, using the cosmic assembler.*

<sub>* in deveopment</sub>

#### What is Cosmic?
* A very interesting project
* A Proof of Concept
* Kinda cool
* A good way to learn assembly and how processors work
* Fun to play with
* Made by two guys learning as they went along

#### What isn't Cosmic
* An optimized, properly designed processor
* Free of mistakes or beginner errors
* Something to base a serious project off of






-----

Cosmic is the Senior Project for Clay Buxton (@clbx) and Kevin Carman (@carmank) at Elizabethtown College. All academically required reports and documentation is in the ``/doc`` folder.

----

## Documentation
* [Cosmic Processor Specification](https://github.com/clbx/Cosmic/blob/master/doc/Cosmic%20Processor%20Specifications.md). This has information about how the processor works, the instruction set, and other general information
* [Cosmic Assembler Specification](https://github.com/clbx/Cosmic/blob/master/doc/Cosmic%20Assembler%20Specifications.md). This has information about how to write assembly programs for Cosmic.
* [Cosmic System Specifications](https://github.com/clbx/Cosmic/blob/master/doc/Cosmic%20System%20Specifications.md) This has information about other parts of the Cosmic system work (Video, Audio, etc.)

## Installation Instructions

### Linux

Install SDL2 using your distros package manager
```
cd Cosmic
cd src
make
```
### Mac
Install SDL2
```
brew install sdl2
```
Then make
```
cd Cosmic
cd src
make
```

### Windows (Unsupported)
Windows requires MSYS2/MinGW
Install the following packages:
```
pacman -S git mingw-w64-x86_64-toolchain mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_net mingw64/mingw-w64-x86_64-cmake make
```
Add MinGW to your Path

```
cd Cosmic
cd src
make
```

----

``/lib`` contains some included software:
*  [ImGui](https://github.com/ocornut/imgui) is a GUI library used for the interface, along with a slightly modified memory editor addon. This is under the MIT License and the license file is included accordingly
* [gl3w](https://github.com/skaslev/gl3w) A OpenGL core loader. This is under the unlicense. 

* [catch2](https://github.com/catchorg/Catch2) is used for testing. This is under the BSL Software License

SDL2 is also required to run Cosmic.


Cosmic is under the MIT License, feel free to use any part of it, but if you do please include the license file and a link to this repository.
