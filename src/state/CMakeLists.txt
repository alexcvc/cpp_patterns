set(EXE_TARGET_NAME state)

set(${EXE_TARGET_NAME}_SRC
   main.cpp
   )

add_executable(${EXE_TARGET_NAME} ${${EXE_TARGET_NAME}_SRC})

install(TARGETS ${EXE_TARGET_NAME} DESTINATION bin)
