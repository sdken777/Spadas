.DEFAULT:all
.PHONY:clean
.SUFFIXES:.c .cpp .cc .o

SRCS_C=$(filter-out $(FILTEROUT_C), $(wildcard *.c **/*.c))
SRCS_CPP=$(filter-out $(FILTEROUT_CPP), $(wildcard *.cpp **/*.cpp))
SRCS_CC=$(filter-out $(FILTEROUT_CC), $(wildcard *.cc **/*.cc))
OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)

ifeq ($(PLATFORM), linux)
	INCLUDE_DIR=includex
	BIN_DIR=binx
	LIB_PREFIX=lib
	LIB_EXT=.so
	ENV_DEFINE=SPADAS_ENV_LINUX
	ARCH_DEFINE=SPADAS_ARCH_X86
endif

ifeq ($(PLATFORM), linuxarm)
	INCLUDE_DIR=includexa
	BIN_DIR=binxa
	LIB_PREFIX=lib
	LIB_EXT=.so
	ENV_DEFINE=SPADAS_ENV_LINUX
	ARCH_DEFINE=SPADAS_ARCH_ARM
endif

ifeq ($(TYPE), lib)
	OUT_NAME=$(LIB_PREFIX)$(TARGET)$(LIB_EXT)
	LDFLAG=-shared
endif

ifeq ($(TYPE), exe)
	OUT_NAME=$(EXE_PREFIX)$(TARGET)$(EXE_EXT)
endif

ifeq ($(MODE), release)
	OBJ_PATH=Release
	OUT_PATH=../$(BIN_DIR)
	CFLAG=-O3
	STRIP_COMMAND=strip $(OUT_PATH)/$(OUT_NAME)
endif

ifeq ($(MODE), debug)
	OBJ_PATH=Debug
	OUT_PATH=../$(BIN_DIR)/debug
	CFLAG=-g -DSPADAS_DEBUG
endif

all:$(OBJS)
	mkdir -p $(OUT_PATH)
	g++ -L../$(BIN_DIR) -Wl,-rpath,./,-rpath-link,../$(BIN_DIR)/ $(LDFLAG) -o $(OUT_PATH)/$(OUT_NAME) $(addprefix $(OBJ_PATH)/, $(OBJS)) $(addprefix -l, $(DEPENDS))
	$(STRIP_COMMAND)
	$(POSTBUILD_COMMAND)
	@echo '---------------OK---------------'

%.o:%.c
	mkdir -p `dirname $(OBJ_PATH)/$@`
	gcc -Wall -I../$(INCLUDE_DIR) -fmessage-length=0 -fvisibility=hidden -fPIC $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) -o $(OBJ_PATH)/$@ -c $<

%.o:%.cpp
	mkdir -p `dirname $(OBJ_PATH)/$@`
	g++ -Wall -I../$(INCLUDE_DIR) -fmessage-length=0 -fvisibility=hidden -fPIC -std=c++11 $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) -o $(OBJ_PATH)/$@ -c $<

%.o:%.cc
	mkdir -p `dirname $(OBJ_PATH)/$@`
	g++ -Wall -I../$(INCLUDE_DIR) -fmessage-length=0 -fvisibility=hidden -fPIC -std=c++11 $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) -o $(OBJ_PATH)/$@ -c $<

clean:
	rm -rf Debug Release