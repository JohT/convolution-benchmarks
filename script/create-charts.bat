REM Run this script from the root directory of this repository

REM Change into the chart directory
cd chart

REM Installs all dependencies using Node.js Package Manager NPM
call npm ci

REM Creates the group folder for the current system and generates the SVG chart based on the benchmark results
call npm run build

REM Change back into main directory
cd ..