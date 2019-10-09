CC = gcc
CFALGS = -g3 -Wall -Iinclude
SRCS = $(wildcard src/*.c)
OBJS = $(addprefix obj/,$(SRCS:.c=.0))

all: $(SRCS)
	$(CC) $(CFALGS) $(SRCS) -o dhcp_service

clean:
	rm -rf dhcp_service
rmtest:
	rm -rf test
	
# test: rmtest include/custom_types.h include/custom_function.h src/checkAvaiableHost.c
	# gcc include/custom_types.h include/custom_function.h src/checkAvaiableHost.c -o test