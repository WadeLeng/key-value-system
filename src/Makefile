CC = gcc -O2
OBJ = libkvs.so.1.0.0
KVSFLAGS = -Wall -fPIC -shared -g -lpthread
SRC_KVS = interface.c index.c buffer.c sync.c log.c
HEADERS = kvs.h index.h buffer.h sync.h log.h layout.h type.h

$(OBJ): $(SRC_KVS) $(HEADERS)
	$(CC) $(KVSFLAGS) -Wl,-soname,libkvs.so.1 -o $(OBJ) $(SRC_KVS)

all:
	make $(OBJ)
	make install

install:
	mv $(OBJ) /usr/local/lib
	ln -s /usr/local/lib/$(OBJ) /usr/local/lib/libkvs.so
	cp kvs.h /usr/local/include/kvs.h
	cp type.h /usr/local/include/type.h
	echo "/usr/local/lib" >> /etc/ld.so.conf
	ldconfig

uninstall:
	rm /usr/local/lib/$(OBJ) -f
	rm /usr/local/lib/libkvs.so -f
	rm /usr/local/include/kvs.h -f
	rm /usr/local/include/type.h -f
	ldconfig
