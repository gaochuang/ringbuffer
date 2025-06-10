Name:           ringbuffer
Version:        0.1.0
Release:        1%{?dist}
Summary:        A simple implementation of a ring buffer

License:       MIT
URL:           https://github.com/gaochuang/ringbuffer
Source0:       ringbuffer-%{version}.tar.gz


%description
%{name} provide ring buffer

%prep
%setup -q -n %{name}-%{version}

%build
make

%install
make install DESTDIR=%{buildroot}

%files
%{_includedir}/ringbuffer.h
%{_libdir}/libringbuffer.a
%{_libdir}/libringbuffer.so
