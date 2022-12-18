##
# CMakefile.txt: examples/simple/CMakeLists.txt
# Project:
# Date: 2022-12-18
# Notes:
##

set(EXE_TARGET_NAME simple)

set(${EXE_TARGET_NAME}_SRC
        main.cpp
)

add_executable(${EXE_TARGET_NAME} ${${EXE_TARGET_NAME}_SRC})
#target_link_libraries(${EXE_TARGET_NAME} your_lib::your_lib)

install(TARGETS ${EXE_TARGET_NAME} DESTINATION bin)
