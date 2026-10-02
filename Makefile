CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

ifeq ($(OS),Windows_NT)
    TARGET = vexis.exe
    RM = powershell -NoProfile -Command "Remove-Item -Force -ErrorAction SilentlyContinue 'src/*.o', '$(TARGET)', 'vexis'; exit 0"
    PREFIX ?= $(USERPROFILE)/bin
    INSTALL_CMD = powershell -NoProfile -Command "New-Item -ItemType Directory -Force '$(PREFIX)'; Copy-Item -Force '$(TARGET)' '$(PREFIX)'"
    UNINSTALL_CMD = powershell -NoProfile -Command "Remove-Item -Force '$(PREFIX)/$(TARGET)'"
else
    TARGET = vexis
    RM = rm -f src/*.o $(TARGET) vexis.exe
    PREFIX ?= /usr/local
    BINDIR = $(PREFIX)/bin
    INSTALL_CMD = install -d $(BINDIR) && install -m 755 $(TARGET) $(BINDIR)/$(TARGET)
    UNINSTALL_CMD = rm -f $(BINDIR)/$(TARGET)
endif

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	$(INSTALL_CMD)

uninstall:
	$(UNINSTALL_CMD)

clean:
	$(RM)

.PHONY: all clean install uninstall
