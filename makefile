CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++17
LDFLAGS = 

src_dir = src/
obj_dir = obj/
bin_dir = bin/

src = $(wildcard $(src_dir)*.cpp)
obj = $(patsubst $(src_dir)%.cpp, $(obj_dir)%.o, $(src))

out: $(obj)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(bin_dir)$@ $<

$(obj) : $(src)
	$(CC) $(CFLAGS) -c $< -o $@