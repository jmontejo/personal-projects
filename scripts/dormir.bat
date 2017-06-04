@if (@a==@b) @end /*

:: batch portion, cancel with SHUTDOWN /A

@ECHO OFF

SHUTDOWN /S /F /T 1800
TIMEOUT /T 1200 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK
cscript /e:jscript "%~f0"
TIMEOUT /T 60 /NOBREAK

:: JScript portion */

var shl = new ActiveXObject("WScript.Shell");
for (var i=0; i<4; i++) {
    shl.SendKeys(String.fromCharCode(0xAE));
    }
