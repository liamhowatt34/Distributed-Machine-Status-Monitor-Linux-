../Common/bin/DX: obj/main.o obj/dx.o ../Common/obj/log.o
	cc obj/main.o obj/dx.o ../Common/obj/log.o -o ../Common/bin/DX

obj/main.o: src/main.c ../Common/inc/util.h inc/dx.h ../DR/inc/master_list.h
	cc -Wall -Wextra -c src/main.c -o obj/main.o

obj/dx.o: src/dx.c ../Common/inc/util.h inc/dx.h ../DR/inc/master_list.h
	cc -Wall -Wextra -c src/dx.c -o obj/dx.o

clean:
	rm -f ./obj/*
	rm -f ./bin/*
	