@echo OFF
if exist ".vs" (
rmdir /S /Q .vs 
echo ".vs" has been deleted) else (echo ".vs" not exist)

if exist "Binaries" (
rmdir /S /Q Binaries 
echo "Binaries" has been deleted) else (echo "Binaries" not exist)

if exist "DerivedDataCache" (
rmdir /S /Q DerivedDataCache 
echo "DerivedDataCache" has been deleted) else (echo "DerivedDataCache" not exist)

if exist "Intermediate" (
rmdir /S /Q Intermediate 
echo "Intermediate" has been deleted) else (echo "Intermediate" not exist)

if exist "KillThem.sln" (
del KillThem.sln 
echo "KillThem.sln" has been deleted) else (echo "KillThem.sln" not exist)
@echo:
echo Please REGENERATE "KillThem.uproject" file before continue
pause
@echo:
CHOICE /C YN /M "Do you regenerated you .uproject files?"
IF ERRORLEVEL 2 EXIT

if exist "KillThem.sln" (start KillThem.sln) else (echo Blyat, gde .sln??????? Dlya kogo vyshe text? Zapuskai zanogo!!!)
pause

@echo:
CHOICE /C YN /M "Do you successfully compile project?"
IF ERRORLEVEL 2 EXIT

if exist "KillThem.uproject" (start KillThem.uproject) else (echo Blyat, dlya kogo vyshe text? Zapuskai zanogo!!!
pause)