# createMathMLTests
The simplest way to build this program is to put it in the libsbml/examples/c++ directory, and modify that CMakeLists.txt file to add the line

add_subdirectory(createMathMLTests)

at the beginning (just before 'foreach').
