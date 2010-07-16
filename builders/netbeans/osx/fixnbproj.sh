for i in `grep -R "../../../../../all/trunk/sources" *|grep -v "/.svn/"|cut -d ":" -f1`;do
	echo fixing $i;
	cat $i|sed "s/\.\.\/\.\.\/\.\.\/\.\.\/\.\.\/all\/trunk\/sources/\.\.\/\.\.\/\.\.\/\.\.\/sources/" >/tmp/temp
	mv /tmp/temp $i
done

