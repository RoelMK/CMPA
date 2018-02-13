using OptiCom;
using OptiCom.Modelling;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OptiCom.Modelling
{
    public class Model
    {
        private ModelData data;                             // Model constants
        private double targetTime;                          // Target time: when to finish
        private bool isModelInModel;                        // Is modelling for a model
        private double startSpeed;                          // Start speed

        private double t = 0;                               // Time
        private double speed = 0;                           // Speed (m/s)
        private double position = 0;                        // Position between two coils (m)
        private double distance = 0;                        // Distance traveled (m)
        private double rotations = 0;                       // Rotations
        private double coilForce = 0;                       // Force of coil (N)
        private double airResistanceForce = 0;              // Air resistance force (N)
        private double rollingResistanceForce = 0;          // Rolling resistance force (N)
        private double centripetalForce = 0;                // Centripetal force (N)
        private double nettoForce = 0;                      // Netto force (N)
        private double acceleration = 0;                    // Acceleration projectile (m/s²)
        private double coilCurrent = 0;                     // Current coil (A)
        private bool FETOpen = false;                       // Is FET open?
        private double timeFETopen = 0;                     // Time the FET is open
        private double timeFETclosed = 0;                   // Time the FET is closed
        private double lastSpeedMeasurement = 0;            // Last speed measurement (software emulation)
        private double keepFETOpenTime = 0;                 // How long should the FET be opened?
        private bool sensorDetection = false;               // Is sensor detecting projectile?
        private bool started = false;                       // Has modulation started?
        private ModelResult result = null;              // OptiLight result
        private bool FETNeverON = true;                     // First time running?
        private double topSpeed = 0;                        // Highest speed measured

        private Task modelTask;                             // The task

        public Model(ModelData _data, double _startSpeed)
        {
            data = _data;
            startSpeed = _startSpeed;
            targetTime = 1;
            isModelInModel = false;
            modelTask = new Task(Modeling);
        }

        //public Model(ModelData _data, double _targetTime)
        //{
        //    data = _data;
        //    targetTime = _targetTime;
        //    isModelInModel = false;
        //    startSpeed = data.StartSpeed;
        //    modelTask = new Task(Modeling);
        //}

        public Model(ModelData _data, double _currentSpeed, double _currentPosition)
        {
            data = _data;
            startSpeed = _currentSpeed;
            position = data.DistanceBetweenPreviousCoilCoreAndLDR - 0.0001;
            distance = data.DistanceBetweenPreviousCoilCoreAndLDR - 0.0001;
            targetTime = double.MaxValue;
            isModelInModel = true;
            modelTask = new Task(Modeling);
        }

        /// <summary>
        /// Start modelling
        /// </summary>
        /// <param name="wait">Wait for the model to finish</param>
        public void Start(bool wait)
        {
            if(!started)
            {
                started = true;
                modelTask.Start();

                if(wait)
                {
                    modelTask.Wait();
                }
            }
        }

        public bool IsFinished
        {
            get
            {
                return modelTask.IsCompleted;
            }
        }

        public bool IsFailed
        {
            get
            {
                if(result == null && IsFinished)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        public ModelResult ModelResult
        {
            get
            {
                return result;
            }
        }

        public double Speed
        {
            get
            {
                return speed;
            }
        }

        public double TopSpeed
        {
            get
            {
                return topSpeed;
            }
        }

        public double Time
        {
            get
            {
                return t;
            }
        }

        /// <summary>
        /// Modeling
        /// </summary>
        private void Modeling()
        {
            speed = startSpeed;
            lastSpeedMeasurement = startSpeed;

            StreamWriter wStream = null;
            if (!isModelInModel && Program.ExportToFile)
            {
                wStream = new StreamWriter(Program.FileToExportTo, false);
            }

            for (t = data.TimeInterval; t < targetTime; t = t + data.TimeInterval)
            {
                // Calculate basic values
                distance += speed * data.TimeInterval;
                speed += acceleration * data.TimeInterval;
                if(speed <= 0)
                {
                    Console.WriteLine(">> Failed (negative speed) time: " + t);
                    break;
                }
                rotations = Math.Floor(distance / data.DistanceBetweenCoilCores);
                double previousPosition = position;
                position = distance - data.DistanceBetweenCoilCores * rotations;
                if (rotations > 0 && isModelInModel)
                {
                    break;
                }

                if (!isModelInModel && Program.ExportToFile)
                {
                    WriteAllData(wStream, 10);
                }

                // LDR state
                sensorDetection = IsSensorDetectingProjectile(previousPosition);
                if (sensorDetection)
                {
                    lastSpeedMeasurement = speed;
                }

                // FET open/closed
                // -> Open timing
                if (FETOpen)
                {
                    timeFETopen += data.TimeInterval;
                    timeFETclosed = 0;
                }
                else
                {
                    timeFETopen = 0;
                    timeFETclosed += data.TimeInterval;
                }
                // -> Is open?
                if (keepFETOpenTime >= data.TimeInterval)
                {
                    FETOpen = true;
                    FETNeverON = false;
                }
                else
                {
                    FETOpen = false;
                }
                // -> How long open?
                keepFETOpenTime = EstimateFETOpenTime();

                // Calculate current
                coilCurrent = CalculateCoilCurrent();

                // Calculate forces
                coilForce = CalculateCoilForce();
                airResistanceForce = 0.5 * data.KValue * Math.Pow(speed, 2);
                centripetalForce = (data.WeightProjectile * Math.Pow(speed, 2)) / data.RadiusAccelerationTube;
                rollingResistanceForce = centripetalForce * data.RollingResistanceCoefficient;
                nettoForce = coilForce - (airResistanceForce + rollingResistanceForce);

                // Set top speed
                if(speed > topSpeed)
                {
                    topSpeed = speed;
                }

                // Acceleration
                acceleration = nettoForce / data.WeightProjectile;

                // Check if model is finished & return if it is finished
                if(!FETNeverON && !FETOpen && !isModelInModel)
                {
                    result = new ModelResult(startSpeed, t);
                    break;
                }
            }

            if (!isModelInModel && Program.ExportToFile)
            {
                wStream.Close();
            }
        }

        /// <summary>
        /// Estimate time the projectile needs to reach the next coil
        /// </summary>
        /// <returns>Estimated time</returns>
        private double EstimateTimeToReachCoilCore()
        {
            Model modelInModel = new Model(data, speed, position);
            modelInModel.Start(true);
            return modelInModel.Time;
        }

        /// <summary>
        /// Estimate the FET time
        /// </summary>
        /// <returns>Estimated time</returns>
        private double EstimateFETOpenTime()
        {
            if (sensorDetection)
            {
                if(!isModelInModel)
                {
                    return EstimateTimeToReachCoilCore() - data.CurrentTo0Time;
                }
                else
                {
                    return double.MaxValue;
                }

            }
            else
            {
                if (keepFETOpenTime > data.TimeInterval)
                {
                    return keepFETOpenTime - data.TimeInterval;
                }
                else
                {
                    return 0;
                }
            }
        }

        /// <summary>
        /// Is sensor covering LDR?
        /// </summary>
        /// <param name="previousPosition">Previous x</param>
        /// <returns>Is covering?</returns>
        private bool IsSensorDetectingProjectile(double previousPosition)
        {
            if(position >= data.DistanceBetweenPreviousCoilCoreAndLDR && previousPosition < data.DistanceBetweenPreviousCoilCoreAndLDR)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Calculate current in coil
        /// </summary>
        /// <returns>Current (I)</returns>
        private double CalculateCoilCurrent()
        {
            if(FETOpen)
            {
                return data.TimeCurrentIncrease.GetCurrent(timeFETopen) * data.CoilCurrentCorrectionFactor;
            }
            else
            {
                if(!FETNeverON)
                {
                    return data.TimeCurrentDecrease.GetCurrent(timeFETclosed) * data.CoilCurrentCorrectionFactor;
                }
                else
                {
                    return 0;
                }
            }
        }

        /// <summary>
        /// Calculate coil force
        /// </summary>
        /// <returns>Force (N)</returns>
        private double CalculateCoilForce()
        {
            double gap = data.DistanceBetweenCoilCores - position;
            if(gap < data.CoilPowerFormulaCorrection)
            {
                gap = data.CoilPowerFormulaCorrection;
            }

            if(position < data.DistanceBetweenPreviousCoilCoreAndLDR && rotations > 0)    // position < (0.1 * data.DistanceBetweenCoilCores)
            {
                return -((Math.Pow(data.CoilWindings * coilCurrent, 2) * data.VacuumPermeability * data.SurfaceCoil) / (2 * Math.Pow(data.DistanceBetweenCoilCores - gap, 2)));
            }
            else
            {
                return (Math.Pow(data.CoilWindings * coilCurrent, 2) * data.VacuumPermeability * data.SurfaceCoil) / (2 * Math.Pow(gap, 2));
            }
            
        }

        private bool hasWrittenOpenString = false;
        private int writeDelay = 0;
        private void WriteAllData(StreamWriter wStream, int delayTarget)
        {
            if(writeDelay >= delayTarget)
            {
                if (!hasWrittenOpenString)
                {
                    wStream.WriteLine("time speed distance position rotations Fcoil Fwl Fwr Fmpz Fnetto acceleration Icoil FETopen timeFETopen timeFETclosed lastSpeed keepFETopenTime");
                    hasWrittenOpenString = true;
                }
                wStream.WriteLine(t + " " + speed + " " + distance + " " + position + " " + rotations + " " + coilForce + " " + airResistanceForce + " " + rollingResistanceForce + " " + centripetalForce + " " + nettoForce + " " + acceleration + " " + coilCurrent + " " + FETOpen + " " + timeFETopen + " " + timeFETclosed + " " + lastSpeedMeasurement + " " + keepFETOpenTime);
                writeDelay = 0;
            }
            else
            {
                writeDelay++;
            }
        }
    }
}
