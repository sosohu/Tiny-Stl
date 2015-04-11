
include $(HOME)/Define.mk

IINC=-I$(INC)

all:
	$(CXX) $(IINC) -o main main.cc -L$(LIB_PATH) -lstl $(LIBS) $(CXXFLAGS)

clean:
	rm -rf main
