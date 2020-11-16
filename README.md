EPIwm
=====

What is EPIwm ?
---------------

**EPIwm** is a window manager born out of a school project. 
It is intended to be small, fast, and configurable while maintening a large feature set. 

Installation :
--------------

### Known Supported Platforms

- Intel GNU/Linux Redhat 5.x 6.0, Mandrake and Debian (primary platform)
- Intel NetBSD 1.4 (primary platform)
- Sparc NetBSD 1.4
- Linux PPC
- Alpha Digital Unix v4.0 (OSF1)

And probably on another...

Patches to make it work on other platforms are welcome.

### Requirements

* X11R6
* make

### Optional

* Imlib
* libpng
* libtiff
* libxpm
* libgif
* libjpeg

### Manual installation

Uncompress the archive : 

```shell
gunzip -c epiwm-0.5.6.tar.gz | tar xvf -
```

Enter in the directory, configure it and build it : 

```shell
cd epiwm-0.5.6
./configure
make
```

If you want to disable the use of libXpm, add `--disable-xpm` to `./configure`.

If you want to disable the use of Imlib, add `--disable-imlib` to `./configure`.

You can specify where to install config files with the `--sysconfdir=[PATH]` option of configure.

See (`./configure --help`)

If you want to install EPIwm, you must be root and type: 

```shell
make install-strip
```

If it does not work, you can try : 

```shell
make -f Makefile.linux
make -f Makefile.linux install
```

### RPM Installation 

Be root and just type :

```shell
rpm -ivh EPIwm-0.5.6-1.i386.rpm
```

### DEB Installation

Be root and just type :

```shell
dpkg -i epiwm_0.5.6-1_i386.deb
```

Add this following line in your `~/.xinitrc` or your `~/.xsession` file :
	
```
exec epiwm
```

It's finished. You can launch EPIwm.


### Theme Installation


Decompress the theme in your `~/.epiwm` directory : 

```shell
tar xvfz EPIwm_be_theme.tar.gz
```

Now you have two directories : 

```shell
$ ls ~/.epiwm
$ be/ pixmaps/
```

Edit your `.xinitrc` file and add the line : 

```shell
exec epiwm -theme ~/.epiwm/be
```

Now it's OK. So you can have several theme directories in your `~/.epiwm` directory. And you can select the theme that you want with the -theme option of the epiwm
command.

Authors : 
---------

* [Mulot Julien](https://github.com/mulot)
* [Poindessous Thomas](https://github.com/tpoindessous)
* Rault Stephane

For more information go to https://github.com/mulot/epiwm
