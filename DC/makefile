bin/DC: obj/main.o obj/message.o ../Common/obj/log.o
	cc obj/main.o obj/message.o ../Common/obj/log.o -o bin/DC

obj/main.o: src/main.c ../Common/inc/util.h ../Common/inc/log.h inc/message.h
	cc -Wall -Wextra -c src/main.c -o obj/main.o

obj/message.o: src/message.c inc/message.h ../Common/inc/util.h ../Common/inc/log.h
	cc -Wall -Wextra -c src/message.c -o obj/message.o

clean:
	rm -f ./obj/*
	rm -f ./bin/*
	