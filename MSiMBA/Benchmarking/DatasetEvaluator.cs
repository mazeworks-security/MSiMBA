using Mba.Ast;
using Mba.Common.Minimization;
using Mba.Common.MSiMBA;
using Mba.Parsing;
using Mba.SMT;
using Mba.Utility;
using Microsoft.Z3;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MSiMBA.Benchmarking
{
    public record SimplifiedExpression(string Result, double MsTaken);

    public class DatasetEvaluator
    {
        private readonly string filePath;

        private readonly uint bitWidth;

        public static void Run(string filePath, uint bitWidth)
            => new DatasetEvaluator(filePath, bitWidth).Run();

        private DatasetEvaluator(string filePath, uint bitWidth)
        {
            this.filePath = filePath;
            this.bitWidth = bitWidth;
        }

        private void Run()
        {
            // Filter truth table fetching time out of benchmark,
            // since this only occurs once per process invocation.
            var prefetch = (int count) => TruthTableDatabase.Instance.GetTableEntry(Enumerable.Repeat(new VarNode("X", 64), count).ToList(), 0);
            prefetch(1);
            prefetch(2);
            prefetch(3);
            prefetch(4);

            // Same story with the constant unmerging truth table.
            var entries = TwoVarTruthTable.TwoVarTruthTableEntries;

            // Warmup the JIT
            Warmup();

            // Process the dataset with MSiMBA.
            Console.WriteLine("Parsing dataset entries...");
            var dataset = MbaDataset.LoadFrom(filePath, bitWidth);
            Console.WriteLine($"Simplifying dataset entries...");
            var results = SimplifyDataset(dataset);

            // Compute the average # of milliseconds.
            var msTaken = Sum(results.Select(x => x.result.MsTaken));
            var avgMs = Avg(msTaken, results.Count);
            Console.WriteLine($"Avg ms: {avgMs}");

            // Calculate the average node counts for the input / simplified versions.
            var getCost = (AstNode x) => (double)MultibitSiMBA.GetCost(x, false, (ulong)ModuloReducer.GetMask(bitWidth));
            var allAsts = results.Select(x => AstParser.Parse(x.result.Result, bitWidth));
            var simplifiedNodeCount = Sum(allAsts.Select(x => getCost(x)));
            var groundTruthNodeCount = Sum(dataset.MbaExpressions.Select(x => getCost(x.ParsedGroundTruth)));
            var avgSimplified = Avg(simplifiedNodeCount, results.Count);
            var avgGroundTruth = Avg(groundTruthNodeCount, results.Count);
            Console.WriteLine($"Avg # of nodes: {avgSimplified} / {avgGroundTruth}");
        }

        // Give the CLR sufficient time to JIT our code.
        private void Warmup()
        {
            var strs = new string[]
            {
                "-(7463589017883180317^~(~x&(y|7463589017883180317))) + ((x&~y)|~(x^(y^7463589017883180317))) + ((x&y)|(x^(y^7463589017883180317))) + (y&(x|7463589017883180317))",
                "2*~(x&~45646)-1*~(x^546465456)+3*~(x&~x)-3*~(x|456456)-234234324*~(x|~23424234)-24243*(x&~4234234)",
            };

            var asts = strs.Select(x => AstParser.Parse(x, bitWidth)).ToList();
            var sw = Stopwatch.StartNew();
            while (sw.ElapsedMilliseconds < 5000)
            {
                for(int i = 0; i < asts.Count; i++)
                    MultibitSiMBA.Run(asts[i], false, multibit: true);
            }
        }

        private List<(AstNode groundTruth, SimplifiedExpression result)> SimplifyDataset(MbaDataset dataset)
        {
            var output = new List<(AstNode groundTruth, SimplifiedExpression result)>();
            foreach (var entry in dataset.MbaExpressions)
            {
                var result = SimplifyViaMSimba(entry.ParsedExpr);
                output.Add(new(entry.ParsedGroundTruth, result));
            }
            return output;
        }

        private SimplifiedExpression? SimplifyViaMSimba(AstNode ast)
        {
            var sw = Stopwatch.StartNew();
            var simplified = MultibitSiMBA.Run(ast, false, multibit: true);
            sw.Stop();

            var ms = sw.Elapsed.TotalMilliseconds;
            return new SimplifiedExpression(simplified.ToString(), ms);
        }

        private double Avg(double sum, int count)
        {
            var avg = sum / (double)count;
            avg = Math.Round(avg, 2);
            return avg;
        }

        private double Sum(IEnumerable<double> values)
        {
            double value = 0;
            foreach (var v in values)
                value += v;
            return value;
        }
    }
}
