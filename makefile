# Names
N_APP = scbl

# Files
F_SRC = \
	example/main.cc\
	example/app.cc\
	example/scbl.cc\
	example/utils.cc

F_HEADER = \
	scbl.hh\
	example/app.hh\
	example/utils.hh\
	example/types.hh\
	example/components.hh\
	example/platform.hh\
	example/config.hh

F_ALL = ${F_SRC} ${F_HEADER}

# Compiler related
CXX = g++
CXX_VER = c++17
CXX_FLAGS = \
	-O3\
	-Wall\
	-std=${CXX_VER}\
	-I./example\
	-I./

# Config
UTILS_USE_GNU_READLINE = false

ifeq (${OS}, Windows_NT)
	CREATE_BIN_DIRECTORY = if not exist "./bin" mkdir ${D_BIN}
	CLEAN = del ./bin/app.exe
	BINARY = ./bin/app.exe
else
	UNAME_S := $(shell uname -s)
	ifeq (${UNAME_S}, Linux)
		UTILS_USE_GNU_READLINE = true
	endif

	ifeq (${UTILS_USE_GNU_READLINE}, true)
		CXX_FLAGS += -lreadline
	endif

	UNAME_S := $(shell uname -s)

	CREATE_BIN_DIRECTORY = mkdir -p ./bin
	CLEAN = rm ./bin/app
	BINARY = ./bin/app
endif

CXX_FLAGS += -o ${BINARY}

compile: ${F_ALL}
	@${CREATE_BIN_DIRECTORY}
	@echo Created ./bin/

	@echo Compiling...
	@${CXX} ${F_SRC} ${CXX_FLAGS}
	@echo Compiled successfully

clean:
	@echo Cleaning...
	@${CLEAN}
	@echo Cleaned successfully

all:
	@echo compile - Compiles the source
	@echo clean - Removes built files
