object_files = \
  scheme.o \
  object.o \
  object_lib.o \
  garbage_collector.o \
  string_representation.o \
  parse.o \
  eval.o \
  primitive_function.o \
  cl_macro.o \
  global.o \
  file_loader.o

scheme: $(object_files)
	gcc -Wall -O3 -std=gnu99 -o scheme $(object_files)

scheme.o: scheme.c
	gcc -Wall -O3 -std=gnu99 -c scheme.c

object.o: object.c
	gcc -Wall -O3 -std=gnu99 -c object.c

object_lib.o: object_lib.c
	gcc -Wall -O3 -std=gnu99 -c object_lib.c

garbage_collector.o: garbage_collector.c
	gcc -Wall -O3 -std=gnu99 -c garbage_collector.c

string_representation.o: string_representation.c
	gcc -Wall -O3 -std=gnu99 -c string_representation.c

parse.o: parse.c
	gcc -Wall -O3 -std=gnu99 -c parse.c

eval.o: eval.c
	gcc -Wall -O3 -std=gnu99 -c eval.c

primitive_function.o: primitive_function.c
	gcc -Wall -O3 -std=gnu99 -c primitive_function.c

global.o: global.c
	gcc -Wall -O3 -std=gnu99 -c global.c

cl_macro.o: cl_macro.c
	gcc -Wall -std=gnu99 -c cl_macro.c

file_loader.o: file_loader.c
	gcc -Wall -std=gnu99 -c file_loader.c


.PHONY: clean
clean:
	rm -f scheme *.o
