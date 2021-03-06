cmake_minimum_required(VERSION 3.15.5)

# Set the name of the project and store it in a variable PROJECT_NAME
project(string_extras)

set(Headers "")
list(APPEND Headers ${CMAKE_CURRENT_SOURCE_DIR}/inc)

set(Sources src/string/extras/CharacterEncoder.cpp)

if (MSVC)
    add_compile_options(-Zi 
                        -W4
                        -EHsc
                        -D_CRT_SECURE_NO_WARNINGS
                        -DSTRICT
                        -DUNICODE
                        -D_UNICODE)
                        
    set(Sources_Windows src/string/extras/windows/CharacterEncoder.cpp)
    
    list(APPEND Sources ${Sources_Windows})
else()
    # fail until we target more platforms
    message(FATAL_ERROR "compiler not supported")
endif()

add_library(${PROJECT_NAME} ${Sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${Headers})