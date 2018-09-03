cmake_minimum_required(VERSION 3.10)
project(Vinci C)

set(CMAKE_C_STANDARD 11)

INCLUDE_DIRECTORIES(/usr/include)
INCLUDE_DIRECTORIES(/usr/local/Cellar/graphicsmagick/1.3.30/include/GraphicsMagick)
add_executable(Vinci main.c http.c http.h magic.c magic.h)


execute_process(
        COMMAND GraphicsMagick-config --cppflags --ldflags --libs
        OUTPUT_VARIABLE gm_str
        ERROR_QUIET
        )

string(REPLACE "\n" " " gm_list ${gm_str})
string(REGEX REPLACE " $" "" gm_list ${gm_list})

string(REPLACE " " ";" gm_list ${gm_list})
foreach(gm_item IN LISTS gm_list)
        message(${gm_item})
        target_link_libraries(Vinci ${gm_item})
endforeach(gm_item)
target_link_libraries(Vinci -lfcgi)
target_link_libraries(Vinci -lcurl)


