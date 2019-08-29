CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++17
LDFLAGS = 

src_dir = src/
obj_dir = obj/
bin_dir = bin/

src = $(wildcard $(src_dir)*.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)

out: $(obj)
	$(CC) -o $(bin_dir)$@ $^ $(LDFLAGS)

-include $(dep)

%.d: %.cpp
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -rf $(obj) $(bin_dir)out	


.PHONY: cleandep
cleandep:
	rm -rf $(dep)	