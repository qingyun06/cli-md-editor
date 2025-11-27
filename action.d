action.o gcm.cache/action.gcm: action.cc
action.c++m: gcm.cache/action.gcm
.PHONY: action.c++m
gcm.cache/action.gcm:| action.o
