# NB: library/include directory usage will need some tweaking if
# you're compiling this yourself.
CC = g++
libdir = M:/DEV/dev
CFLAGS = -I$(libdir)/include -L$(libdir)/lib -L$(libdir)/lib2 -Iheaders
LDFLAGS =
LIBS = -lSDL2 -lSDL2_image
SRC = $(wildcard src/*.cpp)

wxlibs_preformatted = $(wildcard $(libdir)/lib/libwx*.a)
wxlibs_formatted = $(patsubst $(libdir)/lib/libwx%.a, -lwx%, $(wxlibs_preformatted))

OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))

main: $(OBJ)
	$(CC) -g -Wall $(OBJ) $(CFLAGS) $(LDFLAGS) $(LIBS) $(wxlibs_formatted)  -o main 

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

run: main
	main.exe

clean:
	rm obj/*