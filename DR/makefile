../Common/bin/DR: obj/main.o obj/master_list.o ../Common/obj/log.o
	cc obj/main.o obj/master_list.o ../Common/obj/log.o -o ../Common/bin/DR

obj/main.o: src/main.c ../Common/inc/util.h inc/master_list.h ../Common/inc/log.h
	cc -Wall -Wextra -c src/main.c -o obj/main.o

obj/master_list.o: src/master_list.c inc/master_list.h ../Common/inc/util.h
	cc -Wall -Wextra -c src/master_list.c -o obj/master_list.o

clean:
	rm -f ./obj/*
	rm -f ./bin/*
	