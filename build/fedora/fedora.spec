# ananas project
# Spec file for building single rpm package of Ananas for Fedora Linux
# $Id: fedora.spec,v 1.1 2008/11/27 20:20:54 leader Exp $
# $Name:  $
#

%define ShortName ananas
%define _qtdir %(echo $QTDIR)

Name: ananas
Version: 0.9.5
Release: %( echo f`grep -o "[0-9|\.]\+" /etc/redhat-release`)
Buildroot: %{_tmppath}/%{name}-%{version}-root

Summary: Runtime and development environment of Finance Applications.
Summary(ru_RU.UTF8):	Среда разработки и исполнения прикладных решений автоматизации оперативного, бухгалтерского и других видов учета.

License: GPL
Url: http://ananas.lrn.ru/
Vendor:	Project Ananas
Group: Applications/Productivity

Source:	%name-%version.tar.bz2

Requires: mysql-server >= 4.1
Requires: mysql >= 4.1
Requires: qt >= 3.2.0
Requires: qt-qsa >= 1.1
Requires: bluecurve.so
Requires: zip
Requires: unzip


BuildRequires:  gcc-c++ libstdc++-devel
BuildRequires:  mysql-devel
BuildRequires:  qt-devel
BuildRequires:  qt-qsa-devel


%description -l ru_RU.UTF8
Платформа Ананас предназначена для автомматизации малых и средних предприятий.
Обеспечивает автоматизацию ведения оперативного и бухгалтерского учета.
Платформа позволяет создавать приложения, поддерживающие многопользовательский
режим работы, сопровождается документацией на русском языке
имеет русифицированный графический интерфейс пользователя. Для ее
использования необходимо иметь установленную систему X Window System,
графическую среду KDE и сервер баз данных с поддержкой транзакций MySQL.
Пакет ananas необходимо установить, если вы собираетесь использовать
прикладное решение на платформе Ананас.

Разработчики
---------------
Валерий Гражданкин <vg at leaderit dot ru>
Андрей Паскаль <app at lrn dot ru>
Андрей Стрельников <avsone at yandex dot ru>
Павел Миколайчук <Pashik at bk dot ru>
Александр Ковалёв <cibfx at bk dot ru>
Григорий Панов <gr1313 at mail dot ru>

%description
Ananas is  an Accounting Application Framework for Small Businesses.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>
Andrey Paskal <app at lrn dot ru>
Andrey Strelnikov <avsone at yandex dot ru>
Pavel Mikolaychuk <Pashik at bk dot ru>
Alexander Kovalyov <cibfx at bk dot ru>
Grigory Panov <gr1313 at mail dot ru>


%prep
%setup

%build
export PATH=${PATH}:$QTDIR/bin
./configure
make
make tr

%install
export PATH=${PATH}:$QTDIR/bin


rm -f `find src doc -name Makefile`
qmake src/src.pro -o src/Makefile
cd src && make install \
	INSTALL_ROOT="$RPM_BUILD_ROOT" \
	LIBDIR=%_libdir BINDIR=%_bindir \
	INCLUDEDIR=%_includedir/ananas && \
	cd ../doc && qmake doc.pro -o Makefile && make install INSTALL_ROOT="$RPM_BUILD_ROOT" DOCDIR=%_docdir/%name-%version

mkdir -p %buildroot%_sysconfdir/ananas
mkdir -p %buildroot%_datadir/ananas/translations
mkdir -p %buildroot%_datadir/ananas/templates
mkdir -p %buildroot%_datadir/ananas/inventory
mkdir -p %buildroot%_datadir/applications/kde
mkdir -p %buildroot%_datadir/pixmaps
mkdir -p %buildroot/usr/lib/menu

#clear debug info
strip %buildroot%_bindir/ananas
strip %buildroot%_bindir/ananas-designer
strip %buildroot%_libdir/libananas.so*
strip %buildroot%_libdir/ananas/qt3plugins/designer/libananasplugin.so
strip %buildroot%_libdir/ananas/qt3plugins/sqldrivers/libqsqlmysqlu.so
strip %buildroot%_libdir/ananas/*.so

mv %buildroot%_libdir/ananas/qt3plugins/designer/libananasplugin.so %buildroot%_libdir/
ln -fs  ../../../libananasplugin.so  %buildroot%_libdir/ananas/qt3plugins/designer/libananasplugin.so

cp -f %_builddir/%name-%version/translations/*.qm %buildroot%_datadir/ananas/translations
cp -f %_builddir/%name-%version/src/designer/templates/*.* %buildroot%_datadir/ananas/templates

cp -f %_builddir/%name-%version/applications/inventory/inventory.cfg %buildroot%_sysconfdir/ananas/
cp -f %_builddir/%name-%version/applications/inventory/inventory.rc %buildroot%_sysconfdir/ananas/
cp -f %_builddir/%name-%version/applications/inventory/inventory.sql %buildroot%_datadir/ananas/inventory
cp -f %_builddir/%name-%version/applications/inventory/inventory-demo.cfg %buildroot%_sysconfdir/ananas/
cp -f %_builddir/%name-%version/applications/inventory/inventory-demo.rc %buildroot%_sysconfdir/ananas/
cp -f %_builddir/%name-%version/applications/inventory/inventory-demo.sql %buildroot%_datadir/ananas/inventory
cp -f %_builddir/%name-%version/applications/inventory/inventory_pgsql.sql %buildroot%_datadir/ananas/inventory
cp -f %_builddir/%name-%version/applications/inventory/inventory_grouprc %buildroot%_sysconfdir/ananas
cp -f %_builddir/%name-%version/applications/inventory/templ_*.odt %buildroot%_datadir/ananas/inventory
cp -f %_builddir/%name-%version/applications/inventory/templ_*.ods %buildroot%_datadir/ananas/inventory
cp -f %_builddir/%name-%version/build/kde/images/*.png %buildroot%_datadir/pixmaps/
cp -f %_builddir/%name-%version/build/kde/ananas.desktop %buildroot%_datadir/applications/kde
cp -f %_builddir/%name-%version/build/kde/ananas-designer.desktop %buildroot%_datadir/applications/kde
cp -f %_builddir/%name-%version/build/fedora/qtrc %buildroot%_datadir/ananas/

%post

# Fedora 8 does not provides %_libdir/qt-3.3/etc/settings/qtrc
# So we install them manualy
if ! [ -e %_libdir/qt-3.3/etc/settings/qtrc ]; then
	if ! [ -e %_libdir/qt-3.3/etc/settings ] ;then 
		mkdir -p %_libdir/qt-3.3/etc/settings
	fi
	cp %_datadir/ananas/qtrc %_libdir/qt-3.3/etc/settings/
fi

#starts mysql server 
[ -e /etc/init.d/mysqld ] && /etc/init.d/mysqld status|grep -v pid > /dev/null && /etc/init.d/mysqld start > /dev/null || :
#creates system base for mysql
mysqlshow -u root| grep ananas_system > /dev/null || mysqladmin -u root create ananas_system || :
#touch %buildroot%_sysconfdir/ananas/groupsrc

rm -f /usr/lib/menu/ananas
rm -f /usr/lib/menu/ananas-designer


##%post -n ananas-inventory
#starts mysql server 
[ -e /etc/init.d/mysqld ] && /etc/init.d/mysqld status|grep -v pid > /dev/null && /etc/init.d/mysqld start > /dev/null || :
#creates databases
mysql -u root -e "create database ananas_inventory character set utf8" > /dev/null 2>&1 && mysql -u root ananas_inventory < %_datadir/ananas/inventory/inventory.sql || :
mysql -u root -e "create database ananas_inventory_demo character set utf8" > /dev/null 2>&1 && mysql -u root ananas_inventory_demo < %_datadir/ananas/inventory/inventory-demo.sql || :

%postun
# Если %_libdir/qt-3.3/etc/settings/qtrc не принадлежит ни одному пакету,
# значит это мы его поставили и должны удалить при деинсталляции.
rpm -qf %_libdir/qt-3.3/etc/settings/qtrc > /dev/null 2>&1 || rm  -f %_libdir/qt-3.3/etc/settings/qtrc \
&& rmdir  %_libdir/qt-3.3/etc/settings 2>/dev/null && rmdir --ignore-fail-on-non-empty %_libdir/qt-3.3/etc

rm -f %_datadir/ananas/translations/*.qm
rm -f %_datadir/ananas/templates/*.*

rmdir --ignore-fail-on-non-empty %_datadir/ananas/translations
rmdir --ignore-fail-on-non-empty %_datadir/ananas/templates
rmdir --ignore-fail-on-non-empty %_datadir/ananas

##%postun -n ananas-extensions
rmdir --ignore-fail-on-non-empty %_libdir/ananas
rmdir --ignore-fail-on-non-empty %_datadir/ananas


##%postun -n ananas-inventory
rm -f %_sysconfdir/ananas/inventory.*
rm -f %_sysconfdir/ananas/inventory-demo.*
rm -f %_sysconfdir/ananas/inventory_grouprc
rmdir --ignore-fail-on-non-empty %_sysconfdir/ananas
rm -f %_datadir/ananas/inventory/templ_*.odt
rm -f %_datadir/ananas/inventory/templ_*.ods
rmdir --ignore-fail-on-non-empty %_datadir/ananas/inventory
rmdir --ignore-fail-on-non-empty %_datadir/ananas


%clean

%files
%_bindir/ananas
%_bindir/ananas-designer
%_libdir/libananas.so*
%_libdir/libananasplugin.so
%_libdir/ananas/qt3plugins/designer/libananasplugin.so
%_libdir/ananas/qt3plugins/sqldrivers/libqsqlmysqlu.so
%_docdir/%name-%version/ananas-*.sxw
%_datadir/ananas/translations/*.qm
%_datadir/ananas/templates/*.*
%_datadir/pixmaps/*.png
# files menu for ALT
##/usr/lib/menu/ananas
##/usr/lib/menu/ananas-designer
# files desktop for Fedora
%_datadir/applications/kde/ananas.desktop
%_datadir/applications/kde/ananas-designer.desktop
%_datadir/ananas/qtrc

##%files -n ananas-devel
%_includedir/ananas/*.h
%_docdir/%name-%version/*.tex

##%files -n ananas-inventory
%_sysconfdir/ananas/inventory.*
%_sysconfdir/ananas/inventory-demo.*

%_datadir/ananas/inventory/inventory.sql
%_datadir/ananas/inventory/inventory-demo.sql
%_datadir/ananas/inventory/inventory_pgsql.sql

%_datadir/ananas/inventory/templ_*.odt
%_datadir/ananas/inventory/templ_*.ods
%_sysconfdir/ananas/inventory_grouprc

##%files -n ananas-extensions
%_libdir/ananas/*.so

%changelog
* Sun Dec 30 2007 Andrey Paskal 0.9.5
  + Please look at ChangeLog file of Ananas Project

