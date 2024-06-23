// Parse the arguments
using Mba.Ast;
using Mba.Common.MSiMBA;
using Mba.Parsing;
using Mba.SMT;
using Mba.Utility;
using Microsoft.Z3;
using Simplifier;

bool printUsage = false;
uint bitWidth = 64;
bool proveEquivalence = false;
bool checkLinear = false;
bool checkSemiLinear = false;
bool simplifyAsLinearMba = false;
bool processDataset = false;
string inputText = null;

var printHelp = () =>
{
    Console.WriteLine("Usage: Simplifier.exe");
    Console.WriteLine("Command line input not preceded by the option indicators below are considered to be input expressions. Only one input expression is accepted.");
    Console.WriteLine("Command line options:");
    Console.WriteLine("    -h:        print usage");
    Console.WriteLine("    -b:        specify the bit number of variables (default is 64)");
    Console.WriteLine("    -z:        enable a check for valid simplification using Z3");
    Console.WriteLine("    -cl:       enable a check for input expressions being linear MBAs");
    Console.WriteLine("    -csl:      enable a check for input expressions being semi-linear MBAs");
    Console.WriteLine("    -linear:   disable semi-linear simplification methods (default is true)");
    Console.WriteLine("    -dataset:  process the MBA dataset at the provided path");
};

for (int i = 0; i < args.Length; i++)
{
    var arg = args[i];
    switch(arg)
    {
        case "-h":
            printUsage = true;
            break;
        case "-b":
            bitWidth = uint.Parse(args[i+1]);
            i++;
            break;
        case "-z":
            proveEquivalence = true;
            break;
        case "-cl":
            checkLinear = true;
            break;
        case "-csl":
            checkSemiLinear = true;
            break;
        case "-linear":
            simplifyAsLinearMba = true;
            break;
        case "-dataset":
            processDataset = true;
            break;
        default:
            if (inputText != null)
                throw new ArgumentException($"Found more than one expression argument. Received both {inputText} and {args[i]}");
            inputText = args[i];
            break;
    }
}

if(inputText == null || printUsage)
{
    printHelp();
    return;
}

if(processDataset)
{
    var datasetPath = inputText;
    if (!File.Exists(datasetPath))
    {
        Console.WriteLine($"Failed to find dataset at the provided path! File does not exist at {datasetPath}");
        return;
    }
    DatasetEvaluator.Run(datasetPath, bitWidth);
    return;
}

// For now we only support integer widths of up to 64 bits.
const int maxWidth = 64;
if (bitWidth > maxWidth)
    throw new InvalidOperationException($"Received bit width {bitWidth}, which is greater than the max width {maxWidth}");

AstNode input = null;
try
{
    input = AstParser.Parse(inputText, bitWidth);
}
catch(Exception ex)
{
    var color = Console.ForegroundColor;
    Console.ForegroundColor = ConsoleColor.Red;
    Console.WriteLine($"Failed to parse input expression: {input}\n");
    Console.WriteLine($"{ex.ToString()}");
    Console.ForegroundColor = color;
    return;
}

Console.WriteLine($"\nExpression: {input}\n");

// Perform a linearity check.
if (checkLinear || checkSemiLinear)
{
    // Classify the MBA.
    var classification = AstClassifier.Classify(input)[input];

    if (checkLinear && (classification == AstClassification.BitwiseWithConstants || classification == AstClassification.SemiLinear || classification == AstClassification.Nonlinear))
        throw new InvalidOperationException($"Linearity check failed! Expression is {classification}, but a linear MBA was expected.");
    // Note that a linear MBA can still be validly processed as if it was a semilinear MBA. 
    if (checkSemiLinear && classification == AstClassification.Nonlinear)
        throw new InvalidOperationException($"Semi-linearity check failed! Expression is nonlinear, but a linear or semi-linear MBA was expected.");
}

// Note that if simplifyAsLinearMba is enabled, we apply the original SiMBA algorithm,
// which will only output a correct result for linear MBAs.
var simplified = MultibitSiMBA.Run(input, alreadySplit: false, multibit: !simplifyAsLinearMba);

Console.WriteLine($"Simplified to: {simplified}\n");

if (!proveEquivalence)
    return;

var ctx = new Context();
var translator = new AstToZ3(bitWidth, ctx);
var before = translator.Translate(input);
var after = translator.Translate(simplified);
var solver = ctx.MkSolver("QF_BV");

// Set the maximum timeout to 10 seconds.
var p = ctx.MkParams();
uint timeout = 10000;
p.Add("timeout", timeout);
solver.Parameters = p;

Console.WriteLine("Proving equivalence...\n");
solver.Add(ctx.MkNot(ctx.MkEq(before, after)));
var check = solver.Check();

var printModel = (Model model) =>
{
    var values = model.Consts.Select(x => $"{x.Key.Name} = {(long)ulong.Parse(model.Eval(x.Value).ToString())}");
    return $"[{String.Join(", ", values)}]";
};

if(check == Status.UNSATISFIABLE)
    Console.WriteLine("Expressions are equivalent.");
else if(check == Status.SATISFIABLE)
    Console.WriteLine($"Expressions are not equivalent. Counterexample:\n{printModel(solver.Model)}");
else
    Console.WriteLine($"Solver timed out. Could not find counterexample within {timeout}ms");