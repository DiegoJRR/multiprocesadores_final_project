run: read_file.c
	gcc read_file.c -o read_file_obj.o; ./read_file_obj.o

clean:
	rm -rf read_file_obj.o

clean_run:
	rm -rf read_file_obj.o;
	gcc read_file.c -o read_file_obj.o; ./read_file_obj.o
