; Inno Setup Installation Script.
; Version 5+ of Inno Setup required
; (c) Leader InfoTech, 2004
; (c) Valery Grazdankin ( leader ), 2004
; (c) patched by Andrey Paskal, 2005
; (c) patched by Grigory Panov, 2005-2007

[Languages]
Name: Russian; MessagesFile: "compiler:languages\Russian.isl"
Name: English; MessagesFile: "compiler:default.isl"

[Setup]
SourceDir=..\..\
AppName=Ananas
AppVersion=0.9.5
AppVerName=Ananas 0.9.5

AppPublisher=Ananas Team
AppPublisherURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
AppSupportURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
AppUpdatesURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
DefaultDirName={pf}\Ananas
DefaultGroupName=Ananas
AppCopyright=Copyright c 2002-2007 Leader InfoTech, Ananas Team
DisableProgramGroupPage=yes
AllowNoIcons=yes
LicenseFile=COPYING.RU.cp1251
InfoBeforeFile=build\win32\secondstep.txt
Compression=lzma
SolidCompression=yes
OutputDir=.\
OutputBaseFilename=Ananas-Setup-0.9.5

[Components]
Name: "designer"; Description: "Дизайнер"; Types: full compact custom
Name: "runtime"; Description: "Ананас"; Types: full compact custom
;Name: "webengine"; Description: "Веб служба"; Types: full compact custom
Name: "doc"; Description: "Документация"; Types: full custom
;Name: "doc\user"; Description: "Руководство пользователя"; Types: full custom
;Name: "doc\designer"; Description: "Описание дизайнера"; Types: full custom
;Name: "doc\script"; Description: "Описание встроенного языка"; Types: full custom
;Name: "sources"; Description: "Source code"; Types: full custom
;;Name: "sqlservers"; Description: "Database SQL servers"; Types: full custom
;;Name: "sqlservers\mysql"; Description: "MySQL Database servers"; Types: full custom
;;Name: "sqlservers\postgres"; Description: "PostgreSQL Database servers"; Types: full custom
;Name: "acrobat"; Description: "Программа просмотра документации Acrobat Reader 6.0"; Types: full custom
Name: "applications"; Description: "Бизнес-схемы проекта Ананас"; Types: full custom

Name: "applications\inventory"; Description: "Оперативный учет"; Types: full compact custom
Name: "applications\inventory_demo"; Description: "Оперативный учет. Демо."; Types: full custom
Name: "extensions"; Description: "Расширения (плагины)"; Types: full custom 
Name: "extensions\text"; Description: "Для работы с текстовыми файлами"; Types: full custom
Name: "extensions\xml"; Description: "Для работы с XML"; Types: full custom
Name: "extensions\meta"; Description: "Для доступа к метаданным"; Types: full custom
Name: "extensions\sql"; Description: "Для выполнения SQL запросов в отчетах"; Types: full custom
Name: "extensions\service"; Description: "Для доступа к различным служебным функциям"; Types: full custom
Name: "extensions\dateservice"; Description: "Для работы с датами"; Types: full custom
Name: "extensions\minicalc"; Description: "Калькулятор"; Types: full custom
Name: "mysql"; Description: "Сервер баз данных MySQL"; Types: full compact custom; Flags: fixed
Name: "office"; Description: "Подсистема отчетов"; Types: full compact custom; Flags: fixed
Name: "office\ooo"; Description: "Использовать OpenOffice.org";  Types: full compact custom; Flags: exclusive;
Name: "office\mso"; Description: "Использовать Microsoft Office"; Types: full compact custom; Flags: exclusive;

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
Source: "build\win32\setuplogo.bmp"; DestDir: {tmp}; Flags: dontcopy
;Source: "doc\*"; DestDir: "{app}\doc"; Components: doc; Flags: ignoreversion
Source: "src\designer\ananas-designer.exe"; DestDir: "{app}"; Components: designer; Flags: ignoreversion
;Source: "src\designer\testwrite.cfg"; DestDir: "{app}\applications\test"; Flags: ignoreversion
Source: "src\ananas\ananas.exe"; DestDir: "{app}"; Components: runtime; Flags: ignoreversion
;Source: "src\administrator\ananas-administrator.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\plugins\ananasplugin.dll"; DestDir: "{app}\designer"; Flags: ignoreversion
;Source: "src\plugins\rcplugin.dll"; DestDir: "{app}\designer"; Flags: ignoreversion
;Source: "src\webengine\ananas-webengine.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "src\ananas\bkground.jpg"; DestDir: "{app}"; Flags: ignoreversion
;Source: "src\designer\my.rc"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\lib\ananas.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "COPYING.RU.cp1251"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\win32\README.CP1251"; DestDir: "{app}"; Flags: ignoreversion
Source: "translations\*.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "src\designer\templates\*.*"; DestDir: "{app}\templates"; Components: designer; Flags: ignoreversion
;Source: "src\webengine\tpl\*.*"; DestDir: "{app}\tpl"; Flags: ignoreversion
;;;Source: "*"; Excludes:"*.exe,*.dll,*.lib,*.~*,.qmake.cache,*.tds,*.obj,CVS\*,tmp\*"; DestDir: "{app}\sources"; Flags: ignoreversion recursesubdirs
;Source: "c:\mysql\*"; DestDir: "{app}\mysql\"; Flags: ignoreversion recursesubdirs

Source: "applications\inventory\*.*"; DestDir: "{app}\applications\inventory"; Components: applications; Flags: ignoreversion

Source: "src\extensions\aexttext.dll"; DestDir: "{app}\extensions"; Components: extensions\text; Flags: ignoreversion;
Source: "src\extensions\aexttext.lib"; DestDir: "{app}\extensions"; Components: extensions\text; Flags: ignoreversion;
Source: "src\extensions\aextxml.dll"; DestDir: "{app}\extensions"; Components: extensions\xml; Flags: ignoreversion;
Source: "src\extensions\aextxml.lib"; DestDir: "{app}\extensions"; Components: extensions\xml; Flags: ignoreversion;
Source: "src\extensions\aext_meta.dll"; DestDir: "{app}\extensions"; Components: extensions\meta; Flags: ignoreversion;
Source: "src\extensions\aext_meta.lib"; DestDir: "{app}\extensions"; Components: extensions\meta; Flags: ignoreversion;
Source: "src\extensions\aextsql.dll"; DestDir: "{app}\extensions"; Components: extensions\sql; Flags: ignoreversion;
Source: "src\extensions\aextsql.lib"; DestDir: "{app}\extensions"; Components: extensions\sql; Flags: ignoreversion;
Source: "src\extensions\aextservice.dll"; DestDir: "{app}\extensions"; Components: extensions\service; Flags: ignoreversion;
Source: "src\extensions\aextservice.lib"; DestDir: "{app}\extensions"; Components: extensions\service; Flags: ignoreversion;
Source: "src\extensions\aextdateservice.dll"; DestDir: "{app}\extensions"; Components: extensions\dateservice; Flags: ignoreversion;
Source: "src\extensions\aextdateservice.lib"; DestDir: "{app}\extensions"; Components: extensions\dateservice; Flags: ignoreversion;
Source: "src\extensions\aextminicalc.dll"; DestDir: "{app}\extensions"; Components: extensions\minicalc; Flags: ignoreversion;
Source: "src\extensions\aextminicalc.lib"; DestDir: "{app}\extensions"; Components: extensions\minicalc; Flags: ignoreversion;
Source: "c:\Qt\lib\qtmtnc321.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\Qt\lib\qsa110.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\Qt\plugins\designer\qseditorplugin.dll"; DestDir: "{app}\designer"; Flags: ignoreversion

Source: "c:\Qt\plugins\sqldrivers\*.dll"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion
;if qsqlmysqlu exist in c:\Qt\3.2.1noncommercial\plugins\sqldrivers, replaced it with src\plugins\mysql
Source: "src\plugins\mysql\qsqlmysqlu.dll"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion

Source: "..\MySQL\MySQL Server 4.1\bin\libmysql.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "c:\qt\3.2.1noncommercial\bin\pq.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "c:\qt\3.2.1noncommercial\bin\cc3250.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "c:\qt\3.2.1noncommercial\bin\cc3250mt.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\Borland\bcc55\Bin\cc3250.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\Borland\bcc55\Bin\cc3250mt.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "c:\qt\bin\linguist.exe"; DestDir: "{app}"; Flags: ignoreversion

Source: "icons\a-system.ico"; DestDir: "{app}"; Flags: ignoreversion;
Source: "icons\ananas-designer.ico"; DestDir: "{app}"; Flags: ignoreversion;
Source: "icons\manual.ico"; DestDir: "{app}"; Flags: ignoreversion;

Source: "build\win32\makerc.bat"; DestDir: "{app}"; Flags: ignoreversion;
Source: "build\win32\makemysqlini.bat"; DestDir: "{app}"; Flags: ignoreversion;
Source: "applications\inventory\wininstall.bat"; DestDir: "{app}"; Components: applications\inventory; Flags: ignoreversion;
Source: "applications\inventory\demowininstall.bat"; DestDir: "{app}"; Components: applications\inventory_demo; Flags: ignoreversion;


Source: "..\MySQL\*"; DestDir: {app}; Flags: ignoreversion recursesubdirs; Components:  mysql

Source: "..\7-zip\7z.exe"; DestDir: {app}; Flags: ignoreversion recursesubdirs;
Source: "..\7-zip\Codecs\copy.dll"; DestDir: "{app}\Codecs"; Flags: ignoreversion recursesubdirs;
Source: "..\7-zip\Codecs\deflate.dll"; DestDir: "{app}\Codecs"; Flags: ignoreversion recursesubdirs;
Source: "..\7-zip\Formats\zip.dll"; DestDir: "{app}\Formats"; Flags: ignoreversion recursesubdirs;

Source: "..\doc\*.pdf"; DestDir: "{app}\doc"; Flags: ignoreversion recursesubdirs; Components: doc

; NOTE: Don't use "Flags: ignoreversion" on any shared system files
[INI]
Filename: "{app}\lit.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://ananas.lrn.ru"

[Icons]
Name: "{group}\Ананас.Оперативный учет"; Components: runtime; Filename: "{app}\ananas.exe"; IconFilename: "{app}\a-system.ico"
Name: "{group}\Ананас.Дизайнер"; Components: designer; Filename: "{app}\ananas-designer.exe"; IconFilename: "{app}\ananas-designer.ico"
Name: "{group}\Ананас.Руководство пользователя"; Components: doc; Filename: "{app}\doc\ananas-inventory-user-manual.pdf"; IconFilename: "{app}\manual.ico"
Name: "{group}\Ананас.Руководство программиста"; Components: doc; Filename: "{app}\doc\ananas-designer-quick-start.pdf"; IconFilename: "{app}\manual.ico"
Name: "{group}\{cm:UninstallProgram,Ananas}"; Filename: "{uninstallexe}"; AfterInstall: ananasAfterInstall
Name: "{userdesktop}\Ананас.Оперативный учет"; Components: runtime; Filename: "{app}\ananas.exe"; Tasks: desktopicon; IconFilename: "{app}\a-system.ico"
Name: "{userdesktop}\Ананас.Дизайнер"; Components: designer; Filename: "{app}\ananas-designer.exe"; Tasks: desktopicon; IconFilename: "{app}\ananas-designer.ico"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Ananas"; Filename: "{app}\ananas.exe"; Tasks: quicklaunchicon; IconFilename: "{app}\a-system.ico"
Name: "{group}\Ананас в интернете"; Filename: "{app}\lit.url"

[Registry]
Root: HKLM; Subkey: "Software\ananasgroup"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\ananasgroup\ananas"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\ananasgroup"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\ananasgroup\ananas"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\ananasgroup\ananas\groups"; ValueType: string; ValueName: "0"; ValueData: "Бизнес-схемы проекта Ананас"
Root: HKLM; Subkey: "Software\ananasgroup\ananas\groups\0"; Components: applications\inventory; ValueType: string; ValueName: "0"; ValueData: "{app}\applications\inventory\inventorywin.rc"
Root: HKCU; Subkey: "Software\ananasgroup\ananas\groups"; ValueType: string; ValueName: "0"; ValueData: "Бизнес-схемы проекта Ананас"
Root: HKCU; Subkey: "Software\ananasgroup\ananas\groups\0"; Components: applications\inventory; ValueType: string; ValueName: "0"; ValueData: "{app}\applications\inventory\inventorywin.rc"
Root: HKLM; Subkey: "Software\ananasgroup\ananas\groups\0"; Components: applications\inventory_demo; ValueType: string; ValueName: "1"; ValueData: "{app}\applications\inventory\inventory-demowin.rc"
Root: HKCU; Subkey: "Software\ananasgroup\ananas\groups\0"; Components: applications\inventory_demo; ValueType: string; ValueName: "1"; ValueData: "{app}\applications\inventory\inventory-demowin.rc"
Root: HKCU; Subkey: "Software\ananasgroup\ananas\config\variables"; Components: office\mso; ValueType: string; ValueName: "defaultOffice";  ValueData: "MSO"
Root: HKCU; Subkey: "Software\ananasgroup\ananas\config\variables"; Components: office\ooo; ValueType: string; ValueName: "defaultOffice";  ValueData: "OOO"


[Run]
Filename: "{app}\ananas.exe"; Components: runtime; Description: "{cm:LaunchProgram,Ананас.Оперативный учет}"; Flags: nowait postinstall skipifsilent
FileName: "{app}\MySQL Server 4.1\bin\mysqld-nt.exe"; Parameters: "--install MySQLAnanas ""--defaults-file={app}\mysqls~1.1\my.ini"" "; Components:  mysql; Flags: runhidden; StatusMsg: "Установка MySQL..."
FileName: "net"; Parameters: "start MySQLAnanas"; Components:  mysql; Flags: shellexec runhidden

;FileName: "{app}\MySQL Server 4.1\bin\mysqladmin.exe"; Parameters: "-u root create ananas_system"; Components:  mysql; Flags: runhidden; StatusMsg: "Установка MySQL... Создание БД ananas_system"

FileName: "{app}\MySQL Server 4.1\bin\mysqladmin.exe"; Parameters: "-u root create ananas_inventory"; Components:  mysql applications\inventory; Flags: runhidden; StatusMsg: "Создание БД ananas_inventory"
FileName: "{app}\wininstall.bat"; Components: applications\inventory; Parameters: """{app}\MySQL Server 4.1\bin"" ""{app}\applications\inventory\""";  Flags: shellexec runhidden;
FileName: "{app}\makerc.bat"; Components: applications\inventory; Parameters: """{app}\applications\inventory""  ""{app}\applications\inventory\inventory.cfg""  ""{app}\applications\inventory\inventorywin.rc""";  Flags: shellexec runhidden;

FileName: "{app}\MySQL Server 4.1\bin\mysqladmin.exe"; Parameters: "-u root create ananas_inventory_demo"; Components:  mysql applications\inventory_demo; Flags: runhidden; StatusMsg: "Создание БД ananas_inventory_demo"
FileName: "{app}\demowininstall.bat"; Components: applications\inventory_demo; Parameters: """{app}\MySQL Server 4.1\bin"" ""{app}\applications\inventory\""";  Flags: shellexec runhidden;
FileName: "{app}\makerc.bat"; Components: applications\inventory_demo; Parameters: """{app}\applications\inventory""  ""{app}\applications\inventory\inventory-demo.cfg""  ""{app}\applications\inventory\inventory-demowin.rc""";  Flags: shellexec runhidden;


FileName: "{app}\makemysqlini.bat"; Parameters: """{app}\MySQL Server 4.1\""  ""{app}\MySQL Server 4.1\Data\""  ""{app}\MySQL Server 4.1/my.ini""";  Flags: shellexec runhidden;


[UninstallRun]
FileName: "net";  Parameters: "stop MySQLAnanas"; Components:  mysql; Flags: runhidden; StatusMsg: "Остановка MySQL..."
FileName: "{app}\MySQL Server 4.1\bin\mysqld-nt.exe"; Parameters: "--remove MySQLAnanas"; Components:  mysql; Flags: runhidden; StatusMsg: "Удаление MySQL..."

[UninstallDelete]
Type: files; Name: "{app}\lit.url"
Type: files; Name: "{app}\ananasplugin.dll"
Type: filesandordirs; Name: "{app}\MySQL"; Components:  mysql

[Code]
//
procedure InitializeWizard();
var
  SplashImage: TBitmapImage;
  SplashForm: TForm;
  SplashFileName: String;
begin

  SplashFileName := ExpandConstant('{tmp}\setuplogo.bmp');
  ExtractTemporaryFile(ExtractFileName(SplashFileName));

  SplashForm := TForm.create(nil);
  with SplashForm do
  begin
    BorderStyle := bsNone;
    Position := poScreenCenter;
    // Adjust the height and width of the SplashForm
    // to the size of our splash image
    ClientWidth := 400;
    ClientHeight := 244;
  end;

  SplashImage := TBitmapImage.Create(SplashForm);
  with SplashImage do
  begin
    Bitmap.LoadFromFile(SplashFileName);
    Stretch := true;
    Align := alClient;
    Parent := SplashForm;
  end;

  with SplashForm do
  begin
     Show;
     Repaint;
     Sleep(1000);
     Close;
     Free;
  end;
end;


procedure ananasAfterInstall();
begin
  { copy files }
  FileCopy( ExpandConstant('{app}\designer\ananasplugin.dll'), ExpandConstant('{app}\ananasplugin.dll'), false);
  
end;

