src = $(shell find *.c)
obs = $(subst .c,.o, $(src))

main : $(obs)
	gcc $(obs) -lsodium -o main

%.o : %.c
	gcc -c -o $@ $<

clean:
	rm -f main $(obs)
full-clean: clean
	rm micro_aes.c micro_aes.h monocypher.c monocypher.h