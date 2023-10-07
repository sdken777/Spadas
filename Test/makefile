
# Parameter Description
# [Required] TYPE: Output target type (lib: library file, exe: executable file)
# [Required] MODE: Compile mode (debug, release)
# [Required] PLATFORM: Target running platform (linux: Linux 64bit X86, linuxarm: Linux 64bit Arm, windows: Windows 64bit X86, macos: MacOS 64bit X86)
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
ifeq ($(PLATFORM), macos)
	INCLUDE_DIR=includem
	LIB_DIR=libm
	BIN_DIR=binm
	LIB_PREFIX=lib
	LIB_EXT=.dylib
	ifdef TARGET_VERSION
		BIN_EXT=.$(TARGET_VERSION).dylib
	else
		BIN_EXT=.dylib
	endif
	EXE_EXT=
	ENV_DEFINE=SPADAS_ENV_MACOS
	ARCH_DEFINE=SPADAS_ARCH_X86
	SRCS_MM=$(filter-out $(FILTEROUT_MM), $(wildcard *.mm **/*.mm **/**/*.mm **/**/**/*.mm))
	OBJS=$(SRCS_CPP:.cpp=.o) $(SRCS_CC:.cc=.o) $(SRCS_C:.c=.o) $(SRCS_MM:.mm=.o)
endif

# Query header files
HEADERS_H=$(filter-out $(FILTEROUT_H), $(wildcard *.h **/*.h **/**/*.h **/**/**/*.h ../include/*.h ../$(INCLUDE_DIR)/*.h))
HEADERS_HPP=$(filter-out $(FILTEROUT_HPP), $(wildcard *.hpp **/*.hpp **/**/*.hpp **/**/**/*.hpp ../include/*.hpp ../$(INCLUDE_DIR)/*.hpp))

# Output target type related
ifeq ($(TYPE), lib)
	OUT_NAME=$(LIB_PREFIX)$(TARGET)$(BIN_EXT)
	LDFLAG=-shared
endif
ifeq ($(TYPE), exe)
	OUT_NAME=$(TARGET)$(EXE_EXT)
	LDFLAG=
endif

# Compile mode related
ifeq ($(MODE), release)
	OBJ_PATH=Release/$(PLATFORM)
	OUT_PATH=../$(BIN_DIR)
	CFLAG=-O3
endif
ifeq ($(MODE), debug)
	OBJ_PATH=Debug/$(PLATFORM)
	OUT_PATH=../$(BIN_DIR)/debug
	CFLAG=-g -DSPADAS_DEBUG
endif
OBJS:=$(addprefix $(OBJ_PATH)/, $(OBJS))

# Library name and output path
LIB_NAME=$(LIB_PREFIX)$(TARGET)$(LIB_EXT)
LIB_PATH=../$(LIB_DIR)

# Host OS (Not considering architecture)
HOST_OS = $(shell if [ -d "/c/x-tools" ]; then echo "windows"; elif [ -d "/cygdrive/c/x-tools" ]; then echo "windows"; elif [ -d "/Volumes/x-tools" ]; then echo "macos"; else echo "linux"; fi;)
IS_CYGWIN = $(shell if [ -d "/cygdrive/c/x-tools" ]; then echo "yes"; else echo "no"; fi;)

# Compile toolchain path
ifeq ($(HOST_OS), linux)
	XTOOLS_PATH=/opt/x-tools
endif
ifeq ($(HOST_OS), windows)
	ifeq ($(IS_CYGWIN), yes)
		XTOOLS_PATH=/cygdrive/c/x-tools
		CYGWIN_XTOOLS_PATH=C:/x-tools
	else
		XTOOLS_PATH=/c/x-tools
	endif
endif
ifeq ($(HOST_OS), macos)
	XTOOLS_PATH=/Volumes/x-tools
endif

# Compile toolchain related
LINK_OPTION=
ifeq ($(COMPILE), local)
	ifeq ($(PLATFORM), linux)
		COMPILER_C_EXE=gcc
		COMPILER_CPP_EXE=g++
		STRIP_EXE=strip
		SYSROOT_OPTION=
	endif
	ifeq ($(PLATFORM), linuxarm)
		COMPILER_C_EXE=gcc
		COMPILER_CPP_EXE=g++
		STRIP_EXE=strip
		SYSROOT_OPTION=
	endif
	ifeq ($(PLATFORM), windows)
		COMPILER_C_EXE=gcc
		COMPILER_CPP_EXE=g++
		STRIP_EXE=strip
		SYSROOT_OPTION=
	endif
	ifeq ($(PLATFORM), macos)
		COMPILER_C_EXE=clang
		COMPILER_CPP_EXE=clang++
		COMPILER_OBJC_EXE=clang++
		STRIP_EXE=strip
		SYSROOT_OPTION=
	endif
endif
ifeq ($(COMPILE), cross)
	ifeq ($(PLATFORM), linux)
		COMPILER_C_EXE=$(XTOOLS_PATH)/x86-ubuntu1804/bin/x86_64-ubuntu18.04-linux-gnu-gcc
		COMPILER_CPP_EXE=$(XTOOLS_PATH)/x86-ubuntu1804/bin/x86_64-ubuntu18.04-linux-gnu-g++
		STRIP_EXE=$(XTOOLS_PATH)/x86-ubuntu1804/bin/x86_64-ubuntu18.04-linux-gnu-strip
		SYSROOT_OPTION=--sysroot=$(XTOOLS_PATH)/x86-ubuntu1804/x86_64-ubuntu18.04-linux-gnu/sysroot
	endif
	ifeq ($(PLATFORM), linuxarm)
		COMPILER_C_EXE=$(XTOOLS_PATH)/arm-ubuntu1604/bin/aarch64-ubuntu16.04-linux-gnu-gcc
		COMPILER_CPP_EXE=$(XTOOLS_PATH)/arm-ubuntu1604/bin/aarch64-ubuntu16.04-linux-gnu-g++
		STRIP_EXE=$(XTOOLS_PATH)/arm-ubuntu1604/bin/aarch64-ubuntu16.04-linux-gnu-strip
		SYSROOT_OPTION=--sysroot=$(XTOOLS_PATH)/arm-ubuntu1604/aarch64-ubuntu16.04-linux-gnu/sysroot
	endif
	ifeq ($(PLATFORM), windows)
		COMPILER_C_EXE=$(XTOOLS_PATH)/x86-windows/bin/x86_64-w64-mingw32-gcc
		COMPILER_CPP_EXE=$(XTOOLS_PATH)/x86-windows/bin/x86_64-w64-mingw32-g++
		STRIP_EXE=$(XTOOLS_PATH)/x86-windows/bin/x86_64-w64-mingw32-strip
		SYSROOT_OPTION=--sysroot=$(XTOOLS_PATH)/x86-windows/x86_64-w64-mingw32/sysroot
	endif
	ifeq ($(PLATFORM), macos)
		LINK_OPTION:=$(LINK_OPTION) -lgcc_s.10.5 -lc++
		PATH:=$(XTOOLS_PATH)/x86-macos1010/target/bin:$(XTOOLS_PATH)/x86-macos1010/llvm-8/bin:$(PATH)
		COMPILER_C_EXE=$(XTOOLS_PATH)/x86-macos1010/target/bin/x86_64-apple-darwin14-clang
		COMPILER_CPP_EXE=$(XTOOLS_PATH)/x86-macos1010/target/bin/x86_64-apple-darwin14-clang++
		COMPILER_OBJC_EXE=$(XTOOLS_PATH)/x86-macos1010/target/bin/x86_64-apple-darwin14-clang++
		STRIP_EXE=$(XTOOLS_PATH)/x86-macos1010/target/bin/x86_64-apple-darwin14-strip
		ifeq ($(IS_CYGWIN), yes)
			SYSROOT_OPTION=-isysroot $(CYGWIN_XTOOLS_PATH)/x86-macos1010/target/SDK/MacOSX10.10.sdk -cxx-isystem $(CYGWIN_XTOOLS_PATH)/x86-macos1010/target/SDK/MacOSX10.10.sdk/usr/include/c++/v1
		else
			SYSROOT_OPTION=-isysroot $(XTOOLS_PATH)/x86-macos1010/target/SDK/MacOSX10.10.sdk -cxx-isystem $(XTOOLS_PATH)/x86-macos1010/target/SDK/MacOSX10.10.sdk/usr/include/c++/v1
		endif
	endif
endif

# Windows GUI related
ifeq ($(PLATFORM), windows)
	ifeq ($(WINDOWS_GUI), yes)
		CFLAG:=$(CFLAG) -DSPADAS_WINDOWS_GUI
		LINK_OPTION:=$(LINK_OPTION) -mwindows -municode
	endif
endif

# Output lib and bin options
WL_OPTION=-Wl,-rpath,./
PRELIB_COMMAND=
POSTLIB_COMMAND=
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
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
		ifeq ($(MODE), release)
			WL_OPTION:=$(WL_OPTION),--out-implib,$(LIB_PATH)/$(LIB_NAME)
		endif
	endif
	ifeq ($(PLATFORM), macos)
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
	ifeq ($(PLATFORM), windows)
		WL_OPTION:=$(WL_OPTION),-rpath-link,$(LIB_PATH)/
	endif
endif

# strip command
ifeq ($(MODE), release)
	ifeq ($(PLATFORM), macos)
		STRIP_COMMAND=$(STRIP_EXE) -S -x $(OUT_PATH)/$(OUT_NAME)
	else
		STRIP_COMMAND=$(STRIP_EXE) $(OUT_PATH)/$(OUT_NAME)
	endif
endif
ifeq ($(MODE), debug)
	STRIP_COMMAND=
endif

# Charset options
CHARSET_OPTION=
ifeq ($(PLATFORM), windows)
	CHARSET_OPTION=-fexec-charset=GBK
endif

# Build process
all:pre $(OBJS)
	$(COMPILER_CPP_EXE) $(SYSROOT_OPTION) $(CHARSET_OPTION) $(LINK_OPTION) -L../$(LIB_DIR) $(WL_OPTION) $(LDFLAG) -o $(OUT_PATH)/$(OUT_NAME) $(OBJS) $(addprefix -l, $(DEPENDS)) $(addprefix -L, $(SEARCH_LIB)) $(addprefix -framework , $(FRAMEWORKS))
	$(STRIP_COMMAND)
	$(POSTLIB_COMMAND)
	$(POSTBUILD_COMMAND)
	@echo '---------------OK---------------'

$(OBJ_PATH)/%.o:%.c $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_C_EXE) -Wall $(SYSROOT_OPTION) $(CHARSET_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -fmessage-length=0 -fvisibility=hidden -fPIC $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.cpp $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_CPP_EXE) -Wall $(SYSROOT_OPTION) $(CHARSET_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -fmessage-length=0 -fvisibility=hidden -fPIC -std=c++11 $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.cc $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_CPP_EXE) -Wall $(SYSROOT_OPTION) $(CHARSET_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -fmessage-length=0 -fvisibility=hidden -fPIC -std=c++11 $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<

$(OBJ_PATH)/%.o:%.mm $(HEADERS_H) $(HEADERS_HPP)
	mkdir -p `dirname $@`
	$(COMPILER_OBJC_EXE) -Wall $(SYSROOT_OPTION) $(CHARSET_OPTION) -I. -I../include -I../$(INCLUDE_DIR) $(addprefix -I, $(SEARCH_INCLUDE)) -fmessage-length=0 -fvisibility=hidden -fPIC -std=c++11 $(CFLAG) -D$(ENV_DEFINE) -D$(ARCH_DEFINE) $(addprefix -D, $(MACRO_DEFINE)) -o $@ -c $<


pre:
	$(PREBUILD_COMMAND)
	$(PRELIB_COMMAND)
	mkdir -p $(OUT_PATH)

clean:
	rm -rf Debug Release