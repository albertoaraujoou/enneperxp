clang funciones.c -o funciones \
$(pkg-config --cflags --libs sdl2 SDL2_ttf) \
-lm
