valgrind --leak-check=full --track-origins=yes --show-reachable=yes --log-file=vg.log crtmpserver/crtmpserver ./crtmpserver/crtmpserver.lua
cat vg.log

