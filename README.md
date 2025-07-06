# Gendop
Document generator for making decisions on establishing the fact of borrowing

# Build
Install Qt5:
```
# apt install qt5 qt5-dev build-essential
$ cd gendop/build/template
$ chmod +x unzipf.sh
$ chmod +x zipf.sh
$ qmake gendop.pro
$ make
$ sudo make install
```

# Configuration
Change it to fit your position:
```
$ cd build/conf
$ nano teachers.conf
```
Format:
position class = [position name]

