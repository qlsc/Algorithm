# First target is default target, if you just type:  make

FILE=hw5.c

default: run

run: hw5
	./hw5

gdb: hw5
	gdb hw5

hw5: ${FILE}
	gcc -g -O0 -o hw5 ${FILE} -lm

emacs: ${FILE}
	emacs ${FILE}
vi: ${FILE}
	vi ${FILE}

clean:
	rm -f hw5 a.out *~

# 'make' will view $v as variable and try to expand it.
# By typing $$, make will reduce it to a single '$' and pass it to the shell.
# The shell will view $dir as a variable and expand it.
dist:
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar
	dir=`basename $$PWD`; ls -l ../$$dir.tar.gz
