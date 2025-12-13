mode.o gcm.cache/mode.gcm: mode.cc
mode.c++m: gcm.cache/mode.gcm
.PHONY: mode.c++m
gcm.cache/mode.gcm:| mode.o
