CC			= g++
CFLAGS		=
LINKFLAGS	=
SRCS		= STester.cpp SDFuzzySearch.cpp SFrameMatch.cpp SNoteMatch.cpp SSearchCommon.cpp SDSP.cpp SMelody.cpp SModel.cpp SUtil.cpp
OBJS		= $(SRCS:.cpp=.o)
PROG		= SDHumming

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

clean:
	rm $(OBJS) $(PROG)
