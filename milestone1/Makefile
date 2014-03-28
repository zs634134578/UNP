objects_serv = mtserver.o common.o
objects_cli = mtclient.o common.o

mtserver mtclient:$(objects_serv) $(objects_cli)
	g++ -o mtserver $(objects_serv)
	g++ -o mtclient $(objects_cli)

mtclient.o:mtclient.cpp common.h

mtserver.o:mtserver.cpp common.h

common.o:common.h

.PHONY : clean
clean:
	rm $(objects_serv) $(objects_cli) *~
