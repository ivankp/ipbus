SHELL := /bin/bash

# add executable names here
EXE_names := test test-rate

OBJ_ipbus := ByteTools Packet PacketBuilder PacketHeader Resend Status Transaction TransactionHeader Uhal
# add new object names here
OBJ_names := $(OBJ_ipbus)

SOBJ_names := libipbus Herakles

PYTHON_INC := /usr/include/python2.6

DIRS := lib bin

.PHONY: all shared install clean

OBJ_ipbus_full := $(addprefix lib/, $(addsuffix .o, $(OBJ_ipbus)))

OBJ := $(addprefix lib/, $(addsuffix .o, $(OBJ_names)))
EXE := $(addprefix bin/, $(EXE_names))
SOBJ := $(addprefix lib/, $(addsuffix .so, $(SOBJ_names)))

prefix = 

CFLAGS := -Wall -I. -O3

all: $(DIRS) $(EXE)
shared: $(SOBJ)
install: $(DIRS) lib/libipbus.so
	cp lib/libipbus.so $(prefix)/lib/
	cp -r ipbus $(prefix)/include/

# directories rule
$(DIRS):
	@mkdir -p $@

# object rule
$(OBJ): lib/%.o: src/%.cpp ipbus/%.h
	@echo -e "Compiling \E[0;49;96m"$@"\E[0;0m ... "
	@g++ $(CFLAGS) -fPIC -c $(filter %.cpp,$^) -o $@

# executable rule
$(EXE): bin/%: src/%.cpp $(OBJ_ipbus_full)
	@echo -e "Compiling \E[0;49;92m"$@"\E[0;0m ... "
	@g++ $(CFLAGS) $(filter %.cpp,$^) $(filter %.o,$^) -o $@

# shared objects rule
$(SOBJ): $(OBJ_ipbus_full)
	@echo -e "Compiling \E[0;49;96m"$@"\E[0;0m ... "
	@g++ -shared -O3 $(filter %.o,$^) -o $@

lib/Herakles.o: src/Herakles.cxx
	@echo -e "Compiling \E[0;49;96m"$@"\E[0;0m ... "
	@g++ $(CFLAGS) -I$(PYTHON_INC) -fPIC -c $< -o $@

lib/Herakles.so: lib/Herakles.o

clean:
	rm -f $(EXE) $(OBJ) $(SOBJ)

