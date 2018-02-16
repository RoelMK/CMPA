using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace OptiCom.Modelling
{
    public class OptiLightGenerator
    {
        public const string MAGIC_NUMBER = "255";   // Magic ID (for checks)

        private readonly int FETCount;              // Number of FETs
        private readonly double TopSpeed;           // Top speed, do not generate for speeds higher than this speed
        private readonly double LengthOfSpeedBlock; // 'Length' of a speed block (VHigh - VLow)
        private readonly double DistanceBetweenCoils;                       // Distance between two coils
        private readonly double DistanceBetweenCoilsWithEntraceOrExit;      // Distance between two coils when exit / entrance is between
        private readonly int EntranceFET;   // FET which is close to the entrance
        private readonly int ExitFET;       // FET which is close to the exit


        private List<FETOptiLightBlock> FETBlock = new List<FETOptiLightBlock>();   // Data / FET

        public OptiLightGenerator(int fetCount, double topSpeed, double lengthOfSpeedBlock, double distanceBetweenCoils, double distanceBetweenCoilsWithEntraceOrExit, int entranceFET, int exitFET)
        {
            FETCount = fetCount;
            for(int i = 0; i < fetCount; i++)
            {
                FETBlock.Add(new FETOptiLightBlock());
            }
            TopSpeed = topSpeed;
            LengthOfSpeedBlock = lengthOfSpeedBlock;
            DistanceBetweenCoils = distanceBetweenCoils;
            DistanceBetweenCoilsWithEntraceOrExit = distanceBetweenCoilsWithEntraceOrExit;
            EntranceFET = entranceFET;
            ExitFET = exitFET;
        }

        /// <summary>
        /// Start OptiLight generator
        /// </summary>
        public bool Generate()
        {
            Console.WriteLine(">> Generating OptiLight data...");
            StreamWriter wStream = new StreamWriter(Program.FileToExportTo);
            wStream.WriteLine("startspeed time topspeed distance");
            // For every FET
            for(int f = 0; f < FETCount;f++)
            {
                Console.WriteLine("FET: " + f.ToString() + "/" + FETCount);

                // Create speed blocks
                for (double v = 0; v < TopSpeed; v = v + LengthOfSpeedBlock)
                {
                    //Console.WriteLine("");
                    //Console.WriteLine(">> Start speed: " + v.ToString() + " m/s");


                    // Run model
                    double distance;
                    if (f == EntranceFET || f == ExitFET)
                    {
                        distance = DistanceBetweenCoilsWithEntraceOrExit;
                    }
                    else
                    {
                        distance = DistanceBetweenCoils;
                    }

                    Model model = new Model(new ModelData(distance), (v + v + LengthOfSpeedBlock) / 2);     // Create model object, set start speed
                    model.Start(false);
                    while(!model.IsFinished)
                    {
                        //Console.WriteLine("Current time: {0} s              ", model.Time);
                        //Console.SetCursorPosition(0, Console.CursorTop - 1);
                    }

                    // Failed?
                    if(model.IsFailed || model.ModelResult.EstimatedFETOffTime == 0)
                    {
                        wStream.Close();
                        return false;
                    }
                    else
                    {
                        FETBlock[f].SpeedData.Add(new OptiLightData(v, v + LengthOfSpeedBlock, model.ModelResult.EstimatedFETOffTime));

                        double speed = (v + v + LengthOfSpeedBlock) / 2;
                        wStream.WriteLine(speed + " " + (int)(model.ModelResult.EstimatedFETOffTime * 1000) + " " + model.Speed + " " + model.Distance);
                    }
                }
            }

            // Ready
            wStream.Close();
            Console.WriteLine(">> OptiLight data ready!");
            return true;
        }

        /// <summary>
        /// Convert model data to string which OptiCom can use
        /// </summary>
        /// <returns>OptiCom string</returns>
        public string GetOptiLightDataArrayInOptiComFormat()
        {
            string toReturn = MAGIC_NUMBER;
            toReturn = "";

            for(int f =0; f<FETCount;f++)
            {
                //toReturn += ("|" + FETBlock[f].ToString());
                toReturn += "{" + FETBlock[f].ToString() + "},";
            }

            toReturn.Remove(toReturn.Length - 1);
            return toReturn;
        }
    }
    

    public class FETOptiLightBlock
    {
        public List<OptiLightData> SpeedData;   // Data / speed

        public FETOptiLightBlock()
        {
            SpeedData = new List<OptiLightData>();
        }

        public override string ToString()
        {
            string toReturn = OptiLightGenerator.MAGIC_NUMBER;
            toReturn = "";
            for (int s = 0; s < SpeedData.Count; s++)
            {
                //toReturn += ("@" + SpeedData[s].ToString());
                toReturn += "{" + SpeedData[s].ToString() + "},";
            }
            toReturn.Remove(toReturn.Length - 1);
            return toReturn;
        }
    }
}
