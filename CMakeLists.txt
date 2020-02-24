#    Copyright 2014 Wolfgang Thaller.
#
#    This file is part of Retro68.
#
#    Retro68 is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Retro68 is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Retro68.  If not, see <http://www.gnu.org/licenses/>.

# To use this example as a standalone project using CMake:
# mkdir build
# cd build
# cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake
# make

if(APPLE)
    add_executable(Daniels MACOSX_BUNDLE
        main.c
        )

    target_link_libraries(Daniels "-framework Carbon")
else()
    add_application(Daniels
        main.c
        daniels.r
        )
    target_link_libraries(Daniels "-lm")
    
    # save 200KB of code by removing unused stuff
    set_target_properties(Daniels PROPERTIES LINK_FLAGS "-Wl,-gc-sections")
endif()

