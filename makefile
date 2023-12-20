n := $(shell printf '%02d' $(day))

memcheck: day_$(n).out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./day_$(n).out < inputs/day_$(n).in

test: day_$(n).out
	@for file in $(wildcard tests/day_$(n)*); do \
		echo $$file; \
		./day_$(n).out < $$file; \
	done

input: day_$(n).out
	./day_$(n).out < inputs/day_$(n).in

day_$(n).out: day_$(n).c $(wildcard utilities/*)
	gcc -Wall -Wpedantic day_$(n).c utilities/*.c -o day_$(n).out -lm

clean:
	rm -f day_*.out
	