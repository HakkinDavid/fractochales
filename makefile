all: compile link
compile:
	g++ -c includes/Lightning/Lightning.cpp main.cpp -Iincludes/SFML -Iincludes/Lightning -DSFML_STATIC
link:
	g++ Lightning.o main.o -o main -L"C:\Users\David\Documents\GitHub\fractochales\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -mwindows
clean:
	del -f main *.o