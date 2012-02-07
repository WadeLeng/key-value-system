CC = gcc
OBJ = libkvs.so
KVSFLAGS = -Wall -fPIC -shared -g -lpthread
SRC_KVS = interface.c index.c buffer.c sync.c layout.h type.h kvs.h
HEADERS = kvs.h layout.h type.h

$(OBJ): $(SRC_KVS) $(HEADERS)
	$(CC) $(KVSFLAGS) -o libkvs.so $(SRC_KVS)

all:
	make $(OBJ)
	make install

install:
	mv libkvs.so /usr/local/lib/
	cp kvs.h /usr/local/include/
	cp type.h /usr/local/include/
	echo "/usr/local/lib" >> /etc/ld.so.conf
	ldconfig

uninstall:
	rm /usr/local/lib/libkvs.so -f
	rm /usr/local/include/kvs.h -f
	rm /usr/local/include/type.h -f
	ldconfig
