default: main

main:
	g++ -std=c++11 main.cpp -o runner

clean:
	-rm -f runner 
