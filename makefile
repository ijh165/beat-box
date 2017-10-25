all: c node

c:
	cd c_app/ && make

node:
	cd node_app/ && make
	
clean:
	cd c_app/ && make clean
	cd node_app/ && make clean
	