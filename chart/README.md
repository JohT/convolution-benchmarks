# Benchmark Chart Generator

The contained [Node.js](https://nodejs.dev) scripts generate [Vega-Lite](https://vega.github.io/vega-lite) charts from C++ [Catch2](https://github.com/catchorg/Catch2) benchmark results.

## Tools

[Node.js](https://nodejs.dev) is
> Node.js is a free, open-sourced, cross-platform JavaScript run-time environment

[Vega-Lite](https://vega.github.io/vega-lite) is
> is a high-level grammar of interactive graphics.

[Catch2](https://github.com/catchorg/Catch2) is 
> A modern, C++-native, test framework for unit-tests...

## Steps
- The [Catch2 XML Reporter](https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md) is used to generate a XML file with the benchmark data.
- The XML file is converted to a JSON file using [convert-xml-to-json](https://www.npmjs.com/package/convert-xml-to-json).
- The JSON data file is then used to generate the charts using [Vega-Lite](https://vega.github.io/vega-lite).
- [BenchmarkChartHorizontal.json](./BenchmarkChartHorizontal.json) contains the chart description and configuration.
- The command line tool [vl2svg](https://vega.github.io/vega-lite/usage/compile.html#cli) is used to create a SVG vector graphics file with the chart.

## Commands

Run these two commands to setup the project and generate the charts:

```shell
npm ci
npm run build
```

Further commands and details can be found in [COMMANDS.md](./COMMANDS.md).

## References
- [Catch2 XML Reporter](https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md)
- [convert-xml-to-json (NPM)](https://www.npmjs.com/package/convert-xml-to-json)
- [Vega-Lite](https://vega.github.io/vega-lite)
- [Vega-Lite command line tool vl2svg](https://vega.github.io/vega-lite/usage/compile.html#cli)
- [package.json - how to set default value of variable in windows (StackTrace)](https://stackoverflow.com/questions/70219331/package-json-how-to-set-default-value-of-variable-in-windows)
- [Three Things You Didn't Know You Could Do with npm Scripts (Dominik Kundel)](https://www.twilio.com/blog/npm-scripts)