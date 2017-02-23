CC       = c++
FLAGS    = 
CFLAGS   = -g -O2 -Wno-deprecated -Wno-write-strings
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I/local/eit-linux/apps/ThreadMentor/include
TMLIB    = /local/eit-linux/apps/ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = /local/eit-linux/apps/ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = thread-main.o thread.o boat-monitor.o
EXE_FILE = prog5

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

thread.o	: thread.cpp
	${CC} ${FLAGS} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.cpp
	
boat-monitor.o	: boat-monitor.cpp
	${CC} ${FLAGS} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c boat-monitor.cpp

thread-main.o	: thread-main.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${IFLAGS} ${CFLAGS} -c thread-main.cpp
	

visual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread
	
clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}