
bin=crawler
src=crawler.cpp main.cpp
obj=$(shell echo $(src) | sed 's/\.cpp/\.o/g')
LDFLAGS=-lcurl -lboost_regex#-static
$(bin):$(obj)
	g++ -o $@ $^ $(LDFLAGS)
%.o:%.cpp
	g++ -c $< 
.PHONY:clean
clean:
	rm -f *.o $(bin)
.PHONY:debug
debug:
	echo $(bin)
	echo $(obj)
	echo $(src)
