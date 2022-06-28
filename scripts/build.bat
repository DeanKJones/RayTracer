@REM @echo off
 
@REM :: Create build dir
@REM set buildDir=%~dp0..build
@REM if not exist %buildDir% mkdir %buildDir%
@REM pushd %buildDir%
 
@REM :: Needed folders
@REM set extDir=%~dp0..\core\external
@REM set scriptDir=%~dp0..\scripts
 
@REM :: Use make to build default target
@REM cd %scriptDir%\
@REM mingw32-make
 
@REM popd