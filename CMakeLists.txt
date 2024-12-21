cmake_minimum_required(VERSION 3.29)
project(KeyboardHero C)

set(CMAKE_C_STANDARD 11)

add_executable(KeyboardHero main.c
        Funcs/Graphic.c
        Funcs/Graphic.h
        Funcs/Game.c
        Funcs/Game.h
        Funcs/Control.c
        Funcs/Control.h
        Funcs/Datas.c
        Funcs/Datas.h
        Funcs/UI.c
        Funcs/UI.h
)

