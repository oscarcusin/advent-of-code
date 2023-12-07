test: day_$(day).out
	@for file in $(wildcard tests/day_$(day)*); do \
		echo $$file; \
		./day_$(day).out < $$file; \
	done

input: day_$(day).out
	./day_$(day).out < inputs/day_$(day).in

day_$(day).out: day_$(day).c $(wildcard utilities/*)
	gcc -Wall day_$(day).c utilities/*.c -o day_$(day).out -lm

clean:
	rm -f day_?.out