CC = gcc
CFLAGS = -Wall -Wextra

all: sentiment_analysis

assignment4: sentiment_analysis.o functions.o
	$(CC) $(CFLAGS) -o sentiment_analysis sentiment_analysis.o functions.o

assignment4.o: sentiment_analysis.c functions.h
	$(CC) $(CFLAGS) -c sentiment_analysis.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f sentiment_analysis *.o