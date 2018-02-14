using ExcelDataReader;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;

namespace OptiCom.Modelling
{
    public class ModelData
    {
        public readonly double TimeInterval = 0.00001;      // s

        public readonly double VacuumPermeability = 4 * Math.PI * Math.Pow(10, -7);
        public readonly double ResistivityCopper = 1.75e-5;

        public readonly double DistanceBetweenCoilCores;                // AUTO
        public readonly double DistanceBetweenPreviousCoilCoreAndLDR = 0.02;   // m
        public readonly double RadiusAccelerationTube;                  // AUTO
        public readonly double LengthAccelerationTube = 4.71;         // m
        public readonly double NumberOfCoils = 16;                   
        public readonly double ExtraLengthAccelerationTube = 0.2;    // m

        public readonly int CoilWindings;           // AUTO
        public readonly double MaxCurrentCoil;      // AUTO
        public readonly double SurfaceCoil;         // AUTO
        public readonly double ResistanceCoil;      // AUTO
        public readonly int CoilWindingLayers = 5;
        public readonly double CoilWireDiameter = 1.2;    // mm
        public readonly double CoilLength = 26.4;         // mm
        public readonly double CoilVoltage = 12;        // V
        public readonly double CoilPowerFormulaCorrection = 0.01;   // m
        public readonly double CurrentTo0Time = 3e-03;    // s old: 1.24e-03

        public readonly double SurfaceProjectile;       // AUTO
        public readonly double DiameterProjectile = 0.010;  // m 0.018
        public readonly double WeightProjectile = 0.0042;    // kg 0.033
        public readonly double RollingResistanceCoefficient = 0.05;
        public readonly double AirResistanceCoefficient = 0.5;
        public readonly double KValue;  // AUTO

        public readonly double DiameterTube;    // AUTO
        public readonly double ThicknessTubeWall = 0.0025;  // m
        public readonly double ExtraDiameterTube = 0.002;   // m
        public readonly double AirTightness = 1.293;   // kg/m³ (90% vac)

        public readonly double CoilCurrentCorrectionFactor = 0.2;
        public readonly double MinGapNoCoilForceImpact = 0.045;

        public TimeCurrent TimeCurrentIncrease;
        public TimeCurrent TimeCurrentDecrease;


        public ModelData(double distanceBetweenCoilCores)
        {
            // Set constants
            RadiusAccelerationTube = (LengthAccelerationTube + ExtraLengthAccelerationTube) / (2 * Math.PI);
            DistanceBetweenCoilCores = distanceBetweenCoilCores; // LengthAccelerationTube / NumberOfCoils;
            //DistanceBetweenPreviousCoilCoreAndLDR = DistanceBetweenCoilCores / 2;

            DiameterTube = ThicknessTubeWall * 2 + DiameterProjectile + ExtraDiameterTube;

            CoilWindings = (int) (CoilLength / CoilWireDiameter * CoilWindingLayers);

            ResistanceCoil = (4 * ResistivityCopper * (Math.Pow((DiameterTube / 2 * 1000) + (CoilWindingLayers * CoilWireDiameter), 2) - Math.Pow(DiameterTube / 2 * 1000, 2)) * CoilLength) / (Math.Pow(CoilWireDiameter, 4));
            MaxCurrentCoil = CoilVoltage / ResistanceCoil;
            SurfaceCoil = Math.PI * Math.Pow(DiameterTube, 2);
            
            SurfaceProjectile = Math.PI * Math.Pow(DiameterProjectile / 2, 2);
            KValue = AirTightness * AirResistanceCoefficient * SurfaceProjectile;

            // Import Excel
            TimeCurrentIncrease = new TimeCurrent();
            TimeCurrentDecrease = new TimeCurrent();
         
            using (var stream = File.Open("LTSpiceData.xlsx", FileMode.Open, FileAccess.Read))
            {
                using (var reader = ExcelReaderFactory.CreateReader(stream))
                {
                    do
                    {
                        while (reader.Read())
                        {
                            try
                            {
                                TimeCurrentIncrease.Add(reader.GetDouble(0), reader.GetDouble(1));
                                TimeCurrentDecrease.Add(reader.GetDouble(2), reader.GetDouble(3));
                            }
                            catch (NullReferenceException)
                            {
                                break;
                            }
                        }
                    } while (reader.NextResult());
                }
            }
        }

        public void PrintCalculatedModelData()
        {
            Console.WriteLine("DistanceBetweenCoilCores: " + DistanceBetweenCoilCores + " m");
            Console.WriteLine("DistanceBetweenPreviousCoilCoreAndLDR: " + DistanceBetweenPreviousCoilCoreAndLDR + " m");
            Console.WriteLine("RadiusAccelerationTube: " + RadiusAccelerationTube + " m");
            Console.WriteLine("CoilWindings: " + CoilWindings + "");
            Console.WriteLine("MaxCurrentCoil: " + MaxCurrentCoil + " A");
            Console.WriteLine("SurfaceCoil: " + SurfaceCoil + " m²");
            Console.WriteLine("ResistanceCoil: " + ResistanceCoil + " Ω");
            Console.WriteLine("SurfaceProjectile: " + SurfaceProjectile + " m²");
            Console.WriteLine("DiameterTube: " + DiameterTube + " m");
            Console.WriteLine("VacuumPermeability: " + VacuumPermeability + " H/m");
        }
    }
}
