uname_S := $(shell uname -s)
DEBUG = false

ifeq ($(uname_S), Darwin)
	CC = gcc-10
else
	CC = gcc
endif

SRC_DIR = src/
TARGETS_DIR = target/
OBJECTS_DIR = bin/
EXECUTABLES_DIR = app/


CFLAGSDEB = -g -Wall -Og -lm -fopenmp -pg
LFLAGSDEB = -g -Wall -Og -lm -c -fopenmp -pg

CFLAGSPROD = -march=native -O3 -lm -fopenmp
LFLAGSPROD = -march=native -O3 -lm -c -fopenmp

ifeq ($(DEBUG), true)
	CFLAGS = $(CFLAGSDEB)
	LFLAGS = $(LFLAGSDEB)
else
	CFLAGS = $(CFLAGSPROD)
	LFLAGS = $(LFLAGSPROD)
endif

DEPS = \
	$(SRC_DIR)graphContraction.h \
	$(SRC_DIR)graph.h \
	$(SRC_DIR)utils.h

OBJECTS = \
	$(OBJECTS_DIR)graphContraction.o \
	$(OBJECTS_DIR)graph.o \
	$(OBJECTS_DIR)utils.o

OBJECT_TGS = \
	graphContraction.o \
	graph.o \
	utils.o

EXECUTABLE_TGS = \
	convertFromIGraph \
	convertToIGraph \
	doGraphContraction \
	genErdosRenyi

DEF_FILENAME = simpleSample

default: help

all: $(EXECUTABLE_TGS)                                                  ## Compiles all the executables from the executable list

objects: $(OBJECTS)                                                     ## Compiles all the objects from the object list

doGraphContraction: $(TARGETS_DIR)doGraphContraction.c $(OBJECT_TGS)    ## Compiles the target "doGraphContraction"
	$(CC) $(CFLAGS) -o $(EXECUTABLES_DIR)$@.app $< $(OBJECTS)

genErdosRenyi: $(TARGETS_DIR)genErdosRenyi.c $(OBJECT_TGS)              ## Compiles the target "genErdosRenyi"
	$(CC) $(CFLAGS) -o $(EXECUTABLES_DIR)$@.app $< $(OBJECTS)

convertFromIGraph: $(TARGETS_DIR)convertFromIGraph.c $(OBJECT_TGS)      ## Compiles the target "convertFromIGraph"
	$(CC) $(CFLAGS) -o $(EXECUTABLES_DIR)$@.app $< $(OBJECTS)

convertToIGraph: $(TARGETS_DIR)convertToIGraph.c $(OBJECT_TGS)          ## Compiles the target "convertToIGraph"
	$(CC) $(CFLAGS) -o $(EXECUTABLES_DIR)$@.app $< $(OBJECTS)

%.o: $(SRC_DIR)%.c $(DEPS)
	$(CC) $(LFLAGS) -o $(OBJECTS_DIR)$@ $<

testContraction:                                                         ## Builds and execute doGraphContraction on the default graph
	make doGraphContraction
	./app/doGraphContraction.app -v -p $(DEF_FILENAME)

clean:                                                                   ## Removes all the compiled sources
	rm -rf app/
	rm -rf bin/
	mkdir app
	mkdir bin
	touch app/.gitkeep
	touch bin/.gitkeep


data-cleanup:                                                            ## Removes all the outputs
	touch results/remove.txt
	touch results/generated/remove.txt
	rm results/*.txt
	rm results/generated/*.txt

install:                                                                 ## Configure the repository
	touch config/serial.number
	rm config/serial.number
	echo 0 > config/serial.number

help:                                                                    ## Shows this help
	@echo ' '
	@echo 'Usage: make <command>'
	@echo '       ./app/<target> [-?]'
	@echo ' '
	@echo 'Command List:'
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
	@echo ' '
	@echo 'Variable list:'
	@echo ' - Compiler:          $(CC)'
	@echo ' - Debug:             $(DEBUG)'
	@echo ' - Application Flags: $(CFLAGS)'
	@echo ' - Library Flags:     $(LFLAGS)'
	@echo ' - Library Files:     $(DEPS)'
	@echo ' - Object Files:      $(OBJECTS)'
	@echo ' - Target List:       $(EXECUTABLE_TGS)'
	@echo ' - Default test:      $(DEF_FILENAME)'
	@echo ' '
	@echo ' '
	@echo 'Library by Elia Onofri and Flavio Lombardi'
	@echo '{eonofri, flombardi}@uniroma3.it'
	@echo 'Version 1.0 -- 18/08/2023'
	@echo ' '
	@echo ' '


