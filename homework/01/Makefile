CC=g++

NAME=prog

all: $(NAME)

test: $(NAME)
	python3 test.py "./$(NAME)"

run: $(NAME)
	./$(NAME) "${ARGS}"

$(NAME): main.o
	$(CC) -o $(NAME) main.o

main.o: main.cpp
	$(CC) -O2 -c main.cpp

clean:
	rm -rf *.o $(NAME)

