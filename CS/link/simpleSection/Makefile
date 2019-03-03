build_target:simpleSection ManualNameMangling pt pt_multithread

simpleSection:
	gcc -m32 $@.c -c -o $@.o
	gcc -m32 $@.o -o $@

ManualNameMangling:
	g++ -m32 $@.cpp -o $@

pt:
	gcc -m32 thread.c -o $@ 

pt_multithread:
	gcc -m32 thread.c -o $@ -pthread

clean:
	-rm -rf simpleSection ManualNameMangling pt pt_multithread *.o
