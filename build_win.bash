x86_64-w64-mingw32-gcc code/main.c code/visual.c code/logica.c -o xadrez.exe \
  -I../raylib-win/include \
  -L../raylib-win/lib \
  -lraylib -lopengl32 -lgdi32 -lwinmm
