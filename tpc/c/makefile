CFLAGS+=-pthread
LDFLAGS+=-pthread


all:tpc_server_pro

tpc_server_pro:block_queue.o tpc_server_pro.o
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS) 

clean:
	rm -rf *.o tpc_server_pro