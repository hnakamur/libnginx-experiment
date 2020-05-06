libnginx-experiment
===================

Build and install libnginx.

```console
git clone https://github.com/hnakamur/nginx
cd nginx
git checkout libnginx
./auto/configure --with-debug --prefix=/usr/local --libnginx
make
sudo make install
```

Build and run an example program using libnginx.

```console
cd ..
git clone https://github.com/hnakamur/libnginx-experiment
cd libnginx-experiment
make
```

```console
mkdir logs
./main
```
