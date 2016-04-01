FILES= Main.c FS_API.c IO_API.c
HEADERS= Include/FS_API.h Include/IO_API.h
CFLAGS= -g

all: command_line_interface

command_line_interface:	$(FILES) $(HEADERS)
	gcc $(CFLAGS) $(FILES) -o command_line_interface
clean:
	rm -f command_line_interface *~

