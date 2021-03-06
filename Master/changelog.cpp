#include "config.h"

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include "cfg.h"
#include "changelog.h"
#include "ServerCore.h"
#include "MetaLoggerCtrl.h"

#define MAXLOGLINESIZE 200000U
#define MAXLOGNUMBER 1000U
static uint32_t BackLogsNumber;
static FILE *fd;

void changelog_rotate() {
	char logname1[100],logname2[100];
	uint32_t i;
	if (fd) {
		fclose(fd);
		fd=NULL;
	}
	if (BackLogsNumber>0) {
		for (i=BackLogsNumber ; i>0 ; i--) {
			snprintf(logname1,100,"changelog.%"PRIu32".mfs",i);
			snprintf(logname2,100,"changelog.%"PRIu32".mfs",i-1);
			rename(logname2,logname1);
		}
	} else {
		unlink("changelog.0.mfs");
	}
	matomlserv_broadcast_logrotate();
}

void changelog(uint64_t version,const char *format,...) {
	static char printbuff[MAXLOGLINESIZE];
	va_list ap;
	uint32_t leng;

	va_start(ap,format);
	leng = vsnprintf(printbuff,MAXLOGLINESIZE,format,ap);
	va_end(ap);
	if (leng>=MAXLOGLINESIZE) {
		printbuff[MAXLOGLINESIZE-1]='\0';
		leng=MAXLOGLINESIZE;
	} else {
		leng++;
	}

	if (fd==NULL) {
		fd = fopen("changelog.0.mfs","a");
		if (!fd) {
			syslog(LOG_NOTICE,"lost MFS change %"PRIu64": %s",version,printbuff);
		}
	}

	if (fd) {
		fprintf(fd,"%"PRIu64": %s\n",version,printbuff);
		fflush(fd);
	}
	matomlserv_broadcast_logstring(version,(uint8_t*)printbuff,leng);
}

void changelog_reload(void) {
	BackLogsNumber = cfg_getuint32("BACK_LOGS",50);
	if (BackLogsNumber>MAXLOGNUMBER) {
		syslog(LOG_WARNING,"BACK_LOGS value too big !!!");
		BackLogsNumber = MAXLOGLINESIZE;
	}
}

int changelog_init(void) {
	BackLogsNumber = cfg_getuint32("BACK_LOGS",50);
	if (BackLogsNumber>MAXLOGNUMBER) {
		fprintf(stderr,"BACK_LOGS value too big !!!");
		return -1;
	}

    CServerCore::getInstance()->reload_register(changelog_reload);
	fd = NULL;
	return 0;
}
