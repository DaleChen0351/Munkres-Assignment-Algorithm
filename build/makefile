CC = g++
CFLAGS = -std=c++11
OBJDIR=../

HungApp: main.o hung.o
	$(CC) -o HungApp main.o hung.o
	
main.o: $(OBJDIR)main.cpp 
	$(CC) $(CFLAGS) -c $(OBJDIR)main.cpp -o main.o

hung.o: $(OBJDIR)hungarian.cpp
	$(CC) $(CFLAGS) -c $(OBJDIR)hungarian.cpp -o hung.o


clean:
	-rm *.o HungApp

