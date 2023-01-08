all: clean klient serverik

klient: clean
	gcc client/client.c definition.c server/server.c server/vlakna/client/client.c world/world.c world/mravce/mravec/mravec.c world/mravce/mravce.c world/plocha/plocha.c world/plocha/policko/policko.c world/vlakna/worldPlayer/world_player.c world/vlakna/worldUser/world_user.c -o klient -pthread -Wall -Wimplicit-function-declaration -Wreturn-type

serverik: clean
	gcc client/client.c definition.c server/server.c server/vlakna/client/client.c world/world.c world/mravce/mravec/mravec.c world/mravce/mravce.c world/plocha/plocha.c world/plocha/policko/policko.c world/vlakna/worldPlayer/world_player.c world/vlakna/worldUser/world_user.c -o serverik -pthread -Wall -Wimplicit-function-declaration -Wreturn-type

clean:
	rm -f *.o *~ klient serverik