valgrind --leak-check=full --track-origins=yes --show-reachable=yes --log-file=vg.log rtmpserver/rtmpserver ./rtmpserver/rtmpserver.lua
cat vg.log

