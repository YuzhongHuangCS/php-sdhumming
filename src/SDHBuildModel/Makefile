CC			= g++
CFLAGS		=
LINKFLAGS	=
SRCS		= SBuilder.cpp SBuildModel.cpp midifile.cpp
OBJS		= $(SRCS:.cpp=.o)
PROG		= SDHBuildModel

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

clean:
	rm $(OBJS) $(PROG)
