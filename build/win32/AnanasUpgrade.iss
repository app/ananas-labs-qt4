; Inno Setup Installation Script.
; Version 5+ of Inno Setup required
; (c) Leader InfoTech, 2004
; (c) Valery Grazdankin ( leader ), 2004
; (c) patched by Andrey Paskal, 2005
; (c) patched by Grigory Panov, 2005
; (c) patched by Alexey Chernov, 2006

[Languages]
Name: Russian; MessagesFile: "compiler:languages\Russian.isl"
Name: English; MessagesFile: "compiler:default.isl"

[Setup]
SourceDir=..\..\
AppName=Обновления Ananas
AppVersion=0.9.3
AppVerName=Обновление Ananas 0.9.3
AppPublisher=Ananas Team
AppPublisherURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
AppSupportURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
AppUpdatesURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
DefaultDirName={pf}\Ananas
DefaultGroupName=Ananas
DirExistsWarning=No
AppCopyright=Copyright c 2002-2006 Leader InfoTech, Ananas Project Team
DisableProgramGroupPage=yes
AllowNoIcons=yes
LicenseFile=COPYING.RU.cp1251
Compression=lzma
SolidCompression=yes
OutputDir=.\
OutputBaseFilename=Ananas-Update-0.9.3

[Files]
Source: "build\win32\setuplogo.bmp"; DestDir: {tmp}; Flags: dontcopy
Source: "src\designer\ananas-designer.exe"; DestDir: "{app}";
Source: "src\ananas\ananas.exe"; DestDir: "{app}";
Source: "src\plugins\ananasplugin.dll"; DestDir: "{app}\designer"; Flags: ignoreversion
Source: "src\plugins\ananasplugin.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\lib\ananas.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "COPYING.RU.cp1251"; DestDir: "{app}";DestName:"License.txt" ; Flags: ignoreversion
Source: "translations\*.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "c:\Qt\3.2.1noncommercial\lib\qsa110.dll"; DestDir: "{app}"; Flags: ignoreversion

[Code]
//
var
ananas_path:string;
  function InitializeSetup(): Boolean;
  begin
    if RegKeyExists(HKLM,'Software\ananasgroup\ananas')=false
    then
    begin
      Result := MsgBox('Не найдена установленная версия Ananas. Продолжить?', mbConfirmation, MB_YESNO) = idYes;
      if Result = False
      then
        MsgBox('Установка отменена', mbInformation, MB_OK);
    end
    else
      Result:=True;
  end;
procedure InitializeWizard();
var
  SplashImage: TBitmapImage;
  SplashForm: TForm;
  SplashFileName: String;
  uninsPath:string;
  
  ErrorCode:integer;
  
begin
{  if RegValueExists(HKLM,'Software\ananasgroup\ananas','UninstallString')=true
  then
  begin
  if MsgBox('Найдена установленная версия Ananas. Удалить?', mbConfirmation, MB_YESNO)= idYes then
    begin
      RegQueryStringValue(HKLM,'Software\ananasgroup\ananas', 'UninstallString',UninsPath);
      if not InstExec(ExpandConstant(UninsPath), '', '', True, False, SW_SHOWNORMAL, ErrorCode)
        then
      MsgBox('Script.NextButtonClick:' #13#13 'The file could not be executed. ' + SysErrorMessage(ErrorCode) + '.', mbError, MB_OK);
    end;
  end;  }
  
 { if RegKeyExists(HKLM,'Software\ananasgroup\ananas')=false
  then
  if MsgBox('Не найдена установленная версия Ananas. Продолжить?', mbConfirmation, MB_YESNO)= idYes then
  else ; }
    
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
Procedure CurPageChanged(CurPage: Integer);
begin
  Case CurPage of wpFinished:
  begin
    if FileExists(WizardDirValue+'\unins001.exe')
    then
      DeleteFile(WizardDirValue+'\unins001.exe');
    if FileExists(WizardDirValue+'\unins001.dat')
    then
      DeleteFile(WizardDirValue+'\unins001.dat');
  end;
  end;
end;
//procedure ananasAfterInstall();
//begin
  { copy files }
//  FileCopy( WizardDirValue+'\designer\ananasplugin.dll', WizardDirValue+'\ananasplugin.dll', false);
//end;
