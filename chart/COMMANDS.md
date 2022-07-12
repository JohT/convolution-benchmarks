# Commands
This document lists the steps and commands that were executed to set up chart generation.

## Run benchmarks and generate charts

The following two commands are all that is needed to run the benchmark tests and generate the charts (svg and png).
All intermediate files and results are written to `..build/benchmark/`.

```shell
npm ci
npm run create-benchmark-charts
```

## Scripts
Here is a summary of all scripts listed in the [package.json](./package.json) file:

- **create-benchmark-charts**: Executes all scripts that are needed to run the benchmark tests and generate the reports and charts of them.
- **create-benchmark-directory**: Creates the directory `benchmark` inside the `build` directory using [mkdirp](https://www.npmjs.com/package/mkdirp).
- **run-benchmarks**: Runs the executable in `../build/test` with the [catch2](https://github.com/catchorg/Catch2/tree/v2.13.2) benchmark tests.
- **convert-xml-to-json**: Converts the benchmark results XML to JSON using [convert-xml-to-json](https://www.npmjs.com/package/convert-xml-to-json).
- **generate-png-chart**: Generates a bar chart from the benchmark results and stores it as a PNG picture file using [vega-lite](https://vega.github.io/vega-lite).
- **generate-svg-chart**: Generates a bar chart from the benchmark results and stores it as a SVG vector graphics file using [vega-lite](https://vega.github.io/vega-lite).

## Setup commands
Here are some more commands that were used to setup chart generation for this project.

### Install vega-cli on mac os

The following commands were used to install [vega command line utilities](https://vega.github.io/vega/usage/#cli) in the root repository path.
Since it depends on [node-canvas](https://github.com/Automattic/node-canvas) the [node-canvas installation](https://github.com/Automattic/node-canvas#installation) might be needed in advance.

```shell
brew install pkg-config cairo pango libpng jpeg giflib librsvg pixman
npm install --save-dev canvas vega-cli
```

### Install vega-lite

In root repository path execute:

```shell
npm install --save-dev vega-lite
```

### Convert XML to JSON

The following command uses [NPM convert-xml-to-json](https://www.npmjs.com/package/convert-xml-to-json) to convert a XML to a JSON file:
```shell
npx --yes convert-xml-to-json ../build/test/test-xml-report.xml test-xml-report.json
```

## Generate PNG image chart
The following command generates the PNG chart as described in [Compiling Vega-Lite to Vega using npx](https://vega.github.io/vega-lite/usage/compile.html#using-npx):

```shell
npx -p vega -p vega-lite vl2png --loglevel debug BenchmarkChart.json BenchmarkChart.png
```

## Generate SVG image chart
The following command generates the PNG chart as described in [Compiling Vega-Lite to Vega using npx](https://vega.github.io/vega-lite/usage/compile.html#using-npx):

```shell
npx -p vega -p vega-lite vl2svg --loglevel info BenchmarkChart.json BenchmarkChart.svg
```