#!/bin/bash -ev

mkdir -p ~/.turbostake
echo "rpcuser=username" >>~/.turbostake/turbostake.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.turbostake/turbostake.conf

