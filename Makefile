CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

# LRU Policy
rodcut_lru: main.o rod_data.o rod_cut.o cache_lru.o
	$(CC) $(CFLAGS) -o $@ $^

# MRU Policy
rodcut_mru: main.o rod_data.o rod_cut.o cache_mru.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o rodcut_lru rodcut_mru