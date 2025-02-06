CC = g++
OUT = ./build/proxy


main: proxy.cpp
	$(CC) proxy.cpp -o $(OUT) && ./build/proxy
