#!/bin/bash

# Compila el archivo C++
g++ -o SnakeGame src/main.cpp src/SnakePlayer.cpp  -L/~/raylib  -lraylib -ldl -lm -lpthread -lX11
# Ejecuta el programa resultante
./SnakeGame
