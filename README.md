# MSiMBA

MSiMBA is a tool designed for the simplification of mixed boolean-arithmetic expressions. It extends [SiMBA](https://github.com/DenuvoSoftwareSolutions/SiMBA) with a novel algorithm for the deobfuscation of semi-linear MBAs, leveraging new insights that a semi-linear MBA is fully determined by an evaluation for all combinations of zeroes and ones applied individually to each bit position.

The details of this algorithm can be found in the [paper](https://arxiv.org/abs/2406.10016) "*Deobfuscation of Semi-Linear Mixed Boolean-Arithmetic Expressions*".

# Using MSiMBA
MSiMBA provides a single executable, `MSiMBA.exe` for simplifying MBA expressions. To simplify a single expression, use:
```
MSiMBA.exe "expr"
```

As an example:
```
$ MSiMBA.exe "7471873370*~(16832296021645948&y) + 3735936685*16832296021645948 + 3735936685*y + 7471922744 - 49374"

Expression: (((((7471873370*(~(16832296021645948&y)))+(3735936685*16832296021645948))+(3735936685*y))+7471922744)-49374)

Simplified to: (3735936685*(16832296021645948^y))
```

The executable accepts several arguments, e.g. `-b` for specifying the bit width of the input expression, and `-z` for proving the equivalence between the input expression and the simplified result. Use the command line option `-h` to see the available settings.

# Embedding MSiMBA
The underlying MBA simplifier is implemented as a library, `Mba.Common`, and can be easily embedded into other projects.

# Format of MBAs 
MSiMBA uses the same input format as SiMBA. Documentation for this format can be found in SiMBA's readme section.

An ANTLR grammar for this format can be found [here](https://github.com/mazeworks-security/MSiMBA/blob/0f4a1ebb63425f5722a70aa98fed29f4a48a9b44/Mba.Common/Parsing/Expr.g4).

# Algorithms and linearity checking
By default, MSiMBA runs the semi-linear simplification algorithm on all input expressions. The original SiMBA algorithm can be enabled instead via the `-linear` option.

If the `-csl` option is used, MSiMBA will check if input expression is semi-linear, and report an error if not. Similarly, `-cl` can be used to enable a check for linearity.

# Datasets and reproduction of experiments
The datasets can be found in the `Datasets` directory of `MSiMBA`. We include all datasets from the paper, aswell as the 5/6 variable datasets from the SiMBA repository.

To process a dataset with MSiMBA, use:
```
MSiMBA.exe -dataset "path/to/dataset"
```

MSiMBA must be built in release mode when reproducing the experiments. After processing the dataset, MSiMBA will output runtime and quality statistics. 

# Building MSiMBA
Building MSiMBA requires .NET 8 and Visual Studio 2022 w/ ClangCL. Note that only Windows is supported.

To build MSiMBA:
- Clone the repository, using the `--recursive` flag to pull submodules (Espresso).
- Open `MSiMBA.sln` with visual studio
- Change the build configuration from `Debug` to `Release`
- Build the solution
