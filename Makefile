all: main

main: main.cpp student.cpp prepod.cpp admin.cpp
	g++ -o main main.cpp student.cpp prepod.cpp admin.cpp -lpqxx -lpq

clean:
	rm -f main