include $(HOME)/Define.mk

IINC=-I$(INC)

OBJS= $(patsubst %.c,%.o,$(wildcard *.c))
OBJS+= $(patsubst %.cc,%.o,$(wildcard *.cc))

all:$(OBJS) $(LIB_PATH)/libstl.a $(LIB_PATH)/libstl.so

%.o:%.c
	$(CXX) $(IINC) -c $^ $(CXXFLAGS) 
%.o:%.cc
	$(CXX) $(IINC) -c $^ $(CXXFLAGS) 

$(LIB_PATH)/libstl.so:$(OBJS)
	$(CXX) $(IINC) -fPIC -shared $^ -o $@ $(CXXFLAGS)

$(LIB_PATH)/libstl.a:$(OBJS)
	$(AR) $(ARFLAGS) $@ $^ 

clean:
	rm -rf $(OBJS)
