GXX = gcc
SRC = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRC))

compile : $(OBJS)
	$(GXX) $^ -o ../fibo

$(OBJS) : %.o : %.c
	$(CXX) -c -I ../include $< -o $@

.PHONY : clean
clean :
	rm -rf *.o	
