@echo off
@rem This code must be run under Visual Studio Command Line.

@rem This file is part of STARTCL.

@rem STARTCL is free software: you can redistribute it and/or modify
@rem it under the terms of the GNU General Public License as published by
@rem the Free Software Foundation, either version 3 of the License, or
@rem (at your option) any later version.

@rem STARTCL is distributed in the hope that it will be useful,
@rem but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@rem GNU General Public License for more details.

@rem You should have received a copy of the GNU General Public License
@rem along with STARTCL.  If not, see <http://www.gnu.org/licenses/>.

@echo ���뿪ʼ��
if "%VCINSTALLDIR%" == "" goto :exit
cl.exe /MD /D "NDEBUG" /EHsc /c main.cpp
rc.exe main.rc
mkdir Release
link.exe main.obj main.res /OUT:Release\startcl.exe
del main.obj
del main.res
pause
goto realexit

:exit
echo ����: MAKEFILE �ű������� Visual Studio ������ʾ�����У�
echo.
pause

:realexit
