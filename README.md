# Gendop
Document generator for making decisions on establishing the fact of borrowing

# Build
1. Install if not zip on Deb:
```
# apt install zip unzip
```
Or RPM:
```
# dnf install zip unzip
```
2. Install Qt5:
Deb:
```
# apt install qt5 qt5-dev build-essential
```
RPM:
```
dnf install qt5 qt5-dev
dnf groupinstall "Development Tools"
```
3. Begin:
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

