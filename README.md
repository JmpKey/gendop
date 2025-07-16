# Gendop
Document generator for making decisions on establishing the fact of borrowing

# Build
1. Install Qt5:
Deb:
```
# apt install qtbase5-dev zip unzip
```
RPM:
```
dnf install qt5-qtbase-devel zip unzip
```
2. Begin:
```
$ cd gendop/build/template
$ chmod +x unzipf.sh
$ chmod +x zipf.sh
$ cd ../
$ qmake ../gendop.pro
$ make
$ make install INSTALL_ROOT=$HOME/gendop_app
$ cp -r conf db in template ~/gendop_app/opt/gendop/bin/
```

# Configuration
Change it to fit your position:
```
$ cd build/conf
$ nano teachers.conf
```
Format:
position class = [position name]

