Summary: EPIwm is a Window Manager
Name: EPIwm
Version: 0.5.6
Release: 1
Copyright: GPL
Group: X11/Window Managers
URL: https://github.com/mulot/epiwm
Source0: epiwm-0.5.6.tar.gz
BuildRoot: /tmp/EPIwm

%description
EPIwm is a window manager which is intended to be small, fast, configurable while maintening a large feature set. 

%prep
%setup -n epiwm-0.5.6

%build
./configure --prefix=/usr/X11R6 --sysconfdir=/etc/X11
make

%install
rm -rf $RPM_BUILD_ROOT
make install-strip DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT

%files
%doc README ChangeLog BUGS INSTALL TODO NEWS
#%config /etc/X11/epiwm/*
%config /etc/X11/epiwm/icons
%config /etc/X11/epiwm/key 
%config /etc/X11/epiwm/menu
%config /etc/X11/epiwm/start
%config /etc/X11/epiwm/style
%config /etc/X11/epiwm/window
%config /etc/X11/epiwm/workspace

/usr/X11R6/bin/epiwm
/usr/X11R6/bin/epiwm.inst
/usr/X11R6/bin/logoepiwm



