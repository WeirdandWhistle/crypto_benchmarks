src = $(shell find *.c)
obs = $(subst .c,.o, $(src))

main : $(obs)
	gcc $(obs) -lsodium -o main

%.o : %.c
	gcc -c -o $@ $<

clean:
	rm main $(obs)