
# 参数说明
# [必需] TYPE : 输出目标类型（lib：库文件，exe：可执行文件）
# [必需] MODE : 编译模式（debug：调试，release：发布）
# [必需] PLATFORM : 目标运行平台（linux：Linux-64位X86，linuxarm：Linux-64位Arm，windows：Windows-64位X86，macosarm：MacOS-64位Arm，nilrt：NILRT-64位X86）
# [必需] COMPILE : 编译工具链（local：本机编译，cross：交叉编译）
# [必需] TARGET : 输出目标名称，前后缀不用加
# [可选] TARGET_VERSION : 输出目标版本号，仅针对.so和.dylib库文件
# [可选] DEPENDS : 依赖库，前后缀不用加，以空格分隔
# [可选] FRAMEWORKS : 依赖MacOS框架，前后缀不用加，以空格分隔
# [可选] MACRO_DEFINE : 额外的宏定义，以空格分隔
# [可选] SEARCH_INCLUDE : 额外的头文件搜索路径，以空格分隔
# [可选] SEARCH_LIB : 额外的库文件搜索路径，以空格分隔
# [可选] PREBUILD_COMMAND : 在编译前运行的命令行指令
# [可选] POSTBUILD_COMMAND : 在编译后运行的命令行指令
# [可选] FILTEROUT_CPP : 不编译的.cpp文件，需要加后缀，以空格分隔
# [可选] FILTEROUT_CC : 不编译的.cc文件，需要加后缀，以空格分隔
# [可选] FILTEROUT_C :不编译的.c文件，需要加后缀，以空格分隔
# [可选] FILTEROUT_MM :不编译的.mm文件，需要加后缀，以空格分隔
# [可选] FILTEROUT_H : 不依赖的.h文件，需要加后缀，以空格分隔
# [可选] FILTEROUT_HPP : 不依赖的.hpp文件，需要加后缀，以空格分隔

.DEFAULT:all
.PHONY:clean
.SUFFIXES:.c .cpp .cc .o .h .hpp

# 获取源文件集
SRCS_C=$(filter-out $(FILTEROUT_C), $(wildcard *.c **/*.c **/**/*.c **/**/**/*.c))
SRCS_CPP=$(filter-out $(FILTEROUT_CPP), $(wildcard *.cpp **/*.cpp **/**/*.cpp **/**/**/*.cpp))
SRCS_CC=$(filter-out $(FILTEROUT_CC), $(wildcard *.cc **/*.cc **/**/*.cc **/**/**/*.cc))

# 确定本机平台
HOST_OS_NAME=$(shell uname -sm | awk -F' ' '{print tolower($$1)}' | awk -F'-' '{print $$1}')
HOST_ARCH_NAME=$(shell uname -sm | awk -F' ' '{print tolower($$2)}')
HOST_NAME=$(HOST_OS_NAME)-$(HOST_ARCH_NAME)
ifeq ($(HOST_NAME), linux-x86_64)
	HOST=linux
endif
ifeq ($(HOST_NAME), linux-aarch64)
	HOST=linuxarm
endif
ifeq ($(HOST_NAME), msys_nt-x86_64)
	HOST=windows
endif
ifeq ($(HOST_NAME), darwin-arm64)
	HOST=macosarm
endif

# 目标运行平台相关
ifeq ($(PLATFORM), linux)
	USE_CLANG=yes
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
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)
endif
ifeq ($(PLATFORM), linuxarm)
	USE_CLANG=yes
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
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)
endif
ifeq ($(PLATFORM), windows)
	USE_CLANG=yes
	INCLUDE_DIR=include64
	LIB_DIR=lib64
	BIN_DIR=bin64
	LIB_PREFIX=
	LIB_EXT=.lib
	BIN_EXT=.dll
	EXE_EXT=.exe
	ENV_DEFINE=SPADAS_ENV_WINDOWS
	ARCH_DEFINE=SPADAS_ARCH_X86
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)
endif
ifeq ($(PLATFORM), macosarm)
	USE_CLANG=yes
	INCLUDE_DIR=includema
	LIB_DIR=libma
	BIN_DIR=binma
	LIB_PREFIX=lib
	LIB_EXT=.dylib
	ifdef TARGET_VERSION
		BIN_EXT=.$(TARGET_VERSION).dylib
	else
		BIN_EXT=.dylib
	endif
	EXE_EXT=
	ENV_DEFINE=SPADAS_ENV_MACOS
	ARCH_DEFINE=SPADAS_ARCH_ARM
	SRCS_MM=$(filter-out $(FILTEROUT_MM), $(wildcard *.mm **/*.mm **/**/*.mm **/**/**/*.mm))
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o) $(SRCS_MM:.mm=.o)
endif
ifeq ($(PLATFORM), nilrt)
	USE_CLANG=no
	INCLUDE_DIR=includeni
	LIB_DIR=libni
	BIN_DIR=binni
	LIB_PREFIX=lib
	LIB_EXT=.so
	ifdef TARGET_VERSION
		BIN_EXT=.so.$(TARGET_VERSION)
	else
		BIN_EXT=.so
	endif
	EXE_EXT=
	ENV_DEFINE=SPADAS_ENV_NILRT
	ARCH_DEFINE=SPADAS_ARCH_X86
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)
endif

# 获取头文件集
HEADERS_H=$(filter-out $(FILTEROUT_H), $(wildcard *.h **/*.h **/**/*.h **/**/**/*.h ../include/*.h ../$(INCLUDE_DIR)/*.h))
HEADERS_HPP=$(filter-out $(FILTEROUT_HPP), $(wildcard *.hpp **/*.hpp **/**/*.hpp **/**/**/*.hpp ../include/*.hpp ../$(INCLUDE_DIR)/*.hpp))

# 输出目标类型相关
ifeq ($(TYPE), lib)
	OUT_NAME=$(LIB_PREFIX)$(TARGET)$(BIN_EXT)
	LINK_OPTION=-shared
endif
ifeq ($(TYPE), exe)
	OUT_NAME=$(TARGET)$(EXE_EXT)
	LINK_OPTION=
endif

# 编译模式相关
COMPILE_OPTION=-fmessage-length=0 -fvisibility=hidden -fPIC -Wall
ifeq ($(MODE), release)
	OBJ_PATH=Release/$(PLATFORM)
	OUT_PATH=../$(BIN_DIR)
	COMPILE_OPTION:=$(COMPILE_OPTION) -O3
endif
ifeq ($(MODE), debug)
	OBJ_PATH=Debug/$(PLATFORM)
	OUT_PATH=../$(BIN_DIR)/debug
	COMPILE_OPTION:=$(COMPILE_OPTION) -g
endif
OBJS:=$(addprefix $(OBJ_PATH)/, $(OBJS))

# lib名称与输出路径
LIB_NAME=$(LIB_PREFIX)$(TARGET)$(LIB_EXT)
LIB_PATH=../$(LIB_DIR)

# 交叉编译工具链路径
XTOOLS_PATH=/opt/xtools-uni
CLANG_PATH=/opt/clang-15

# 编译工具链相关
ifeq ($(USE_CLANG), yes)
	COMPILE_OPTION:=$(COMPILE_OPTION) -std=c++17
	COMPILER_C_EXE=$(CLANG_PATH)/bin/clang
	COMPILER_CPP_EXE=$(CLANG_PATH)/bin/clang++
	COMPILER_OBJC_EXE=$(CLANG_PATH)/bin/clang++
	STRIP_EXE=$(CLANG_PATH)/bin/llvm-strip
	ifeq ($(COMPILE), local)
		SYSROOT_OPTION=
	endif
	ifeq ($(COMPILE), cross)
		LINK_OPTION:=$(LINK_OPTION) -fuse-ld=lld
		ifeq ($(PLATFORM), linux)
			SYSROOT_OPTION=--sysroot=$(XTOOLS_PATH)/x86-ubuntu2204 --target=x86_64-ubuntu22.04-linux-gnu
		endif
		ifeq ($(PLATFORM), linuxarm)
			SYSROOT_OPTION=--sysroot=$(XTOOLS_PATH)/arm-ubuntu2204 --target=aarch64-ubuntu22.04-linux-gnu
		endif
		ifeq ($(PLATFORM), windows)
			SYSROOT_OPTION=--sysroot=$(XTOOLS_PATH)/x86-windows10 --target=x86_64-w64-mingw32
		endif
		ifeq ($(PLATFORM), macosarm)
			SYSROOT_OPTION=--sysroot=$(XTOOLS_PATH)/arm-macos1203 --target=arm64-apple-darwin21.4.0
		endif
	endif
endif
ifeq ($(COMPILE), cross)
	ifeq ($(PLATFORM), nilrt)
		COMPILE_OPTION:=$(COMPILE_OPTION) -std=c++11
		SYSROOT_OPTION=--sysroot=/opt/xtools-uni/x86-nilrt17/sysroots/core2-64-nilrt-linux
		ifeq ($(HOST), windows)
			COMPILER_C_EXE=/opt/xtools-uni/x86-nilrt17/sysroots/i686-nilrtsdk-mingw32/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-gcc -m64 -march=core2 -mtune=core2 -msse3 -mfpmath=sse
			COMPILER_CPP_EXE=/opt/xtools-uni/x86-nilrt17/sysroots/i686-nilrtsdk-mingw32/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-g++ -m64 -march=core2 -mtune=core2 -msse3 -mfpmath=sse
			STRIP_EXE=/opt/xtools-uni/x86-nilrt17/sysroots/i686-nilrtsdk-mingw32/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-strip
		endif
		ifeq ($(HOST), linux)
			COMPILER_C_EXE=/opt/xtools-uni/x86-nilrt17/sysroots/x86_64-nilrtsdk-linux/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-gcc -m64 -march=core2 -mtune=core2 -msse3 -mfpmath=sse
			COMPILER_CPP_EXE=/opt/xtools-uni/x86-nilrt17/sysroots/x86_64-nilrtsdk-linux/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-g++ -m64 -march=core2 -mtune=core2 -msse3 -mfpmath=sse
			STRIP_EXE=/opt/xtools-uni/x86-nilrt17/sysroots/x86_64-nilrtsdk-linux/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-strip
		endif
	endif
endif

# 输出lib与bin选项
PRELIB_COMMAND=
POSTLIB_COMMAND=
ifeq ($(PLATFORM), linux)
	WL_OPTION=-Wl,-rpath,./
endif
ifeq ($(PLATFORM), linuxarm)
	WL_OPTION=-Wl,-rpath,./
endif
ifeq ($(PLATFORM), windows)
	WL_OPTION=-Wl
endif
ifeq ($(PLATFORM), macosarm)
	WL_OPTION=-Wl,-rpath,./
endif
ifeq ($(PLATFORM), nilrt)
	WL_OPTION=-Wl,-rpath,./
endif
ifeq ($(TYPE), lib)
	ifeq ($(MODE), release)
		PRELIB_COMMAND=mkdir -p $(LIB_PATH)
	endif
	ifeq ($(PLATFORM), linux)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
		ifneq ($(BIN_EXT), $(LIB_EXT))
			WL_OPTION:=$(WL_OPTION),-soname,$(OUT_NAME)
		endif
		ifeq ($(MODE), release)
			POSTLIB_COMMAND=cp -vf $(OUT_PATH)/$(OUT_NAME) $(LIB_PATH)/$(LIB_NAME)
		endif
	endif
	ifeq ($(PLATFORM), linuxarm)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
		ifneq ($(BIN_EXT), $(LIB_EXT))
			WL_OPTION:=$(WL_OPTION),-soname,$(OUT_NAME)
		endif
		ifeq ($(MODE), release)
			POSTLIB_COMMAND=cp -vf $(OUT_PATH)/$(OUT_NAME) $(LIB_PATH)/$(LIB_NAME)
		endif
	endif
	ifeq ($(PLATFORM), windows)
		ifeq ($(MODE), release)
			WL_OPTION:=$(WL_OPTION),--out-implib,$(LIB_PATH)/$(LIB_NAME)
		endif
	endif
	ifeq ($(PLATFORM), macosarm)
		WL_OPTION:=$(WL_OPTION),-install_name,$(OUT_NAME)
		ifeq ($(MODE), release)
			POSTLIB_COMMAND=cp -vf $(OUT_PATH)/$(OUT_NAME) $(LIB_PATH)/$(LIB_NAME)
		endif
	endif
	ifeq ($(PLATFORM), nilrt)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
		ifneq ($(BIN_EXT), $(LIB_EXT))
			WL_OPTION:=$(WL_OPTION),-soname,$(OUT_NAME)
		endif
		ifeq ($(MODE), release)
			POSTLIB_COMMAND=cp -vf $(OUT_PATH)/$(OUT_NAME) $(LIB_PATH)/$(LIB_NAME)
		endif
	endif
endif
ifeq ($(TYPE), exe)
	ifeq ($(PLATFORM), linux)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
	endif
	ifeq ($(PLATFORM), linuxarm)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
	endif
	ifeq ($(PLATFORM), nilrt)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
	endif
endif
LINK_OPTION:=$(LINK_OPTION) $(WL_OPTION)

# strip命令
ifeq ($(MODE), release)
	STRIP_COMMAND=$(STRIP_EXE) $(OUT_PATH)/$(OUT_NAME)
endif
ifeq ($(MODE), debug)
	STRIP_COMMAND=
endif

# 编译流程
all:pre $(OBJS)
	$(COMPILER_CPP_EXE) $(SYSROOT_OPTION) $(LINK_OPTION) -L../$(LIB_DIR) -o $(OUT_PATH)/$(OUT_NAME) $(OBJS) $(addprefix -l, $(DEPENDS)) $(addprefix -L, $(SEARCH_LIB)) $(addprefix -framework , $(FRAMEWORKS))
	$(STRIP_COMMAND)
	$(POSTLIB_COMMAND)
	$(POSTBUILD_COMMAND)
	@echo '---------------OK---------------'

$(OBJ_PATH)/%.o:%.c $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_C_EXE) $(SYSROOT_OPTION) $(COMPILE_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.cpp $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_CPP_EXE) $(SYSROOT_OPTION) $(COMPILE_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.cc $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_CPP_EXE) $(SYSROOT_OPTION) $(COMPILE_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.mm $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_OBJC_EXE) $(SYSROOT_OPTION) $(COMPILE_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

pre:
	$(PREBUILD_COMMAND)
	$(PRELIB_COMMAND)
	mkdir -p $(OUT_PATH)

clean:
	rm -rf Debug Release