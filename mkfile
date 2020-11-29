<mkconfig
SRC = `{ls *.c}
OBJ = ${SRC:%.c=%.o}
#HDR = `{ls *.h}
TGT = $NAME
all :Q: $TGT 
	echo -n
$TGT : $OBJ
	$LD -o $target $LDFLAGS $prereq
%.o :V: %.c
	$CC -c -o $target $CFLAGS $prereq
%.c :Q: $HDR
	touch $target
%.h :Q:
	touch $target
clean :V:
	rm -rf $TGT *.o
install :V: $TGT
	cp -f $TGT $EXE/
	chmod 0755 $EXE/$TGT
uninstall :V:
	rm -f $EXE/$TGT

