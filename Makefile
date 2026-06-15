src = $(shell find *.c)
obs = $(subst .c,.o, $(src))
flags = -O3 -maes -msse4.1 -mpclmul -lsodium -lm

main : $(obs)
	gcc $(obs) $(flags) -o main

%.o : %.c
	gcc $(flags) -c -o $@ $<

clean:
	rm -f main $(obs)
full-clean: clean
	rm micro_aes.c micro_aes.h monocypher.c monocypher.h