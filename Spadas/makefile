
# 参数说明
# [必需] TYPE : 输出目标类型（lib：库文件，exe：可执行文件）
# [必需] MODE : 编译模式（debug：调试，release：发布）
# [必需] PLATFORM : 目标运行平台（linux：Linux-64位X86，linuxarm：Linux-64位Arm，windows：Windows-64位X86）
# [必需] COMPILE : 编译工具链（local：本机编译，cross：交叉编译）
# [必需] TARGET : 输出目标名称，前后缀不用加
# [可选] TARGET_VERSION : 输出目标版本号，仅针对.so库文件
# [可选] DEPENDS : 依赖库，前后缀不用加，以空格分隔
# [可选] MACRO_DEFINE : 额外的宏定义，以空格分隔
# [可选] SEARCH_INCLUDE : 额外的头文件搜索路径，以空格分隔
# [可选] SEARCH_LIB : 额外的库文件搜索路径，以空格分隔
# [可选] PREBUILD_COMMAND : 在编译前运行的命令行指令
# [可选] POSTBUILD_COMMAND : 在编译后运行的命令行指令
# [可选] FILTEROUT_CPP : 不编译的.cpp文件，需要加后缀，以空格分隔
# [可选] FILTEROUT_CC : 不编译的.cc文件，需要加后缀，以空格分隔
# [可选] FILTEROUT_C :不编译的.c文件，需要加后缀，以空格分隔

.DEFAULT:all
.PHONY:clean
.SUFFIXES:.c .cpp .cc .o

SRCS_C=$(filter-out $(FILTEROUT_C), $(wildcard *.c **/*.c))
SRCS_CPP=$(filter-out $(FILTEROUT_CPP), $(wildcard *.cpp **/*.cpp))
SRCS_CC=$(filter-out $(FILTEROUT_CC), $(wildcard *.cc **/*.cc))
OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)

ifeq ($(PLATFORM), linux)
	INCLUDE_DIR=includex
	LIB_DIR=libx
	BIN_DIR=binx
	LIB_PREFIX=lib
	LIB_EXT=.so
	ifdef TARGET_VERSION
		BIN_EXT=.so.$(TARGET_VERSION)
	else
		BIN_EXT=.so
	endif
	EXE_EXT=
	ENV_DEFINE=SPADAS_ENV_LINUX
	ARCH_DEFINE=SPADAS_ARCH_X86
endif
ifeq ($(PLATFORM), linuxarm)
	INCLUDE_DIR=includexa
	LIB_DIR=libxa
	BIN_DIR=binxa
	LIB_PREFIX=lib
	LIB_EXT=.so
	ifdef TARGET_VERSION
		BIN_EXT=.so.$(TARGET_VERSION)
	else
		BIN_EXT=.so
	endif
	EXE_EXT=
	ENV_DEFINE=SPADAS_ENV_LINUX
	ARCH_DEFINE=SPADAS_ARCH_ARM
endif
ifeq ($(PLATFORM), windows)
	INCLUDE_DIR=include64
	LIB_DIR=lib64
	BIN_DIR=bin64
	LIB_PREFIX=
	LIB_EXT=.lib
	BIN_EXT=.dll
	EXE_EXT=.exe
	ENV_DEFINE=SPADAS_ENV_WINDOWS
	ARCH_DEFINE=SPADAS_ARCH_X86
endif

ifeq ($(TYPE), lib)
	OUT_NAME=$(LIB_PREFIX)$(TARGET)$(BIN_EXT)
	LDFLAG=-shared
endif
ifeq ($(TYPE), exe)
	OUT_NAME=$(TARGET)$(EXE_EXT)
	LDFLAG=
endif

ifeq ($(MODE), release)
	OBJ_PATH=Release
	OUT_PATH=../$(BIN_DIR)
	CFLAG=-O3
endif
ifeq ($(MODE), debug)
	OBJ_PATH=Debug
	OUT_PATH=../$(BIN_DIR)/debug
	CFLAG=-g -DSPADAS_DEBUG
endif

LIB_NAME=$(LIB_PREFIX)$(TARGET)$(LIB_EXT)
LIB_PATH=../$(LIB_DIR)

ifeq ($(COMPILE), local)
	ifeq ($(PLATFORM), linux)
		GCC_EXE=gcc
		GPP_EXE=g++
		STRIP_EXE=strip
	endif
	ifeq ($(PLATFORM), linuxarm)
		GCC_EXE=gcc
		GPP_EXE=g++
		STRIP_EXE=strip
	endif
	SYSROOT_OPTION=
endif
ifeq ($(COMPILE), cross)
	ifeq ($(PLATFORM), linux)
		GCC_EXE=/opt/x-tools/x86-ubuntu1804/bin/x86_64-ubuntu18.04-linux-gnu-gcc
		GPP_EXE=/opt/x-tools/x86-ubuntu1804/bin/x86_64-ubuntu18.04-linux-gnu-g++
		STRIP_EXE=/opt/x-tools/x86-ubuntu1804/bin/x86_64-ubuntu18.04-linux-gnu-strip
		SYSROOT_OPTION=--sysroot=/opt/x-tools/x86-ubuntu1804/x86_64-ubuntu18.04-linux-gnu/sysroot
	endif
	ifeq ($(PLATFORM), linuxarm)
		GCC_EXE=/opt/x-tools/arm-ubuntu1604/bin/aarch64-ubuntu16.04-linux-gnu-gcc
		GPP_EXE=/opt/x-tools/arm-ubuntu1604/bin/aarch64-ubuntu16.04-linux-gnu-g++
		STRIP_EXE=/opt/x-tools/arm-ubuntu1604/bin/aarch64-ubuntu16.04-linux-gnu-strip
		SYSROOT_OPTION=--sysroot=/opt/x-tools/arm-ubuntu1604/aarch64-ubuntu16.04-linux-gnu/sysroot
	endif
	ifeq ($(PLATFORM), windows)
		GCC_EXE=/opt/x-tools/x86-windows/bin/x86_64-w64-mingw32-gcc
		GPP_EXE=/opt/x-tools/x86-windows/bin/x86_64-w64-mingw32-g++
		STRIP_EXE=/opt/x-tools/x86-windows/bin/x86_64-w64-mingw32-strip
		SYSROOT_OPTION=--sysroot=/opt/x-tools/x86-windows/x86_64-w64-mingw32/sysroot
	endif
endif

WL_OPTION=
PRELIB_COMMAND=
POSTLIB_COMMAND=
ifeq ($(TYPE), lib)
	ifeq ($(MODE), release)
		PRELIB_COMMAND=mkdir -p $(LIB_PATH)
	endif
	ifeq ($(PLATFORM), linux)
		ifneq ($(BIN_EXT), $(LIB_EXT))
			WL_OPTION=-Wl,-soname,$(OUT_NAME)
		endif
		ifeq ($(MODE), release)
			POSTLIB_COMMAND=cp -vf $(OUT_PATH)/$(OUT_NAME) -T $(LIB_PATH)/$(LIB_NAME)
		endif
	endif
	ifeq ($(PLATFORM), linuxarm)
		ifneq ($(BIN_EXT), $(LIB_EXT))
			WL_OPTION=-Wl,-soname,$(OUT_NAME)
		endif
		ifeq ($(MODE), release)
			POSTLIB_COMMAND=cp -vf $(OUT_PATH)/$(OUT_NAME) -T $(LIB_PATH)/$(LIB_NAME)
		endif
	endif
	ifeq ($(PLATFORM), windows)
		ifeq ($(MODE), release)
			WL_OPTION=-Wl,--out-implib,$(LIB_PATH)/$(LIB_NAME)
		endif
	endif
endif

ifeq ($(MODE), release)
	STRIP_COMMAND=$(STRIP_EXE) $(OUT_PATH)/$(OUT_NAME)
endif
ifeq ($(MODE), debug)
	STRIP_COMMAND=
endif

all:$(OBJS)
	$(PREBUILD_COMMAND)
	$(PRELIB_COMMAND)
	mkdir -p $(OUT_PATH)
	$(GPP_EXE) $(WL_OPTION) $(SYSROOT_OPTION) -L../$(LIB_DIR) -Wl,-rpath,./,-rpath-link,$(OUT_PATH)/ $(LDFLAG) -o $(OUT_PATH)/$(OUT_NAME) $(addprefix $(OBJ_PATH)/, $(OBJS)) $(addprefix -l, $(DEPENDS)) $(addprefix -L, $(SEARCH_LIB))
	$(STRIP_COMMAND)
	$(POSTLIB_COMMAND)
	$(POSTBUILD_COMMAND)
	@echo '---------------OK---------------'

%.o:%.c
	mkdir -p `dirname $(OBJ_PATH)/$@`
	$(GCC_EXE) -Wall $(SYSROOT_OPTION) -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -fmessage-length=0 -fvisibility=hidden -fPIC $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $(OBJ_PATH)/$@ -c $<

%.o:%.cpp
	mkdir -p `dirname $(OBJ_PATH)/$@`
	$(GPP_EXE) -Wall $(SYSROOT_OPTION) -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -fmessage-length=0 -fvisibility=hidden -fPIC -std=c++11 $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $(OBJ_PATH)/$@ -c $<

%.o:%.cc
	mkdir -p `dirname $(OBJ_PATH)/$@`
	$(GPP_EXE) -Wall $(SYSROOT_OPTION) -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -fmessage-length=0 -fvisibility=hidden -fPIC -std=c++11 $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $(OBJ_PATH)/$@ -c $<

clean:
	rm -rf Debug Release