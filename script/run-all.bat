REM Run this script from the root directory of this repository

REM Runs all scripts in the right order to build the project, run the benchmarks and create the charts
call script\build.bat
call script\run-benchmarks.bat
call script\create-charts.bat