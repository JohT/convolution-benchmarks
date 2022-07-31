# Benchmark Chart Generator

The contained [Node.js](https://nodejs.dev) scripts generate [Vega-Lite](https://vega.github.io/vega-lite) charts from C++ [Catch2](https://github.com/catchorg/Catch2) benchmark results.

## ⚒️ Tools

[Node.js](https://nodejs.dev) is
> Node.js is a free, open-sourced, cross-platform JavaScript run-time environment

[Vega-Lite](https://vega.github.io/vega-lite) is
> is a high-level grammar of interactive graphics.

[Catch2](https://github.com/catchorg/Catch2) is 
> A modern, C++-native, test framework for unit-tests...

## 🦶 Steps
- The [Catch2 XML Reporter](https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md) is used to generate a XML file with the benchmark data.
- The XML file is converted to a JSON file using [convert-xml-to-json](https://www.npmjs.com/package/convert-xml-to-json).
- The JSON data file is then used to generate the charts using [Vega-Lite](https://vega.github.io/vega-lite).
- [BenchmarkChartHorizontal.json](./BenchmarkChartHorizontal.json) contains the chart description and configuration.
- The command line tool [vl2svg](https://vega.github.io/vega-lite/usage/compile.html#cli) is used to create a SVG vector graphics file with the chart.

## ⚙️ Parameter

### BenchmarkChartEnvironmentFile (pre set)
- Located in [package.json](./package.json) config section
- Specifies the path to the environment file that contains parameter for the chart creation
- Currently set to `./../build/BenchmarkChart.env`
- It is expected to be created with [Catch2 XML Reporter](https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md) for example with the following command:   
`MyCatch2TestExecutable [performance] --reporter XML::out=benchmark-report.xml`

### BenchmarkChartLocalDataFile (pre set)
- Located in [package.json](./package.json) config section
- Specifies the name of the local data file name without path that is referenced in the [Vega-Lite](https://vega.github.io/vega-lite) JSON chart description [BenchmarkChartHorizontal.json](./BenchmarkChartHorizontal.json) under `"data": { "url": "..."`
- Currently set to `benchmark-report.json`

### BENCHMARK_CHART_DATA_FILE (mandatory)
- Located inside the environment file specified with `BenchmarkChartEnvironmentFile`
- Specifies the absolute path to the XML file that contains the results of the benchmark tests.
- Example:   
`BENCHMARK_CHART_DATA_FILE=C:\Git\convolution-benchmarks\build\benchmark\benchmark-report.xml`
- In this project it is created with [cmake/BenchmarkChartEnvironmentFileSetup.cmake](./../cmake/BenchmarkChartEnvironmentFileSetup.cmake)

### BENCHMARK_CHART_GROUP (mandatory)
- Located inside the environment file specified with `BenchmarkChartEnvironmentFile`
- Name of the group folder that will contain the chart and its data (e.g. "AppleClang-macOS-arm64")
- Example:   
`BENCHMARK_CHART_GROUP=MSVC-Windows-AMD64AVX2`
- In this project it is created with [cmake/BenchmarkChartEnvironmentFileSetup.cmake](./../cmake/BenchmarkChartEnvironmentFileSetup.cmake)

### Parametrized benchmark charts

If the benchmark test case name contains a description surrounded by double round brackets, 
a separate chart is created with the parametrized description as title. This convention makes it easy to create charts for parametrized benchmark tests. For example: All test cases that contain "((kernel size 16))" in their name will be grouped into the same chart, all test tests containing "((kernel size 512))" will be grouped into a second chart and so on.

## ⚡️ Commands

Run these two commands to setup the project and generate the charts:

```shell
npm ci
npm run build
```

Further commands and details can be found in [COMMANDS.md](./COMMANDS.md).

## 🔎 References
- [Catch2 XML Reporter](https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md)
- [convert-xml-to-json (NPM)](https://www.npmjs.com/package/convert-xml-to-json)
- [Vega-Lite](https://vega.github.io/vega-lite)
- [Vega-Lite command line tool vl2svg](https://vega.github.io/vega-lite/usage/compile.html#cli)
- [package.json - how to set default value of variable in windows (StackTrace)](https://stackoverflow.com/questions/70219331/package-json-how-to-set-default-value-of-variable-in-windows)
- [Three Things You Didn't Know You Could Do with npm Scripts (Dominik Kundel)](https://www.twilio.com/blog/npm-scripts)