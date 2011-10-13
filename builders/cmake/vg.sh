valgrind --leak-check=full --track-origins=yes --show-reachable=yes --track-fds=yes --log-file=vg.log crtmpserver/crtmpserver ./crtmpserver/crtmpserver.lua
cat vg.log

