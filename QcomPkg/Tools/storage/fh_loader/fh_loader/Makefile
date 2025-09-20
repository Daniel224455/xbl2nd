CC=gcc
CFLAGS=-I./ -I./stringl/
BINDIR=bin
OBJDIR=obj
LDIR =lib

LIBS=-lrt

OBJ = fh_cobs.c \
      fh_crc.c \
      fh_hsuart_packet.c \
      fh_loader.c      \
      fh_loader_sha.c \
      fh_transfer.c \
      fh_transport.c \
      fh_transport_com.c \
      fh_transport_hsuart.c \
      fh_transport_linux_pipe.c \
      stringl/memscpy.c \
      stringl/memsmove.c \
      stringl/strlcat.c \
      stringl/strlcpy.c


fh_loader: $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) -o bin/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm fh_*.o
	rm fh_loader
