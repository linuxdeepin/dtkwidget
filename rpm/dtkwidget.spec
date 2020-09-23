Name:           dtkwidget
Version:        5.2.2.16
Release:        1%{?dist}
Summary:        Deepin tool kit widget modules
License:        LGPLv3+
%if 0%{?fedora}
URL:            https://github.com/linuxdeepin/dtkwidget
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz
%else
URL:            https://github.com/linuxdeepin/dtkwidget
Source0:        %{name}_%{version}.orig.tar.xz
%endif

BuildRequires:  gcc-c++
BuildRequires:  qt5-linguist
BuildRequires:  qt5-qtbase-static
BuildRequires:  dtkgui-devel
BuildRequires:  dtkcore-devel
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Concurrent)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Multimedia)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5X11Extras)
BuildRequires:  pkgconfig(dframeworkdbus)
BuildRequires:  pkgconfig(gsettings-qt)
BuildRequires:  pkgconfig(libudev)
BuildRequires:  pkgconfig(librsvg-2.0)
BuildRequires:  pkgconfig(libstartup-notification-1.0)
BuildRequires:  pkgconfig(xi)
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xext)
BuildRequires:  pkgconfig(xcb-util)
BuildRequires:  pkgconfig(xkbcommon)
BuildRequires:  pkgconfig(xrender)

# libQt5Gui.so.5(Qt_5.10.1_PRIVATE_API)(64bit) needed by dtkwidget-2.0.6.1-1.fc29.x86_64
BuildRequires:  qt5-qtbase-private-devel
%{?_qt5:Requires: %{_qt5}%{?_isa} = %{_qt5_version}}

%description
DtkWidget is Deepin graphical user interface for deepin desktop development.

%package devel
Summary:        Development package for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       dtkcore-devel%{?_isa}
Requires:       dtkgui-devel%{?_isa}

%description devel
Header files and libraries for %{name}.

%prep
%setup -q

%build
# help find (and prefer) qt5 utilities, e.g. qmake, lrelease
export PATH=%{_qt5_bindir}:$PATH
%qmake_qt5 PREFIX=%{_prefix} LIB_INSTALL_DIR=%{_libdir} DBUS_VERSION_0_4_2=YES
%make_build

%install
%make_install INSTALL_ROOT=%{buildroot}

%files
%doc README.md
%license LICENSE
%{_libdir}/lib%{name}.so.5*
%{_libdir}/libdtk-*/
%{_datadir}/libdtk-*/

%files devel
%{_includedir}/libdtk-*/
%{_qt5_archdatadir}/mkspecs/modules/*.pri
%{_libdir}/cmake/DtkWidget/
%{_libdir}/pkgconfig/%{name}.pc
%{_libdir}/lib%{name}.so

%changelog
* Thu Jun 09 2020 uoser <uoser@uniontech.com> - 5.2.2.3-1
- Update to 5.2.2.3
