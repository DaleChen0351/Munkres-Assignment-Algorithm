CC = g++
CFLAGS = -std=c++11
OBJDIR=../

HungApp: main.o 
	$(CC) -o HungApp main.o
	
main.o: $(OBJDIR)hungarian.cpp 
	$(CC) $(CFLAGS) -c $(OBJDIR)hungarian.cpp -o main.o

clean:
	-rm *.o HungApp

