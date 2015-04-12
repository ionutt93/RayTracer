OBJ = main.o
INC = -I "./"

raytracer: $(OBJ)
	g++ -O3 $(OBJ) -o raytracer.exe 
	rm -f $(OBJ)

main.o:
	g++ -g -c main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracer

run:
	./raytracer.exe
	open first_scene.bmp

test:
	g++ Plane.cpp main.cpp $(INC) -o raytracer.exe