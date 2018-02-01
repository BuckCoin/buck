#!/bin/bash

sudo apt -y update
sudo apt-get install -y libc6-dev g++-multilib python p7zip-full pwgen jq curl
cd ~

if [ -f buck.zip ]
then
    rm buck.zip
fi
wget -O buck.zip `curl -s 'https://api.github.com/repos/buckcoin/buck/releases/latest' | jq -r '.assets[].browser_download_url' | egrep "buck.+x64.zip"`
7z x -y buck.zip
chmod -R a+x ~/buck-pkg
rm buck.zip

cd ~/buck-pkg
./fetch-params.sh

if ! [[ -d ~/.buck ]]
then
    mkdir -p ~/.buck
fi

if ! [[ -f ~/.buck/buck.conf ]] 
then
    echo "rpcuser=rpc`pwgen 15 1`" > ~/.buck/buck.conf
    echo "rpcpassword=rpc`pwgen 15 1`" >> ~/.buck/buck.conf
fi

./zcashd