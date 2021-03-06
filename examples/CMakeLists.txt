#
# %injeqt copyright begin%
# Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
# %injeqt copyright end%
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

find_package(Qt5Test 5.2 REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR}/src
)

function (injeqt_add_example name_)
	file (RELATIVE_PATH sourcePath "${CMAKE_SOURCE_DIR}/examples" "${CMAKE_CURRENT_SOURCE_DIR}")

	get_filename_component (exeDir "${CMAKE_CURRENT_BINARY_DIR}/${name_}" PATH)
	file (MAKE_DIRECTORY "${exeDir}")

	add_executable (${name_} ${ARGN})
	target_link_libraries (${name_} injeqt)
	set_property (TARGET ${name_} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-error")
	qt5_use_modules (${name_} Core)
endfunction ()

injeqt_add_example(hello-world-example hello-world.cpp)
