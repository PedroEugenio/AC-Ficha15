
ficha15: ficha15.c
	gcc -g -fopenmp ficha15.c -o ficha15
	chmod 755 doplot2movie

clean:
	rm ficha15
