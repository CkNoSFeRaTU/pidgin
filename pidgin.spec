# Older RPM doesn't define these by default
%{!?perl_vendorlib: %define perl_vendorlib %(eval "`%{__perl} -V:installvendorlib`"; echo $installvendorlib)}
%{!?perl_vendorarch: %define perl_vendorarch %(eval "`%{__perl} -V:installvendorarch`"; echo $installvendorarch)}
%{!?perl_archlib: %define perl_archlib %(eval "`%{__perl} -V:installarchlib`"; echo $installarchlib)}

# When not doing betas comment this out
# NOTE: %defines in spec files are evaluated in comments so the correct
#       way to comment it out is to replace the % with #
#define beta 7

%if 0%{?beta}
%define pidginver %(echo "2.14.12"|sed -e 's/dev.*//; s/beta.*//')
%else
%define pidginver 2.14.12
%endif

# define the minimum API version required, so we can use it for plugin deps
%define apiver %(echo "2.14.12"|awk -F. '{print $1"."$2}')

Summary:    A GTK+ based multiprotocol instant messaging client
Name:       pidgin
Version:    %pidginver
Release:    0%{?beta:.beta%{beta}}
License:    GPL
Group:      Applications/Internet
URL:        http://pidgin.im/
Source:     %{name}-2.14.12.tar.bz2
BuildRoot:  %{_tmppath}/%{name}-%{version}-root

# Generic build requirements
BuildRequires: libtool, pkgconfig, intltool, gettext, libxml2-devel
BuildRequires: gtk2-devel, libidn-devel

%{!?_without_startupnotification:BuildRequires: startup-notification-devel}
%{?_with_avahi:BuildRequires: avahi-glib-devel}
%{!?_without_gtkspell:BuildRequires: gtkspell-devel}
%{?_with_meanwhile:BuildRequires: meanwhile-devel}
%{?_with_mono:BuildRequires: mono-devel}
%{?_with_sasl:BuildRequires: cyrus-sasl-devel >= 2}
%{!?_without_silc:BuildRequires: /usr/include/silc/silcclient.h}
%{!?_without_tcl:BuildRequires: tcl, tk, /usr/include/tcl.h}
%{!?_without_text:BuildRequires: ncurses-devel}
%{!?_without_nm:BuildRequires: NetworkManager-devel}
%{!?_without_gevolution:BuildRequires: evolution-data-server-devel}

%if "%{_vendor}" == "suse"
# For SuSE:
BuildRequires: gnutls-devel
%define sslopts --enable-gnutls=yes --enable-nss=no
%{?_with_dbus:BuildRequires: dbus-1-devel >= 0.35}
%{!?_without_gstreamer:BuildRequires: gstreamer010-devel >= 0.10}
Requires(pre): gconf2
Requires(post): gconf2
Requires(preun): gconf2
%else
%define sslopts --enable-gnutls=no --enable-nss=yes
%{?_with_dbus:BuildRequires: dbus-devel >= 0.35}
%{!?_without_gstreamer:BuildRequires: gstreamer-devel >= 0.10}
Requires(pre): GConf2
Requires(post): GConf2
Requires(preun): GConf2
%endif

# Mandrake 10.1 and lower || Mandrake 10.2 (and higher?)
%if "%{_vendor}" == "MandrakeSoft" || "%{_vendor}" == "Mandrakesoft" || "%{_vendor}" == "Mandriva"
# For Mandrake/Mandriva:
BuildRequires: libnss3-devel, perl-devel
Obsoletes:  libgaim-remote0
%{!?_without_modularx:BuildRequires: libsm-devel, libxscrnsaver-devel}
%else
# For !Mandriva
%{!?_without_modularx:BuildRequires: libSM-devel, libXScrnSaver-devel}
# For SuSE, Red Hat, Fedora and others:
%if "%{_vendor}" != "suse"
# For Red Hat, Fedora and others:
# let's assume RH & FC1 are the only brain-dead distros missing the
# perl-XML-Parser dependency on intltool and that other RH/FC releases
# don't care if we specify it here
BuildRequires: perl-XML-Parser
BuildRequires: mozilla-nss-devel
%endif
%endif

# For some reason perl isn't always automatically detected as a requirement :(
Requires: perl

Requires: libpurple = %{version}

Obsoletes: gaim
Provides: gaim
Obsoletes: pidgin-perl < %{version}
Provides: pidgin-perl = %{version}-%{release}

%package devel
Summary:    Development headers, documentation, and libraries for Pidgin
Group:      Applications/Internet
Requires:   pidgin = %{version}, libpurple-devel = %{version}
Requires:   gtk2-devel
Requires:   pkgconfig
Obsoletes:  gaim-devel
Provides:   gaim-devel

%package -n libpurple
Summary:    libpurple library for IM clients like Pidgin and Finch
Group:      Applications/Internet
Obsoletes:  gaim-silc
Obsoletes:  gaim-tcl
Obsoletes:  gaim-gadugadu
Obsoletes:  pidgin-tcl < 2.0.0
Obsoletes:  pidgin-silc < 2.0.0
Obsoletes:  libpurple-perl < %{version}
Provides:   libpurple-perl = %{version}-%{release}
%{?_with_sasl:Requires:   cyrus-sasl-plain, cyrus-sasl-md5}

%package -n libpurple-devel
Summary:    Development headers, documentation, and libraries for libpurple
Group:      Applications/Internet
Requires:   libpurple = %{version}
Requires:   pkgconfig
%if "%{_vendor}" == "suse"
# For SuSE:
%{?_with_dbus:Requires: dbus-1-devel >= 0.35}
%else
%{?_with_dbus:Requires: dbus-devel >= 0.35}
%endif

%if 0%{?_with_avahi:1}
%package -n libpurple-bonjour
Summary:    Bonjour plugin for Pidgin
Group:      Applications/Internet
Requires:   libpurple >= %{apiver}
%endif

%if 0%{?_with_meanwhile:1}
%package -n libpurple-meanwhile
Summary:    Lotus Sametime plugin for Pidgin using the Meanwhile library
Group:      Applications/Internet
Requires:   libpurple >= %{apiver}
%endif

%if 0%{?_with_mono:1}
%package -n libpurple-mono
Summary:    Mono .NET plugin support for Pidgin
Group:      Applications/Internet
Requires:   libpurple >= %{apiver}
%endif

%if 0%{!?_without_text:1}
%package -n finch
Summary:    A text-based user interface for Pidgin
Group:      Applications/Internet
Requires:   libpurple = %{version}

%package -n finch-devel
Summary:    Headers etc. for finch stuffs
Group:      Applications/Internet
Requires:   finch = %{version}, libpurple-devel = %{version}
Requires:   ncurses-devel
Requires:   pkgconfig
%endif

%description
Pidgin allows you to talk to anyone using a variety of messaging
protocols including XMPP, Bonjour, Gadu-Gadu,
IRC, Novell Groupwise, QQ, Lotus Sametime, SILC, Simple and
Zephyr.  These protocols are implemented using a modular, easy to
use design.  To use a protocol, just add an account using the
account editor.

Pidgin supports many common features of other clients, as well as many
unique features, such as perl scripting, TCL scripting and C plugins.

%description devel
The pidgin-devel package contains the header files, developer
documentation, and libraries required for development of Pidgin scripts
and plugins.

%description -n libpurple
libpurple contains the core IM support for IM clients such as Pidgin
and Finch.

libpurple supports a variety of messaging protocols including
XMPP, Bonjour, Gadu-Gadu, IRC, Novell Groupwise, QQ,
Lotus Sametime, SILC, Simple and Zephyr.

%description -n libpurple-devel
The libpurple-devel package contains the header files, developer
documentation, and libraries required for development of libpurple based
instant messaging clients or plugins for any libpurple based client.

%if 0%{?_with_avahi:1}
%description -n libpurple-bonjour
Bonjour plugin for Pidgin.
%endif

%if 0%{?_with_meanwhile:1}
%description -n libpurple-meanwhile
Lotus Sametime plugin for Pidgin using the Meanwhile library.
%endif

%if 0%{?_with_mono:1}
%description -n libpurple-mono
Mono plugin loader for Pidgin.  This package will allow you to write or
use Pidgin plugins written in the .NET programming language.
%endif

%if 0%{!?_without_text:1}
%description -n finch
A text-based user interface for using libpurple.  This can be run from a
standard text console or from a terminal within X Windows.  It
uses ncurses and our homegrown gnt library for drawing windows
and text.

%description -n finch-devel
The finch-devel package contains the header files, developer
documentation, and libraries required for development of Finch scripts
and plugins.
%endif

%prep
%setup -q -n %{name}-2.14.12

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} \
                                    --bindir=%{_bindir} \
                                    --datadir=%{_datadir} \
                                    --includedir=%{_includedir} \
                                    --libdir=%{_libdir} \
                                    --mandir=%{_mandir} \
                                    --sysconfdir=%{_sysconfdir} \
                                    --disable-schemas-install \
                                    %{sslopts} \
                                    %{!?_with_vv:--disable-vv} \
                                    %{!?_with_dbus:--disable-dbus} \
                                    %{!?_with_avahi:--disable-avahi} \
                                    %{!?_with_meanwhile:--disable-meanwhile} \
                                    %{?_without_gstreamer:--disable-gstreamer} \
                                    %{?_without_gtkspell:--disable-gtkspell} \
                                    %{?_without_nm:--disable-nm} \
                                    %{!?_without_gevolution:--enable-gevolution} \
                                    %{?_with_mono:--enable-mono} \
                                    %{?_with_sasl:--enable-cyrus-sasl} \
                                    %{?_without_tcl:--disable-tcl} \
                                    %{?_without_text:--disable-consoleui} \
                                    %{?_with_trayiconcompat:--enable-trayicon-compat}

make %{?_smp_mflags} LIBTOOL=/usr/bin/libtool

%install
rm -rf %{buildroot}
make DESTDIR=$RPM_BUILD_ROOT LIBTOOL=/usr/bin/libtool install

# Delete files that we don't want to put in any of the RPMs
rm -f $RPM_BUILD_ROOT%{_libdir}/finch/*.la
rm -f $RPM_BUILD_ROOT%{_libdir}/gnt/*.la
rm -f $RPM_BUILD_ROOT%{_libdir}/pidgin/*.la
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/*.la
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/liboscar.so
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/libjabber.so
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/libymsg.so
rm -f $RPM_BUILD_ROOT%{_libdir}/*.la
rm -f $RPM_BUILD_ROOT%{perl_archlib}/perllocal.pod
find $RPM_BUILD_ROOT -type f -name '*.a' -exec rm -f {} ';'
find $RPM_BUILD_ROOT -type f -name .packlist -exec rm -f {} ';'
find $RPM_BUILD_ROOT -type f -name '*.bs' -empty -exec rm -f {} ';'

%if 0%{!?_with_avahi:1}
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/libbonjour.so
%endif

%if 0%{!?_with_meanwhile:1}
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/libsametime.so
%endif

%if 0%{!?_with_mono:1}
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/mono.so
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/*.dll
%endif

%if 0%{?_without_silc:1}
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/libsilcpurple.so
%endif

%if 0%{?_without_tcl:1}
rm -f $RPM_BUILD_ROOT%{_libdir}/purple-2/tcl.so
%endif

%if 0%{?_without_text:1}
rm -f $RPM_BUILD_ROOT%{_mandir}/man1/finch.*
rm -rf $RPM_BUILD_ROOT%{_bindir}/finch
%endif

%find_lang %{name}

find $RPM_BUILD_ROOT%{_libdir}/purple-2 -xtype f -print | \
        sed "s@^$RPM_BUILD_ROOT@@g" | \
        grep -v /libbonjour.so | \
        grep -v /libsametime.so | \
        grep -v /mono.so | \
        grep -v ".dll$" > %{name}-%{version}-purpleplugins

find $RPM_BUILD_ROOT%{_libdir}/pidgin -xtype f -print | \
        sed "s@^$RPM_BUILD_ROOT@@g" > %{name}-%{version}-pidginplugins

find $RPM_BUILD_ROOT%{_libdir}/finch -xtype f -print | \
        sed "s@^$RPM_BUILD_ROOT@@g" > %{name}-%{version}-finchplugins

# files -f file can only take one filename :(
cat %{name}.lang >> %{name}-%{version}-purpleplugins

%clean
rm -rf %{buildroot}

%pre
if [ "$1" -gt 1 -a -n "`which gconftool-2 2>/dev/null`" ]; then
    export GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source`
    if [ -f %{_sysconfdir}/gconf/schemas/purple.schemas ]; then
        gconftool-2 --makefile-uninstall-rule \
            %{_sysconfdir}/gconf/schemas/purple.schemas >/dev/null || :
        killall -HUP gconfd-2 &> /dev/null || :
    fi
fi

%post
if [ -n "`which gconftool-2 2>/dev/null`" ]; then
    export GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source`
    gconftool-2 --makefile-install-rule \
        %{_sysconfdir}/gconf/schemas/purple.schemas > /dev/null || :
    killall -HUP gconfd-2 &> /dev/null || :
fi
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
	%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor &> /dev/null || :
fi

%post -n libpurple -p /sbin/ldconfig

%post -n finch -p /sbin/ldconfig

%preun
if [ "$1" -eq 0 -a -n "`which gconftool-2 2>/dev/null`" ]; then
    export GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source`
    gconftool-2 --makefile-uninstall-rule \
      %{_sysconfdir}/gconf/schemas/purple.schemas > /dev/null || :
    killall -HUP gconfd-2 &> /dev/null || :
fi

%postun
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
	%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor &> /dev/null || :
fi

%postun -n libpurple -p /sbin/ldconfig

%postun -n finch -p /sbin/ldconfig

%files -f %{name}-%{version}-pidginplugins
%defattr(-, root, root)

%doc AUTHORS
%doc COPYING
%doc COPYRIGHT
%doc ChangeLog
%doc NEWS
%doc README
%doc README.hg
%doc doc/the_penguin.txt
%doc %{_mandir}/man1/pidgin.*
%doc %{_mandir}/man3*/*

%dir %{_libdir}/pidgin
%dir %{_libdir}/pidgin/perl
%dir %{_libdir}/pidgin/perl/auto
%dir %{_libdir}/pidgin/perl/auto/Pidgin
%{_bindir}/pidgin
%{_datadir}/pixmaps/pidgin
%{_datadir}/icons/hicolor/*/apps/pidgin.*
%{_datadir}/applications/*
%{_sysconfdir}/gconf/schemas/purple.schemas


%files -f %{name}-%{version}-purpleplugins -n libpurple
%defattr(-, root, root)

%{_libdir}/libpurple.so.*
%dir %{_libdir}/purple-2
%dir %{_libdir}/purple-2/perl
%dir %{_libdir}/purple-2/perl/auto
%dir %{_libdir}/purple-2/perl/auto/Purple
%{_datadir}/purple
%{_datadir}/sounds/purple

%if 0%{?_with_dbus:1}
%{_bindir}/purple-client-example
%{_bindir}/purple-remote
%{_bindir}/purple-send
%{_bindir}/purple-send-async
%{_bindir}/purple-url-handler
%{_libdir}/libpurple-client.so.*
%doc libpurple/purple-notifications-example
%endif

%files devel
%defattr(-, root, root)
%dir %{_includedir}/pidgin
%{_includedir}/pidgin/*.h
%{_libdir}/pkgconfig/pidgin.pc

%files -n libpurple-devel
%defattr(-, root, root)

%doc ChangeLog.API
%doc HACKING
%doc PLUGIN_HOWTO

%dir %{_includedir}/libpurple
%{_includedir}/libpurple/*
%{_libdir}/libpurple.so
%{_libdir}/pkgconfig/purple.pc
%{_datadir}/aclocal/purple.m4
%if 0%{?_with_dbus:1}
%{_libdir}/libpurple-client.so
%endif


%if 0%{?_with_avahi:1}
%files -n libpurple-bonjour
%defattr(-, root, root)

%{_libdir}/purple-2/libbonjour.*
%endif

%if 0%{?_with_meanwhile:1}
%files -n libpurple-meanwhile
%defattr(-, root, root)

%{_libdir}/purple-2/libsametime.*
%endif

%if 0%{?_with_mono:1}
%files -n libpurple-mono
%defattr(-, root, root)

%{_libdir}/purple-2/mono.so
%{_libdir}/purple-2/*.dll
%endif

%if 0%{!?_without_text:1}
%files -f %{name}-%{version}-finchplugins -n finch
%defattr(-, root, root)

%doc %{_mandir}/man1/finch.*
%{_bindir}/finch

%files -n finch-devel
%defattr(-, root, root)
%dir %{_includedir}/finch
%{_includedir}/finch/*.h
%{_libdir}/pkgconfig/finch.pc

%endif

%changelog
* Wed Sep 01 2010 Stu Tomlinson <stu@nosnilmot.com>
- Ensure predictable use of SSL libs

* Wed Jun 02 2010 Stu Tomlinson <stu@nosnilmot.com>
- add an option to build RPMs using --enable-trayicon-compat
  (--with trayiconcompat)

* Thu May 13 2010 Stu Tomlinson <stu@nosnilmot.com>
- Include all libpurple headers in libpurple-devel

* Sat Sep 05 2009 Stu Tomlinson <stu@nosnilmot.com>
- Disable Voice & Video unless --with vv is used
- Add BuildRequires for libidn-devel
- Add Provides/Obsoletes to ease transition from Red Hat / Fedora RPMs

* Sat Jul 11 2009 Stu Tomlinson <stu@nosnilmot.com>
- Update to reflect changes in perl module installation directories

* Mon May 19 2008 Stu Tomlinson <stu@nosnilmot.com>
- Fix building without meanwhile support

* Fri May 16 2008 Stu Tomlinson <stu@nosnilmot.com>
- Add "--without nm" support to build without NetworkManager

* Thu Feb 28 2008 Stu Tomlinson <stu@nosnilmot.com>
- Remove --with-howl options as we no longer support using howl for bonjour

* Wed Dec  5 2007 Stu Tomlinson <stu@nosnilmot.com>
- When building with avahi, use native avahi instead of howl compatability
  headers
- Make the split out plugins depend only on the minimum necessary API
  version of libpurple

* Tue Oct 23 2007 Stu Tomlinson <stu@nosnilmot.com>
- Add finch.pc to finch-devel

* Mon Sep 17 2007 Stu Tomlinson <stu@nosnilmot.com>
- Add version dependency on libpurple for pidgin
- Support for OpenSuse lowercase package name for GConf2

* Fri Sep 14 2007 Stu Tomlinson <stu@nosnilmot.com>
- Fix spec file for moved sounds & new CA certificates

* Thu Jul 12 2007 Stu Tomlinson <stu@nosnilmot.com>
- Don't hardcode silc header locations, rely on pkg-config for those,
  because I think I broke non-pkg-config detection of older silc
  toolkit.

* Tue Jun 5 2007 Stu Tomlinson <stu@nosnilmot.com>
- Add missing Requires for gtk2-devel, dbus-devel & ncurses-devel to
  appropriate -devel subpackages

* Sun May 27 2007 Stu Tomlinson <stu@nosnilmot.com>
- add cyrus-sasl-plain & cyrus-sasl-md5 to Requires

* Thu May 24 2007 Stu Tomlinson <stu@nosnilmot.com>
- Silence errors from gtk-update-icon-cache
- Change Mandriva build dependencies to reflect the correct (lower case)
  names for libSM-devel & libXScrnSaver-devel (Sunny Dubey)

* Thu May 10 2007 Stu Tomlinson <stu@nosnilmot.com>
- Add scriptlet Requires for GConf2 to fix schema installation
- Silence harmless errors when gconfd-2 is not running at install time

* Thu May 3 2007 Stu Tomlinson <stu@nosnilmot.com>
- Add missing BuildRequires: startup-notification-devel, if you really
  need to build on a distro without it use --without startupnotification
- Add BuildRequires: libSM-devel, libXScrnSaver-devel for distros with
  modular X. For those without, build with --without modularx
- Change Mandriva BuildRequires to gkt2-devel (reported by Götz Waschk)

* Tue May 1 2007 Stu Tomlinson <stu@nosnilmot.com>
- Run gtk-update-icon-cache on installation/uninstallation
- Guard against errors when upgrading from Gaim/Pidgin 1.5.x which had
  no schemas file

* Sun Apr 29 2007 Stu Tomlinson <stu@nosnilmot.com>
- Update to reflect perl Purple::GtkUI -> Pidgin change

* Wed Apr 25 2007 Stu Tomlinson <stu@nosnilmot.com>
- Update libpurple to pick up plugins in %%{_libdir}/purple

* Sun Apr 22 2007 Stu Tomlinson <stu@nosnilmot.com>
- Remove Epoch because it's gone in Fedora now
- Add virtual provides for gaim & gaim-devel

* Thu Apr 19 2007 Stu Tomlinson <stu@nosnilmot.com>
- Fix pkgconfig Requires
- Add ldconfig in pre/post scripts for libpurple & finch
- Bump Epoch to 2 because Fedora unfortunately forgot to drop the Epoch
  during the rename :(

* Tue Apr 17 2007 Stu Tomlinson <stu@nosnilmot.com>
- Add some Obsolete:s to help upgrades
- Remove explicit Packager: %%{packager} from spec, it was redundant

* Sun Apr 15 2007 Stu Tomlinson <stu@nosnilmot.com>
- Fix for building without Finch
- Drop -tcl & -silc sub-packages, include them in the main libpurple
  package (--without tcl and --without silc can be used to build libpurple
  without support for these).

* Thu Apr 12 2007 Stu Tomlinson <stu@nosnilmot.com>
- Actually move libpurple.so.* to the libpurple RPM

* Wed Apr 11 2007 Stu Tomlinson <stu@nosnilmot.com>
- Separate out libpurple, libpurple-devel and finch-devel

* Sat Mar 24 2007 Stu Tomlinson <stu@nosnilmot.com>
- Update to reflect renaming to pidgin/libpurple

* Sun Oct  1 2006 Stu Tomlinson <stu@nosnilmot.com>
- We can build with internal gadu gadu again, so bring it back into the
  main package
- Deal with gconf schame uninstallation on package upgrade and removal

* Sun Aug 20 2006 Stu Tomlinson <stu@nosnilmot.com>
- Make the gstreamer-devel dependency overridable with '--without-gstreamer'
  to allow building on older distributions without suitable gstreamer

* Tue Aug 15 2006 Mark Doliner <thekingant@users.sourceforge.net>
- Add a BuildRequire for gstreamer-devel
- Remove the BuildRequires for audiofile-devel and libao-devel

* Mon May 8 2006 Mark Doliner <thekingant@users.sourceforge.net>
- Add --with avahi option to compile the gaim-bonjour package against
  Avahi's Howl compatibility layer

* Wed Mar 29 2006 Stu Tomlinson <stu@nosnilmot.com>
- Source RPM uses tar.bz2 now to save space
- Update BuildRequires for new intltool dependencies
- Add a --with perlmakehack option to allow builds to succeed on RH9
- Add a --with gadugadu to build (separate) gaim-gadugadu package

* Sat Dec 17 2005 Stu Tomlinson <stu@nosnilmot.com>
- Add support for beta versions so the subsequent releases are seen as newer
  by RPM
- Split of sametime support to gaim-meanwhile
- Use make DESTDIR=... instead of overloading prefix etc. when installing
- Default build to include cyrus-sasl support in Jabber
- Add --with dbus to build with DBUS support

* Sun Dec 04 2005 Christopher O'Brien <siege@preoccupied.net>
- Added obsoletes gaim-meanwhile

* Sun Oct 30 2005 Stu Tomlinson <stu@nosnilmot.com>
- Add separate gaim-bonjour package if built with --with-howl
- Add separate gaim-mono package if built with --with-mono
- Exclude some unwanted perl files

* Sat Aug 20 2005 Stu Tomlinson <stu@nosnilmot.com>
- Include libgaimperl.so
- Include gaim.m4 in gaim-devel

* Thu Apr 28 2005 Stu Tomlinson <stu@nosnilmot.com>
- Use perl_vendorlib & perl_archlib for better 64bit compat (Jeff Mahoney)
- Clean up Requires, most should be auto-detected
- Restore gtkspell-devel build requirement (and add --without gtkspell option)
- Fix Tcl build requirements to work across more distros
- Fix SILC build requirements to work across more distros

* Mon Oct 11 2004 John Jolly <john.jolly@gmail.com>
- Added if "%%{_vendor}" == "suse" to handle GnuTLS libraries for SuSE

* Sat Oct  2 2004 Stu Tomlinson <stu@nosnilmot.com>
- If --with tcl or silc are not specified, make sure the plugins don't
  exist to prevent RPM complaining about unpackaged files

* Tue Jun 29 2004 Ethan Blanton <eblanton@cs.ohiou.edu>
- Change Tcl to use --with tcl, the same as SILC, and build a gaim-tcl
  package if specified.

* Thu Jun 24 2004 Mark Doliner <thekingant@users.sourceforge.net>
- Add --with silc rebuild option for compiling a separate gaim-silc
  RPM containing the silc protocol plugin (Stu Tomlinson).

* Wed Jun 23 2004 Ethan Blanton <eblanton@cs.ohiou.edu>
- Moved gaim headers and a pkgconfig configuration file into the
  gaim-devel RPM (Stu Tomlinson).

* Thu Jan 15 2004 Ethan Blanton <eblanton@cs.ohiou.edu>
- Removed the manual strip command, as it seems to be unwarranted if
  the necessary programs are properly installed.  (For me, this was
  elfutils.)

* Sun Jul 20 2003 Bjoern Voigt <bjoern@cs.tu-berlin.de>
- Added pkgconfig build dependency.
- if "%%{_vendor}" != "MandrakeSoft" now also works with rpm 3.x.
- Added Gaim-specific directories to list of Gaim's files.

* Wed Jul 16 2003 Ethan Blanton <eblanton@cs.ohiou.edu>
- Complete spec file rewrite to take advantage of "new" RPM features
  and make things prettier.
- Use system-supplied %%{_prefix}, %%{_datadir}, etc. rather than
  attempt to define our own.
