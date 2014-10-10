#Makefile
CC=g++

libluaudp: lua_udp.cpp socket_client.cpp
	$(CC) -fPIC -shared -o libluaudp.so lua_udp.cpp socket_client.cpp
clean: libluaudp
	rm -f libluaudp.so
