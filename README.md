!BUCK(https://i.imgur.com/RXp7QTz.png)

# Buck - The Future of Cryptocurrency

**Keep running wallet to strengthen the Buck network. Backup your wallet in many locations & keep your coins wallet offline.**

BUCK

Project site: https://Buck.red 

Block Explorer: https://Explorer.Buck.red 


- Type: Cryptocurrency
- Ticker: BUCK
- Algo: Equihash
- Max supply: 1 Billion coins 
- Target block creation rate: 1000 coins every 2.5 minutes
- Current block size is similar to BCC/BCH (BUCK = 2MB every 2.5 mins ~ BCC/BCH = 8MB every 10 min)
- Technology: ZeroCoin / Zcash

## Buck Principles: 
- Anonymous:
It is recommended to use an anonymous zaddr for all transactions. Public taddrs transactions are also allowed.
- All contributors are welcome

As a miner you should not use the biggest pools to follow main principles.
- Easy to mine:
Equihash algorithm.
You can use your Desktop PC to mine Buck. Most profitable is GPU mining.

## Roadmap
- Windows build - (COMPLETED)
- Windows GUI wallet - (COMPLETED)

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
addnode=159.89.40.161
addnode=139.59.111.53
addnode=45.55.195.233
addnode=138.68.95.165
addnode=18.216.125.20
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

### Docker

Build
```
$ docker build -t buck/buck .
```

Create a data directory on your local drive and create a buck.conf config file
```
$ mkdir -p /ops/volumes/buck/data
$ touch /ops/volumes/buck/data/buck.conf
$ chown -R 999:999 /ops/volumes/buck/data
```

Create buck.conf config file and run the application
```
$ docker run -d --name buck-node \
  -v buck.conf:/buck/data/buck.conf \
  -p 5749:5749 -p 127.0.0.1:5739:5739 \
  buck/buck
```

Verify buck-node is running
```
$ docker ps
CONTAINER ID        IMAGE                  COMMAND                  CREATED             STATUS              PORTS                                              NAMES
31868a91456d        buck/buck          "zcashd --datadir=..."   2 hours ago         Up 2 hours          127.0.0.1:5739->5739/tcp, 0.0.0.0:5749->5749/tcp   buck-node
```

Follow the logs
```
docker logs -f buck-node
```

The cli command is a wrapper to zcash-cli that works with an already running Docker container
```
docker exec -it buck-node cli help
```

## Using a Dockerfile
If you'd like to have a production btc/buck image with a pre-baked configuration
file, use of a Dockerfile is recommended:

```
FROM buck/buck
COPY buck.conf /buck/data/buck.conf
```

Then, build with `docker build -t my-buck .` and run.



Security Warnings
-----------------

**Buck is experimental and a work-in-progress.** Use at your own risk.
