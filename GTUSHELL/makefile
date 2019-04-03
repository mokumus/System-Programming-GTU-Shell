CFLAGS=-pedantic -errors -Wall

all: gtushell pwd lsf cat wc bunedu

gtushell: gtushell.o
	gcc gtushell.o -o gtushell -lm

pwd: pwd.o
	gcc pwd.o -o pwd -lm

lsf: lsf.o
	gcc lsf.o -o lsf -lm

cat: cat.o
	gcc cat.o -o cat -lm

wc: wc.o
	gcc wc.o -o wc -lm

bunedu: bunedu.o
	gcc bunedu.o -o bunedu -lm


clean:
	rm *.o gtushell pwd lsf cat wc bunedu