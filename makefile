FLAGS =-Wall -g
CC = gcc #if we want to change compiler


myshell: myShell.o DataListVariable.o Job.o Histories.o
	$(CC) $(FLAGS) -o myshell myShell.o DataListVariable.o Job.o Histories.o


myShell.o: myShell.c myShell.h
	$(CC) $(FLAGS) -c myShell.c 

DataListVariable: DataListVariable.o

DataListVariable.o: DataListVariable.c DataListVariable.h
	$(CC) $(FLAGS) -c DataListVariable.c 

Job: Job.o

Job.o: Job.c Job.h
	$(CC) $(FLAGS) -c Job.c 

Histories: Histories.o

Histories.o: Histories.c Histories.h
	$(CC) $(FLAGS) -c Histories.c 

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.out test Job DataListVariable myshell


tidy:
	valgrind --leak-check=full ./myshell

run:
	./myshell
