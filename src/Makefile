CC=g++-5
CFLAGS=-c -std=c++14 -Wall -Werror
LDFLAGS=-pthread -lpthread
SOURCES=main.cpp mapmaker.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mapmaker
BUILD_DIR=./bin

all: $(SOURCES) $(EXECUTABLE)

debug: CFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)

bench: CFLAGS += -DBENCH
bench: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) copy
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/$@
	rm *.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf $(BUILD_DIR)
	
copy:
	mkdir -p $(BUILD_DIR)
	cp -f ./Makefile $(BUILD_DIR)/Makefile
	
test:
	cd bin/ && ./mapmaker

test_c:
	cd bin/ && ./mapmaker -c ${ARG1}
	
test_t:
	cd bin/ && ./mapmaker -t ${ARG1}

test_d:
	make debug && cd bin/ && gdb --args ./mapmaker
	
test_cd:
	make debug && cd bin/ && gdb --args ./mapmaker -c ${ARG1}
	
test_ct:
	cd bin/ && ./mapmaker -c ${ARG1} -t ${ARG2}
	
test_td:
	make debug && cd bin/ && gdb --args ./mapmaker -t ${ARG1}
	
test_ctd:
	make debug && cd bin/ && gdb --args ./mapmaker -c ${ARG1} -t ${ARG2}