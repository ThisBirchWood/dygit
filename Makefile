CC := gcc

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  # Homebrew's OpenSSL is keg-only, so ask brew where it lives
  OPENSSL_PREFIX := $(shell brew --prefix openssl@3 2>/dev/null)
  ifeq ($(OPENSSL_PREFIX),)
    $(error OpenSSL not found. Run: brew install openssl@3)
  endif
  OPENSSL_CFLAGS := -I$(OPENSSL_PREFIX)/include
  OPENSSL_LIBS   := -L$(OPENSSL_PREFIX)/lib -lcrypto
else
  # Linux: pkg-config knows where libssl-dev put things
  OPENSSL_CFLAGS := $(shell pkg-config --cflags libcrypto 2>/dev/null)
  OPENSSL_LIBS   := $(shell pkg-config --libs libcrypto 2>/dev/null)
  ifeq ($(OPENSSL_LIBS),)
    $(error OpenSSL not found. Install it: sudo apt install libssl-dev)
  endif
endif

CFLAGS  := -Wall -Wextra -std=c11 -MMD -MP -Iinclude $(OPENSSL_CFLAGS)
LDLIBS  := $(OPENSSL_LIBS)
SRC_DIR := src
BUILD_DIR := build
TARGET := dygit

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)