
# Parameter Description
# [Required] TYPE: Output target type (lib: library file, exe: executable file)
# [Required] MODE: Compile mode (debug, release)
# [Required] PLATFORM: Target running platform (linux: Linux 64bit X86, linuxarm: Linux 64bit Arm, windows: Windows 64bit X86, macosarm: MacOS 64bit Arm)
# [Required] COMPILE: Compile toolchain (local: native compile, cross: cross-compile)
# [Required] TARGET: The target name. Prefix and suffix are not required
# [Optional] TARGET_VERSION: Target version number. Only for.so and .dylib library files
# [Optional] DEPENDS: Depended libraries. Prefix and suffix are not required, separated by Spaces
# [Optional] FRAMEWORKS: Depended frameworks on the MacOS. Prefix and suffix are not added, separated by Spaces
# [Optional] MACRO_DEFINE: Additional macro definitions, separated by Spaces
# [Optional] SEARCH_INCLUDE: Additional search paths for header files, separated by Spaces
# [Optional] SEARCH_LIB: Additional search paths for library files, separated by Spaces
# [Optional] PREBUILD_COMMAND: Commands to run before compilation
# [Optional] POSTBUILD_COMMAND: Commands to run after compilation
# [Optional] FILTEROUT_CPP: The .cpp files not to compile, which must be suffified and separated by Spaces
# [Optional] FILTEROUT_CC: The .cc files not to compile, which must be suffified and separated by Spaces
# [Optional] FILTEROUT_C: The .c files not to compile, which must be suffified and separated by Spaces
# [Optional] FILTEROUT_MM: The .mm files not to compile, which must be suffified and separated by Spaces
# [Optional] FILTEROUT_H: The .h files not to scan, which must be suffified and separated by Spaces
# [Optional] FILTEROUT_HPP: The .hpp files not to scan, which must be suffified and separated by Spaces
# [Optional] WINDOWS_GUI: Windows target only, yes for GUI program, no for console program

.DEFAULT:all
.PHONY:clean
.SUFFIXES:.c .cpp .cc .o .h .hpp

# Query source files
SRCS_C=$(filter-out $(FILTEROUT_C), $(wildcard *.c **/*.c **/**/*.c **/**/**/*.c))
SRCS_CPP=$(filter-out $(FILTEROUT_CPP), $(wildcard *.cpp **/*.cpp **/**/*.cpp **/**/**/*.cpp))
SRCS_CC=$(filter-out $(FILTEROUT_CC), $(wildcard *.cc **/*.cc **/**/*.cc **/**/**/*.cc))

# Query host OS
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

# Target running platform related
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
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)
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
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)
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
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o)
endif
ifeq ($(PLATFORM), macosarm)
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

# Query header files
HEADERS_H=$(filter-out $(FILTEROUT_H), $(wildcard *.h **/*.h **/**/*.h **/**/**/*.h ../include/*.h ../$(INCLUDE_DIR)/*.h))
HEADERS_HPP=$(filter-out $(FILTEROUT_HPP), $(wildcard *.hpp **/*.hpp **/**/*.hpp **/**/**/*.hpp ../include/*.hpp ../$(INCLUDE_DIR)/*.hpp))

# Output target type related
ifeq ($(TYPE), lib)
	OUT_NAME=$(LIB_PREFIX)$(TARGET)$(BIN_EXT)
	LINK_OPTION=-shared
endif
ifeq ($(TYPE), exe)
	OUT_NAME=$(TARGET)$(EXE_EXT)
	LINK_OPTION=
endif

# Compile mode related
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

# Library name and output path
LIB_NAME=$(LIB_PREFIX)$(TARGET)$(LIB_EXT)
LIB_PATH=../$(LIB_DIR)

# Compile toolchain path
XTOOLS_PATH=/opt/xtools-uni
CLANG_PATH=/opt/clang-15

# Compile toolchain related
COMPILE_OPTION_CPP=-std=c++17
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

# Windows GUI related
ifeq ($(PLATFORM), windows)
	ifeq ($(WINDOWS_GUI), yes)
		COMPILE_OPTION:=$(COMPILE_OPTION) -DSPADAS_WINDOWS_GUI
		LINK_OPTION:=$(LINK_OPTION) -mwindows -municode
	endif
endif

# Output lib and bin options
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
endif
ifeq ($(TYPE), exe)
	ifeq ($(PLATFORM), linux)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
	endif
	ifeq ($(PLATFORM), linuxarm)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
	endif
endif
LINK_OPTION:=$(LINK_OPTION) $(WL_OPTION)

# strip command
ifeq ($(MODE), release)
	STRIP_COMMAND=$(STRIP_EXE) $(OUT_PATH)/$(OUT_NAME)
endif
ifeq ($(MODE), debug)
	STRIP_COMMAND=
endif

# Build process
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
	$(COMPILER_CPP_EXE) $(SYSROOT_OPTION) $(COMPILE_OPTION) $(COMPILE_OPTION_CPP) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.cc $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_CPP_EXE) $(SYSROOT_OPTION) $(COMPILE_OPTION) $(COMPILE_OPTION_CPP) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.mm $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_OBJC_EXE) $(SYSROOT_OPTION) $(COMPILE_OPTION) $(COMPILE_OPTION_CPP) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

pre:
	$(PREBUILD_COMMAND)
	$(PRELIB_COMMAND)
	mkdir -p $(OUT_PATH)

clean:
	rm -rf Debug Release