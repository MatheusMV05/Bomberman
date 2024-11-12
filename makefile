all:
	gcc ./src/*.c -I./include -o space.out -lm

run:
	./space.out

clean:
	rm space.out