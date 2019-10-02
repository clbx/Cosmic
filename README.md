# Cosmic

<div style="text-align:center">
    <img width="300" src="./doc/img/logo.png">
</div>

----


<p align="center">
<a href="https://action-badges.now.sh"><img src="https://action-badges.now.sh/clbx/cosmic" /></a>
</p>

Cosmic is a fully emulated computer architecture that provides a full environment to use the Cosmic Processor, specialized devices, ROM, RAM, graphics, and more. The cosmic processor can also be used on a Raspberry Pi to interface with real-world devices. The cosmic system is all entirely accessible in a rich GUI interface that allows for full customization of the system environment and code execution.*


Cosmic is designed to act similar to a real, physical chip but take advantage of it being software-based and leaving some of the nuances of the physical world behind.*

With an instruction set that makes sense (and is fun), and simple memory-mapped I/O, Cosmic is an excellent "chip" to write for, using the cosmic assembler.*

Along with plenty of prewritten software and a simple OS cosmic is in league with retro computers such as the Apple II and Commodore 64.*

<sub>* in deveopment</sub>

-----

Cosmic is the Senior Project for Clay Buxton (@clbx) and Kevin Carman (@carmank) at Elizabethtown College. All academically required reports and documentation is in the ``/doc`` folder.

----

``/lib`` contains some included software:
*  [ImGui](https://github.com/ocornut/imgui) is a GUI library used for the interface, along with a slightly modified memory editor addon. This is under the MIT License and the license file is included accordingly
* [gl3w](https://github.com/skaslev/gl3w) A OpenGL core loader. This is not under a license. 

SDL2 is also required to run Cosmic.


Cosmic is under the MIT License, feel free to use any part of it, but if you do please include the license file and a link to this repository.
