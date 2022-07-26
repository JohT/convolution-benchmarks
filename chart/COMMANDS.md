# Commands
This document lists the steps and commands that were executed to set up chart generation.

## Run benchmarks and generate charts

The following commands are all that is needed to run the benchmark tests and generate the charts (SVG).
All intermediate files and results are written to `..build/benchmark/`.
The final results will be copied into this folder.

```shell
npm ci # to install the dependencies.
npm test # to start the benchmark tests
npm run build # to generate the charts
```

## Scripts
Here is a summary of the most important scripts listed in the [package.json](./package.json) file:

- **test**: Executes all scripts that are needed to run the benchmark tests and to store their results.
- **build**: Executes all scripts that are needed to create the charts. Same as `create-benchmark-charts`.
- **execute-benchmarks**: Runs the executable in `../build/test` with the [catch2](https://github.com/catchorg/Catch2) benchmark tests.
- **convert-xml-to-json**: Converts the benchmark results XML to JSON using [convert-xml-to-json](https://www.npmjs.com/package/convert-xml-to-json).
- **generate-svg-chart**: Generates a bar chart from the benchmark results and stores it as a SVG vector graphics file using [vega-lite](https://vega.github.io/vega-lite).
- **copy-all**: Copies all results from the working directory `..build/benchmark/` to the corresponding directories in this folder.

## Setup commands
Here are some more commands that were used to setup chart generation for this project.

### Install vega-cli on mac os

The following commands were used to install [vega command line utilities](https://vega.github.io/vega/usage/#cli) in the root repository path.
Since it depends on [node-canvas](https://github.com/Automattic/node-canvas) the [node-canvas installation](https://github.com/Automattic/node-canvas#installation) might be needed in advance.

```shell
brew install pkg-config cairo pango librsvg pixman
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

## Generate SVG image chart
The following command generates the SVG chart as described in [Compiling Vega-Lite to Vega using npx](https://vega.github.io/vega-lite/usage/compile.html#using-npx):

```shell
npx -p vega -p vega-lite vl2svg --loglevel info BenchmarkChartHorizontal.json BenchmarkChart.svg
```