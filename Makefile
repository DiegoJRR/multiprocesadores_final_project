compile:
	gcc -o read_file_obj.o read_file.c

clean:
	rm -rf read_file_obj.o;

clean_run:
	make clean;
	make compile;
	./read_file_obj.o "./data/matrixA1048576.txt" "./data/matrixB1048576.txt";

clean_test:
	make clean;
	make compile;
	./read_file_obj.o "./data/testSmallA.txt" "./data/testSmallB.txt";

