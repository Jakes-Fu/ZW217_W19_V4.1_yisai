CC := gcc
AR := ar
DBG := -g
CFLAGS := -Wall -fstack-protector -fPIC -DLWM2M_CLIENT_MODE -DLWM2M_SUPPORT_JSON -DLWM2M_LITTLE_ENDIAN -DLWM2M_WITH_LOGS 
CUR_DIR := $(shell pwd)
CORE_DIR := $(CUR_DIR)/core
SDKSRC_DIR := $(CUR_DIR)/sdksrc
SDK_DIR := $(SDKSRC_DIR)/sdk
LIB_HOME := $(CUR_DIR)


TARG := liblwm2msdkL.a
TESTSDK :=testsdk
TARGSO := liblwm2msdkL.so


#============================================================= 
LIBS :=  
INCS := -I$(CORE_DIR) -I$(CORE_DIR)/er-coap-13 -I$(SDKSRC_DIR)/shared -I$(SDK_DIR) 
SRCS = $(shell ls $(CORE_DIR)/*.c) $(shell ls $(CORE_DIR)/er-coap-13/*.c)  $(shell ls $(SDKSRC_DIR)/shared/*.c) $(shell ls $(SDK_DIR)/*.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))

	
all: $(TARG) $(TARGSO)
	

	
$(TARG): $(OBJS)
	$(AR) -r $@ $^
	#cp $(TARG) $(LIB_HOME)

$(TARGSO): $(OBJS)
	$(CC)  -Wall -fstack-protector -fPIC -shared -o  $@ $^	

$(TESTSDK):
	$(CC) -Wall -fstack-protector -fPIC  $(SDKSRC_DIR)/testsdk/main.c  -I$(SDK_DIR) -o $@  -L./ -llwm2msdkL -lpthread

	
.c.o:
	$(CC) $(CFLAGS) -c $< $(INCS) -o $@

install:
	-rm -rf ./install
	-mkdir -p install/lib
	-mkdir -p install/include
	-cp -rf $(TARG) ./install/lib/
	-cp -rf $(SDKSRC_DIR)/sdk/lwm2msdk.h  ./install/include/
	-cp -rf $(TARGSO) ./install/lib/
	
.Phony: clean
clean:
	-rm -rf $(TARG) $(TARGSO) $(TESTSDK) $(OBJS) $(SDKSRC_DIR)/testsdk/main.o  ./install  *.bak

