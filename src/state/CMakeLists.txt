set(EXE_TARGET_NAME state)

set(${EXE_TARGET_NAME}_SRC
   main.cpp
   contextConcrete.cpp
   stateConcreteOne.cpp
   stateConcreteTwo.cpp
   )

add_executable(${EXE_TARGET_NAME} ${${EXE_TARGET_NAME}_SRC})
target_include_directories(${EXE_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
   "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS ${EXE_TARGET_NAME} DESTINATION bin)
