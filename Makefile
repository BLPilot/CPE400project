# Makefile for each source file 
#
#
#
server: server.cpp
	@echo "=== (first compiling 'server') ==="
	g++ -o server server.cpp
#
#
#
client: client.cpp
	@echo "=== (now compiling 'client') ==="
	g++ -o client client.cpp
#
# to clean our files
#
clean:
	@echo "=== Removing files... ==="
	rm -f server client Input_file.txt

