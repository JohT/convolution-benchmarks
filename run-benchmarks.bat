@ECHO OFF
mkdir .\build\benchmark
.\build\test\ConvolutionBenchmarksTests [performance] --reporter XML::out=.\build\benchmark\benchmark-report.xml --reporter console::out=-::colour-mode=ansi