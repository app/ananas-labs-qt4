# ananas project
# Spec file for building rpm and rpms under ALT Linux and Fedora
# $Id: altlinux.spec,v 1.1 2008/11/27 20:20:54 leader Exp $
# $Name:  $
#

%define ShortName ananas
%define _qtdir %(echo $QTDIR)
#%define OSDISTR %(OSVENDOR=REDHAT; echo $OSVENDOR)
%define OSDISTR %(if [ -e /etc/altlinux-release ] ; then OSVENDOR=ALT; fi ; echo $OSVENDOR )

Name: ananas
Version: 0.9.5
Release: %(if [ -e /etc/altlinux-release ] ; then echo "alt2"; else echo f`grep -o "[0-9|\.]\+" /etc/redhat-release` ; fi)
Buildroot: %{_tmppath}/%{name}-%{version}-root

Summary: Runtime and development environment of Finance Applications
Summary(ru_RU.KOI8-R):	Среда разработки и исполнения прикладных решений автоматизации оперативного, бухгалтерского и других видов учета.

License: GPL
Url: http://ananas.lrn.ru/
Vendor:	Project Ananas

Group(ru): %(if [ -e /etc/altlinux-release ] ; then echo "Офис"; else echo "Приложения/Повышение уровня производительности"; fi)
Group: %(if [ -e /etc/altlinux-release ] ; then echo "Office"; else echo "Applications/Productivity"; fi)

Source:		%name-%version.tar.bz2


#Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "MySQL-server >= 4.1 "; else echo "mysql-server >= 4.1 "; fi)
Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "MySQL-client"; else echo "mysql"; fi)
#Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "MySQL-client >= 4.1 "; else echo "mysql >= 4.1 "; fi)
Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "libpq3"; else echo "postgresql"; fi)
Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "libqt3 >= 3.2"; else echo "qt >= 3.2.0"; fi)
Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "libqt3-qsa >= 3.2"; else echo "qt-qsa >= 1.1"; fi)
#Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "qt3-designer >= 3.2"; else echo "qt-designer >= 3.2" ;fi)
#Requires: tradeequip
Requires: libqt3-qdataschema

BuildRequires:  gcc-c++ libstdc++-devel
#BuildRequires:  tradeequip-devel 
#tradeequip

#BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libMySQL-devel >= 4.1"; else echo "mysql-devel >= 4.1"; fi)
BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libMySQL-devel"; else echo "mysql-devel"; fi)
BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libqt3-devel"; else echo "qt-devel"; fi)
BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libqt3-qsa-devel"; else echo "qt-qsa-devel"; fi)
#BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libqt3-mysql"; else echo "qt-MySQL"; fi)
#BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libqt3-postgresql"; else echo "qt-PostgreSQL"; fi)
#BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libqt3-sqlite"; else echo "sqlite-devel"; fi)
BuildRequires: libqt3-qdataschema-devel


%description -l ru_RU.KOI8-R
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


%package -n ananas-devel
Summary(ru_RU.KOI8-R):	Статические библиотеки и файлы определений на языке Cи.
Summary:	Ananas development library. Static libs and C header files.
Group(ru_RU.KOI8-R):	Разработка/Библиотеки
Group:		Development/Other

%description -l ru_RU.KOI8-R -n ananas-devel
Статические библиотеки и файлы определений на языке C++.
ananas-devel необходимо установить, если вы планируете использовать библиотеки
Ананаса в своей C/C++ программе. Если вы планируете использовать прикладное
решение на платформе Ананас, то пакет не нужно устанавливать.

Разработчики
---------------
Валерий Гражданкин <vg at leaderit dot ru>
Андрей Паскаль <app at lrn dot ru>
Андрей Стрельников <avsone at yandex dot ru>
Павел Миколайчук <Pashik at bk dot ru>
Александр Ковалёв <cibfx at bk dot ru>
Григорий Панов <gr1313 at mail dot ru>

%description -n ananas-devel
Ananas development library. Static libs and C header files.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>
Andrey Paskal <app at lrn dot ru>
Andrey Strelnikov <avsone at yandex dot ru>
Pavel Mikolaychuk <Pashik at bk dot ru>
Alexander Kovalyov <cibfx at bk dot ru>
Grigory Panov <gr1313 at mail dot ru>

%package -n ananas-inventory
Summary(ru_RU.KOI8-R):	Прикладная настройка для ведения простого складского учета.
Summary:	Scripts, Documents and Reports templates of Inventory system.
Group:		Office
Group(ru_RU.KOI8-R):	Приложения/Учет и Финансы

Requires: ananas >= %version
Requires: zip
Requires: unzip

%description -l ru_RU.KOI8-R -n ananas-inventory
Прикладная настройка для ведения простого складского учета.

Разработчики
---------------
Валерий Гражданкин <vg at leaderit dot ru>
Андрей Паскаль <app at lrn dot ru>
Андрей Стрельников <avsone at yandex dot ru>
Павел Миколайчук <Pashik at bk dot ru>
Александр Ковалёв <cibfx at bk dot ru>
Григорий Панов <gr1313 at mail dot ru>

%description -n ananas-inventory
Scripts, Documents and Reports templates of Inventory Accounting system.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>
Andrey Paskal <app at lrn dot ru>
Andrey Strelnikov <avsone at yandex dot ru>
Pavel Mikolaychuk <Pashik at bk dot ru>
Alexander Kovalyov <cibfx at bk dot ru>
Grigory Panov <gr1313 at mail dot ru>

%package -n ananas-extensions
Summary(ru_RU.KOI8-R):	Набор плагинов для Ананаса.
Summary:	Ananas plugins collection.
Group:		Office
Group(ru_RU.KOI8-R):	Приложения/Учет и Финансы

Requires: ananas >= %version

%description -l ru_RU.KOI8-R -n ananas-extensions
Набор плагинов для Ананаса.

Разработчики
---------------
Валерий Гражданкин <vg at leaderit dot ru>
Андрей Паскаль <app at lrn dot ru>
Андрей Стрельников <avsone at yandex dot ru>
Павел Миколайчук <Pashik at bk dot ru>
Александр Ковалёв <cibfx at bk dot ru>
Григорий Панов <gr1313 at mail dot ru>

%description -n ananas-extensions
Ananas plugins collection.

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
#export QTDIR=/usr/lib/qt3
export PATH=${PATH}:$QTDIR/bin
./configure
make
make tr

%install
export PATH=${PATH}:$QTDIR/bin

if [ -e /etc/altlinux-release ] ; then

# пока libMySQL >=4.1 нет в Sisyphus, чтобы не пересобирать MySQL-devel :-(
%set_verify_elf_method textrel=relaxed

	make install INSTALL_ROOT="$RPM_BUILD_ROOT" LIBDIR=%_libdir BINDIR=%_bindir DOCDIR=%_docdir/%name-%version INCLUDEDIR=%_includedir/ananas
	%__mkdir_p %buildroot%_sysconfdir/ananas
	%__mkdir_p %buildroot%_datadir/ananas/translations
	%__mkdir_p %buildroot%_datadir/ananas/templates
	%__mkdir_p %buildroot%_datadir/ananas/inventory
	%__mkdir_p %buildroot%_datadir/pixmaps
	%__mkdir_p %buildroot%_menudir
	%__mkdir_p %buildroot%_datadir/applications/kde

else

	rm -f `find src doc -name Makefile`
	qmake src/src.pro -o src/Makefile
	cd src && make install INSTALL_ROOT="$RPM_BUILD_ROOT" LIBDIR=%_libdir BINDIR=%_bindir INCLUDEDIR=%_includedir/ananas && cd ../doc && qmake doc.pro -o Makefile && make install INSTALL_ROOT="$RPM_BUILD_ROOT" DOCDIR=%_docdir/%name-%version

	mkdir -p %buildroot%_sysconfdir/ananas
	mkdir -p %buildroot%_datadir/ananas/translations
	mkdir -p %buildroot%_datadir/ananas/templates
	mkdir -p %buildroot%_datadir/ananas/inventory
	mkdir -p %buildroot%_datadir/applications/kde
	mkdir -p %buildroot%_datadir/pixmaps
	mkdir -p %buildroot/usr/lib/menu
fi

#clear debug info
strip %buildroot%_bindir/ananas
strip %buildroot%_bindir/ananas-designer
strip %buildroot%_libdir/libananas.so*
strip %buildroot%_libdir/ananas/qt3plugins/designer/libananasplugin.so
strip %buildroot%_libdir/ananas/qt3plugins/sqldrivers/libqsqlmysqlu.so
strip %buildroot%_libdir/ananas/*.so

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

#if [ -e /etc/altlinux-release ] ; then

	cp -f %_builddir/%name-%version/build/kde/ananas %buildroot/usr/lib/menu
	cp -f %_builddir/%name-%version/build/kde/ananas-designer %buildroot/usr/lib/menu

#else

	cp -f %_builddir/%name-%version/build/kde/ananas.desktop %buildroot%_datadir/applications/kde
	cp -f %_builddir/%name-%version/build/kde/ananas-designer.desktop %buildroot%_datadir/applications/kde
#fi

%post
ln -fs  %_libdir/ananas/qt3plugins/designer/libananasplugin.so %_libdir/libananasplugin.so

#starts mysql server 
[ -e /etc/init.d/mysqld ] && /etc/init.d/mysqld status|grep -v pid > /dev/null && /etc/init.d/mysqld start > /dev/null || :
#creates system base for mysql
mysqlshow -u root| grep ananas_system > /dev/null || mysqladmin -u root create ananas_system || :
#touch %buildroot%_sysconfdir/ananas/groupsrc

if [ -e /etc/altlinux-release ] ; then
	rm -f %_datadir/applications/kde/ananas.desktop
	rm -f %_datadir/applications/kde/ananas-designer.desktop
	%update_menus
else
	rm -f /usr/lib/menu/ananas
	rm -f /usr/lib/menu/ananas-designer
fi


%post -n ananas-inventory
#cat %_datadir/ananas/inventory/groupsrc >> %_sysconfdir/ananas/groupsrc
#starts mysql server 
[ -e /etc/init.d/mysqld ] && /etc/init.d/mysqld status|grep -v pid > /dev/null && /etc/init.d/mysqld start > /dev/null || :
#creates databases
mysql -u root -e "create database ananas_inventory character set utf8" > /dev/null 2>&1 && mysql -u root ananas_inventory < %_datadir/ananas/inventory/inventory.sql || :
mysql -u root -e "create database ananas_inventory_demo character set utf8" > /dev/null 2>&1 && mysql -u root ananas_inventory_demo < %_datadir/ananas/inventory/inventory-demo.sql || :

%postun

if [ -e /etc/altlinux-release ] ; then
%clean_menus
fi
rm -f %_datadir/ananas/translations/*.qm
rm -f %_datadir/ananas/templates/*.*

rmdir --ignore-fail-on-non-empty %_datadir/ananas/translations
rmdir --ignore-fail-on-non-empty %_datadir/ananas/templates
rmdir --ignore-fail-on-non-empty %_datadir/ananas

%postun -n ananas-extensions
rmdir --ignore-fail-on-non-empty %_libdir/ananas
rmdir --ignore-fail-on-non-empty %_datadir/ananas


%postun -n ananas-inventory
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
%_libdir/ananas/qt3plugins/designer/libananasplugin.so
%_libdir/ananas/qt3plugins/sqldrivers/libqsqlmysqlu.so
%_docdir/%name-%version/ananas-*.sxw
%_datadir/ananas/translations/*.qm
%_datadir/ananas/templates/*.*
%_datadir/pixmaps/*.png
# files menu for ALT
/usr/lib/menu/ananas
/usr/lib/menu/ananas-designer
# files desktop for Fedora
%_datadir/applications/kde/ananas.desktop
%_datadir/applications/kde/ananas-designer.desktop


%files -n ananas-devel
%_includedir/ananas/*.h
%_docdir/%name-%version/*.tex

%files -n ananas-inventory
%_sysconfdir/ananas/inventory.*
%_sysconfdir/ananas/inventory-demo.*

%_datadir/ananas/inventory/inventory.sql
%_datadir/ananas/inventory/inventory-demo.sql
%_datadir/ananas/inventory/inventory_pgsql.sql

%_datadir/ananas/inventory/templ_*.odt
%_datadir/ananas/inventory/templ_*.ods
%_sysconfdir/ananas/inventory_grouprc

%files -n ananas-extensions
%_libdir/ananas/*.so

%changelog
* Sun Dec 30 2007 Andrey Paskal 0.9.5
  + Please look at ChangeLog file of Ananas Project

* Fri Sep 08 2006 Andrey Paskal
  + Meta extention has been added
  
* Sun Jul 02 2006 Valery Grazdankin <leader@altlinux.ru> 0.9.3-alt2
  + Trade equipment extension added 

* Tue Jun 20 2006 Grigory Panov <gr1313 at mail dot ru> 0.9.3-alt1
  + Added extensions
  
* Mon Mar 27 2006 Valery Grazdankin <leader@altlinux.ru> 0.9.3-alt1
  - Automatic generated cvs build

* Tue Feb 21 2006 Valery Grazdankin <leader@altlinux.ru> 0.9.2-alt1
  + Add ananas extensions plugin system.

* Thu Jan 05 2006 Grigory Panov <gr1313 at mail dot ru> 0.9.1-alt1
  + Merge with st05 branch

* Wed Jun 06 2005 Valery Grazdankin <leader@altlinux.ru> 0.4.2-alt1
  + Files list was changed in the inventory package

* Fri May 06 2005 Valery Grazdankin <leader@altlinux.ru> 0.4.2-alt1
  + Added support some configuration files

* Thu May 05 2005 Andrey Paskal <app at lrn dot ru> 0.4.2-alt1
  + rpm build procedure was changed. After install ananas-inventory.*rpm
  we can immediately to work with "Deport accouning" configuration ,
  by running ananas-engine.
  + Debugged (by gr) report procession in OpenOffice format under Linux.

* Sat Mar 19 2005 Grigory Panov <gr1313 at mail dot ru> 0.4.2-alt1
  + Added template in OpenOffice format for ananas-inventory
  + Added section for remove files and directories after deinstallation
  + Added ananas-engine-qt requires to ananas-inventory package

* Tue Mar 15 2005 Valery Grazdankin <leader@altlinux.ru> 0.4.2-alt1
  + File name ananasrc changed to groupsrc
  + groupsrc have Unix file format now

* Sun Nov 21 2004 Valery Grazdankin <leader@altlinux.ru> 0.4.2-alt1
  + Correct installation procedure

* Fri Nov 05 2004 Valery Grazdankin <leader@altlinux.ru> 0.4.2-alt1
  + Added spec file for build ALT Linux packages.
  + win32 version build completed. Ananas - is multitarget application now
