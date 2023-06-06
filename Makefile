compile:
	gcc -o main_obj.o main.c

clean:
	rm -rf main_obj.o;

clean_run:
	make clean;
	make compile;
	./main_obj.o "./data/matrixA1048576.txt" "./data/matrixB1048576.txt";

clean_test:
	make clean;
	make compile;
	./main_obj.o "./data/testSmallA.txt" "./data/testSmallB.txt";

clean_big_test:
	make clean;
	make compile;
	./main_obj.o "./data/MatrizTestAlumnos/matrizA.txt" "./data/MatrizTestAlumnos/matrizB.txt"
