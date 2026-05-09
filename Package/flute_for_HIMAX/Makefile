CC:= gcc
CFLAGS = -O3 

obj:= bookshelf_IO.o dist.o dl.o err.o flute.o flute_mst.o \
      heap.o memAlloc.o mst2.o neighbors.o

all:$(obj)
	$(CC) $(CFLAGS)  $(obj) -lm
%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@ -lm

.PHONY:clean
clean:
	rm -rf $(obj) 
