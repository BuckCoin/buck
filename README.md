![BUCK](https://i.imgur.com/RXp7QTz.png)

# Buck 2.0.7-1 - The Future of Cryptocurrency

**Keep running wallet to strengthen the Buck network. Backup your wallet in many locations & keep your coins wallet offline.**

BUCK

Project site: https://Buck.red 

Block Explorer: https://Explorer.Buck.red 


- Type: Cryptocurrency
- Ticker: BUCK
- Algo: Equihash
- Max supply: 1 Billion coins 
- Target block creation rate: 1000 coins every 2.5 minutes
- Current block size: 2MB every 2.5 mins
- Technology: Zcash

## Buck Principles: 
- All contributors are welcome

As a miner you should not use the biggest pools to follow main principles.
- Easy to mine:
Equihash algorithm.

## Roadmap 
- Linux build - (COMPLETED)
- Linux CLI binaries - (COMPLETED)
- Windows build - (COMPLETED)
- Windows CLI binaries - (COMPLETED)
- Windows GUI wallet - (COMPLETED)
- Mac build - (COMPLETED)
- Mac CLI binaries - (COMPLETED)
- Mac GUI Wallet - (COMPLETED)
- Linux ARM CLI binaries - (COMPLETED)

### Ports:
- RPC port: 5739
- P2P port: 5749

Install
-----------------
### Linux


Get dependencies
```{r, engine='bash'}
sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake curl
```

Install
```{r, engine='bash'}
# First, install prerequisites listed above.
# Download the github repository
git clone https://github.com/BuckCoin/buck.git
# Depending on your setup you may need to modify permissions (OPTIONAL)
chmod -R +x buck/  
# Change directories to the newly created buck directory
cd buck
# Build the Buck application
./zcutil/build.sh -j$(nproc)
# Fetch keys necessary for Buck 
./zcutil/fetch-params.sh
# Change to one folder lower (your user home folder)
cd ..
# Create a folder named .buck
mkdir .buck
# Change to the .buck directory
cd .buck
# Create a new file named buck.conf
nano buck.conf
# Insert the following into the new buck.conf file 
addnode=165.227.123.242
addnode=202.181.27.3
addnode=136.33.111.57
addnode=34.83.236.138
addnode=104.199.207.36
addnode=96.30.196.44
addnode=136.33.111.57
addnode=84.234.52.190
addnode=76.110.75.137
addnode=52.14.3.137
addnode=159.89.40.161
addnode=45.55.35.219
# To save this new file, hit CTRL-X, then Y to save new file.
# Change back to your home folder
cd ..
# Start Buck
~/buck/src/zcashd
# The node will download blocks. You are now running a Buck node and wallet!

# Optional
# If you want to run Buck in the background, start buck with the -daemon flag:
~/buck/src/zcashd -daemon
# To stop Buck safely if it is running in daemon, close the application with zcash-cli stop
zcash-cli stop
```


Security Warnings
-----------------

**Buck is experimental and a work-in-progress.** Use at your own risk.
