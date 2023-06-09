# This makefile expects to be run from dng_sdk_1_6/dng_sdk/source

# Top level of the repo
ROOT        = ../../../..

include $(ROOT)/build/common.make

SOURCE_H    = .
OUR_H       = $(ROOT)/build
XMP_H       = $(ROOT)/sdk/$(DNGSDK)/xmp/toolkit/public/include
INCLUDES    = -I$(SOURCE_H) -I$(OUR_H) -I$(XMP_H)

CXXFLAGS   := $(CXXFLAGS) $(INCLUDES)
LDFLAGS    := $(LDFLAGS) $(ROOT)/sdk/lib/$(XMP_LIB) -lexpat

LIB_CPP_FILES := $(wildcard *.cpp)

LIB_OBJ_FILES := $(LIB_CPP_FILES:.cpp=.o)

# Custom build rule for this one source file, to get the -D
dng_validate.o: ./dng_validate.cpp
	$(CXX) $(CXXFLAGS) -DqDNGValidateTarget -c $^

$(DNG_LIB): $(LIB_OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $^

dng_validate: dng_validate.o $(DNG_LIB)
	$(CXX) $^ $(LDFLAGS) -o $@


clean:
	-rm -f dng_validate *.o $(DNG_LIB)

.PHONY: clean
