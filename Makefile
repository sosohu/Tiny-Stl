include Define.mk

all:SRC_MAKE TEST_MAKE

SRC_MAKE:
	$(MAKE) -C $(SRC)

TEST_MAKE:SRC_MAKE
	$(MAKE) -C $(TEST)

clean:
	$(MAKE) clean -C $(SRC)
	$(MAKE) clean -C $(TEST)
	rm -rf $(LIB_PATH)/*.so $(LIB_PATH)/*.a
