package cpu_test

// Tests are defined as JSON files in testdata/ and run by TestFromFiles in harness_test.go.
//
// To add a test:
//   1. Create testdata/<name>.json (see schema in harness_test.go for format).
//   2. Optionally create testdata/<name>.bin for programs that are easier to
//      express as a binary than as inline hex.
//
// Example single-case file (testdata/my_test.json):
//
//   {
//     "name": "my instruction test",
//     "hex": "38 42 00 01",
//     "expect": {
//       "registers": { "A": "42" },
//       "flags":     { "Z": false }
//     }
//   }
//
// Example multi-case file (array at top level):
//
//   [
//     { "name": "case 1", "hex": "...", "expect": { ... } },
//     { "name": "case 2", "file": "program.bin", "expect": { ... } }
//   ]
