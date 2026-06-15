#!/bin.bash
curl -Z -OLS https://raw.githubusercontent.com/polfosol/micro-AES/refs/heads/master/micro_aes.c \
    -OLS https://raw.githubusercontent.com/polfosol/micro-AES/refs/heads/master/micro_aes.h \
    -OLS https://raw.githubusercontent.com/LoupVaillant/Monocypher/refs/heads/master/src/monocypher.c \
    -OLS https://raw.githubusercontent.com/LoupVaillant/Monocypher/refs/heads/master/src/monocypher.c

echo "You just gonna have to install libsodium through a package manager or their website https://doc.libsodium.org/doc/installation"