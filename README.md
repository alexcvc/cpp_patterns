# Template of C++ project with library and executable

The project template was inherited from another project and adapted for test applications in ARM.
[https://github.com/robotology/how-to-export-cpp-library]()

## Build

```shell
git clone https://github.com/alexcvc/template-lib-exe-cmake-normal.git <your_project_name> 
cd <your_project_name>
mkdir build && cd build
cmake ..
cmake --build .
```

## Customize this template

1. Change the argument <your_lib> with the name of your library/package <your_project_name> 
(the name will be the one used for find_package() call)
2. Change version information for <your_project_name>
3. Add your library source files in the subdirectories
    - src/<your_project_name>/include
    - src/<your_project_name>/src
4. Add your executable source files in the subdirectories
    - src/<your_exe_name>
5. Update src/CMakeLists.txt accordingly.
6. Add the tests to your library in /test subdirectory and modify
   test/CMakeLists.txt accordingly.
6. Add the examples to your library in /examples subdirectory and modify
   test/CMakeLists.txt accordingly.
7. Run to clear git history

```shell
sh setup.sh
```
8. Delete `startup.sh`

## Test

1. Adjust all test accordingly.
2. Delete unneeded tests

### Add a test

Use the template to add a new test:

```cmake
add_executable(YOUR_TEST_NAME_EXE your_test_source.cpp)
target_link_libraries(YOUR_TEST_NAME_EXE your_lib-in-this-cmake)
add_test(NAME YOUR_TEST_NAME COMMAND YOUR_TEST_NAME_EXE)
```

### Run the tests

1. Compile the library enabling the `BUILD_TESTING` CMake option. 
2. To list the available tests, run `ctest -N` in the build directory.
3. run the tests with use `ctest` command in the build directory.

```shell
ctest -R your_test_name`.
```

or verbose:

```shell
ctest -R -V your_test_name`.
```

## Generate documentation

1. Check that the Doxygen tool is installed.
2. Doxygen documentation will be generated using the `dox` target. See `doc/CMakeLists.txt` for details.
3. Once generated, the doxygen documentation can be browsed at `build/doc/html/index.html`.
4. The documentation will be installed in `${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}/html/`.
5. You can adjust build and installation directories using the `DOXYGEN_BUILD_DIR` and `DOXYGEN_INSTALL_DIR`.

## Licensing

1. Change a file `LICENSE`
2. Read to be sure:

 - [Choose a license](http://choosealicense.com/)
 - [tl;drLegal](https://tldrlegal.com/)

Enjoy!

A.Sacharov
