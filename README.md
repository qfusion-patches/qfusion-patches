qfusion-patches
=======

This repository contains patches for the Warsow 2.1 sources to get them
compiling cleanly on modern toolchains, as well as fixes for a few
long-standing bugs in the engine.

Changes:

* Build system rewritten in "modern" CMake
* Removal of obsolete IRC, Mumble, Steam, Matchmaker code
* SDL backend ported to SDL 3
* Fixed UI
* Fixed demo recording
* Fixed various other bugs

### Demos

Compression of demo recordings was removed as it was bugged and the cause of
the broken demo files, but correctly compressed demos should still be viewable.
For a technical explanation, read
[here](https://github.com/qfusion-patches/qfusion-patches/commit/11f843663301a66cabdf37500decef2cd406486e).

### Windows/macOS support

I have no interest in supporting Windows/macOS and as such, I've removed the
platform backends for them (as they're dead code with the new build) as well as
the relevant definitions in `q_arch.h`. I am also not vendoring zlib, libpng,
libjpeg, ... as was originally done because they are stable and can be sourced
from the system on Linux.

---

The original README.md is reproduced below.

qfusion
=======

http://www.qfusion.net/

qfusion is the id Tech 2 derived game engine powering the game <a href="https://warsow.gg/">Warsow</a>

## Features (incomplete list)

- Fully open-source under the GPLv2 and easy to mod
- Runs on Linux, OS X, Windows and Android (in development)
- Modern and fast OpenGL 3.0 and OpenGL ES 3.0 renderer, running in a dedicated program thread
- <a href="http://librocket.com/wiki/documentation">User Interface</a> based on XHTML/CSS standards with support for remote content
- Support for vertex and skeletal animation
- Powerful multiplayer & eSports features (global stats, friend lists, IRC, TV-server, etc.)
- Ready to go FPS example gametype scripts from Warsow
- Multithreaded sound mixer design
- OpenAL support
- Hardware-accelerated Ogg Theora video playback

## Extensible

- C/C++ mods (plugins) can ship new gameplay features while maintaining compatibility with the core
- UI and game mechanics scriptable with <a href="http://www.angelcode.com/angelscript/">AngelScript</a> (C++ style syntax)
- Flexible HUD scripting
- "Pure" (models, maps, textures, sounds) game content is automatically delivered to players by game servers
- Players are allowed to locally override non-"pure" game content

## License (GPLv2)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
