# Macros para compilacao
CC = gcc
CFLAGS = -Wextra -lpthread
FREEIMAGEFLAGS = -lfreeimage
DIR = src
DIR_IMAGES = filtered_images
TARGET = main
SRCS := $(shell find $(DIR) -name '*.c')
OBJS = $(SRCS:.c=.o)


# Macros para teste
BASH = python3
TEST_SCRIPT = test.py

.PHONY: depend clean

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o$(TARGET) $(OBJS) $(CFLAGS) $(FREEIMAGEFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) $(FREEIMAGEFLAGS) -c $< -o $@

test:all
	$(BASH) $(TEST_SCRIPT) $(TARGET)

clean:
	$(RM) ./$(TARGET)
	$(RM) $(DIR)/*.o ./*.csv
	$(RM) $(DIR_IMAGES)/*.jpg $(DIR_IMAGES)/*.jpeg
