#dmalloc instruction

#console
_> make
_> eval `dmalloc -d 0 -l file.log -p log-non-free`
_> LD_PRELOAD="libdmalloc.so" ./leak

#see log
