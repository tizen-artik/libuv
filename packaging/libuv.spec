Name:           libuv
Version:        1.7.0
Release:        1
License:        BSD-2-Clause-FreeBSD and BSD-3-Clause and ISC
Summary:        Multi-platform support asynchronous I/O library
Group:          System/Libraries
Source0:        %{name}-%{version}.tar.gz
Source1:        %{name}.pc.in
Source2:        %{name}.manifest
BuildRequires:  autoconf >= 2.59
BuildRequires:  automake
BuildRequires:  libtool

%description
Libuv is a multi-platform support library with a focus on asynchronous I/O. It was primarily developed for use by Node.js, but it's also used by Luvit, Julia, pyuv, and others.

%package devel
Summary: Development libraries for libuv
Group: Development/Tools
Requires: %{name} = %{epoch}:%{version}-%{release}
Requires: pkgconfig
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description devel
Development libraries for libuv

%prep
%setup -q

%build
sh autogen.sh

%configure
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install

mkdir -p %{buildroot}/%{_libdir}/pkgconfig
sed -e "s#@prefix@#%{_prefix}#g" \
    -e "s#@exec_prefix@#%{_exec_prefix}#g" \
    -e "s#@libdir@#%{_libdir}#g" \
    -e "s#@includedir@#%{_includedir}#g" \
    -e "s#@version@#%{version}#g" \
    %SOURCE1 > %{buildroot}/%{_libdir}/pkgconfig/libuv.pc

mkdir -p %{buildroot}%{_datadir}/license
cp LICENSE %{buildroot}%{_datadir}/license/%{name}

%clean
rm -rf %{buildroot}

%check
# make check

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig


%files
%{_datadir}/license/%{name}
%defattr(-,root,root)
%{_libdir}/*.so
%{_libdir}/*.so.*

%files devel
%{_includedir}/*.h
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/pkgconfig/*.pc
