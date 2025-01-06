## @: non fa vedere il comando scritto nella bash

listato.txt:
	ls -altr $(HOME) > listato.txt
conteggio.txt:
	wc -l listato.txt > conteggio.txt
mostra:
	echo "Nr righe:"
	@more conteggio.txt
clean:
	rm -f listato.txt
	rm -f conteggio.txt
##

miaradice: miaradice.o sqrt.o stampa.o
	gcc -Wall -Wextra miaradice.o sqrt.o stampa.o -o miaradice

miaradice.o: miaradice.c
	gcc -Wall -c miaradice.c

sqrt.o: sqrt.c
	gcc -Wall -c sqrt.c

stampa.o: stampa.c
	gcc -Wall -c stampa.c

clean2:
	rm -f *.o
	rm -f *.~
	rm -f miaradice miaradice1 miaradice2
##

miaradice1: miaradice1.o sqrt.o stampa.o
	gcc -Wall -Wextra miaradic1e.o sqrt.o stampa.o -o miaradice1
##

miaradice2: miaradice2.o sqrt.o stampa.o
	gcc -Wall -Wextra miaradice2.o sqrt.o stampa.o -o miaradice2
