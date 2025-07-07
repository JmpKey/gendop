# Gendop
Document generator for making decisions on establishing the fact of borrowing

# Build
Install Qt5 and begin:
```
# apt install qt5 qt5-dev build-essential
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

