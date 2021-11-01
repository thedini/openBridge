## Introduction

CANview is software for exploring can inputs from a CAN translator. It is available from YachtDevices and is cross platform (kinda)

### CANview in Ubuntu

If you go to run CANview in a modern version of Ubuntu (in my case 20.10) you will find that it requires a old dependency

```
./CANView: error while loading shared libraries: libpng12.so.0: cannot open shared object file: No such file or directory
```
in order to fix this we will need to install libpng12, one can either compile from source or be able to install the .deb in Ubuntu/Debian.

### Install

1) Install Dependencies -- In order to compile you will need to do the following in install dependencies

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install libtool autoconf build-essential pkg-config automake tcsh
```

2) Download source

```
mkdir ~/CANview
cd ~/CANview
wget http://archive.ubuntu.com/ubuntu/pool/main/libp/libpng/libpng_1.2.54.orig.tar.xz
```

3) Extract Source

```
tar xvf  libpng_1.2.54.orig.tar.xz
```

4) Compile the code
```
cd libpng-1.2.54
./autogen.sh
./configure
make -j8
sudo make install
```

5) Update Links

```
sudo ldconfig
```

If all goes well you will have a nice old version of libpng installed!

### Running CANview in Linux

To run CANview simply download and unzip from Yacht Devices website:

```
cd ~/CANview
wget https://www.yachtd.com/downloads/CANVIEW.zip
unzip CANVIEW.zip
```

You will need to set the permissions prior to running:

```
cd Linux # change directory to where the Linux binary is
chmod u+x CANview
```
Finally you can run it:

```
./CANview
```

Lastly, you can add it to your local binary directory with so you can run from command line with `CANview`:

```
cp CANview ~/.local/bin/
```

You may need to add ~/.local/bin to your $PATH in `.bashrc` with:
```
echo 'export PATH="~/.local/bin:$PATH"' >> ~/.bashrc
```
