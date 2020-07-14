#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <pwd.h>

static char *argv0;

static void
usage(void)
{
	fprintf(stderr, "Usage: %s [files]\n", argv0);
	exit(1);
}

char
gettypechar(struct stat *st)
{
	mode_t m = st->st_mode ;
	if(S_ISREG(m))
		return 'f' ;
	if(S_ISDIR(m))
		return 'd' ;
	if(S_ISLNK(m))
		return 'l' ;
	if(S_ISCHR(m))
		return 'c' ;
	if(S_ISBLK(m))
		return 'b' ;
	if(S_ISFIFO(m))
		return 'F' ;
	if(S_ISSOCK(m))
		return 'S' ;
	return '_' ;
}

void
finfo(const char *p)
{
	struct stat st;
	struct passwd *pwu;
	int e, a;
	char *gs, *us;
	gs = us = "_" ;
	char c;
	if(e=lstat(p, &st)){
		switch(e){
		case ENOENT :
		case ENOTDIR :
			fprintf(stderr, "%s: %s: No such file or directory.\n",
				argv0, p);
		break;
		case EACCES :
			fprintf(stderr, "%s: %s: Access denied.\n",
				argv0, p);
		break;
		default:
			fprintf(stderr, "%s: %s: Unknown error.\n",
			argv0, p);
		}
		return;
	}
	
	c = gettypechar(&st) ;
	a =  0777 & st.st_mode ;
	pwu = getpwuid(st.st_uid) ;
	printf("%s %c %04o %s %u\n",
		p, c, a, pwu->pw_name, st.st_size);
}

main(int argc, char *argv[])
{
	char buf[BUFSIZ];
	int i, l;
	argv0 = argv[0] ;
	if(argc==1)
		while(fgets(buf, sizeof(buf), stdin)){
			l = strnlen(buf, sizeof(buf)) - 1 ;
			if(buf[l]=='\n')
				buf[l] = 0 ;
			finfo(buf);
		}
	else
		for(i=1;i<argc;++i)
			finfo(argv[i]);
	exit(0);
}
