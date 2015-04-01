OBJ = main.o
INC = -I "./"

raytracer: $(OBJ)
	g++ $(OBJ) -o raytracer.exe
	rm -f $(OBJ)

main.o:
	g++ -O3 -c main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracer

run:
	./raytracer.exe
	open first_scene.bmp
