using OptiCom.Modelling;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;

namespace OptiCom
{
    class Program
    {
        // Modelling
        public const bool ExportToFile = false;                     // Export modelling data to file
        public const string FileToExportTo = "H:\\export.txt";      // File to export modelling data to 

        private const int NumberOfFETs = 1;             // Number of FETs
        private const int TopSpeedCMPA = 5;             // Top speed CMPA (m/s)
        private const double DeltaVLow_VHigh = 0.1;     // Difference between VLow and VHigh in OptiLight data
        private const double DistanceBetweenCoils = 0.2628;                 // Distance between two coils 
        private const double DistanceBetweenCoils_ENTRANCE_EXIT = 0.4228;   // Distance between two coils when entrace / exit is between
        private const int EntranceFET = 7;      // FET close to the entrance
        private const int ExitFET = 15;         // FET close to the exit

        // Arduino communication
        private const string ArduinoPortName = "COM3";  // Name of serial port
        private const int ArduinoBitRate = 38400;       // Bit rate

        private static SerialPort ArduinoPort = new SerialPort(ArduinoPortName, ArduinoBitRate);       // Arduino serial port properties
        private static readonly string OptiLightDataFile = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\OptiLightData.ini";    // Folder to save OptiLight data


        // Logging
        private static Stopwatch watch = new Stopwatch();   // Watch
        private static StreamWriter logger;                 // Normal logger
        private static StreamWriter speedLogger;            // Speed only logger

        /// <summary>
        /// Main
        /// </summary>
        /// <param name="args">Arguments</param>
        static void Main(string[] args)
        {
            // Check for command to generate OptiLight data
            if (args.Length > 0)
            {
                if(args[0] == "-g" && args.Length > 1)
                {
                    StreamWriter wStream;

                    try
                    {
                        wStream = new StreamWriter(args[1]);    
                    }
                    catch (Exception)
                    {
                        Console.WriteLine("Failed to write to file: " + args[1]);
                        Console.ReadKey();
                        return;
                    }

                    OptiLightGenerator generator = new OptiLightGenerator(NumberOfFETs, TopSpeedCMPA, DeltaVLow_VHigh, DistanceBetweenCoils, DistanceBetweenCoils_ENTRANCE_EXIT, EntranceFET, ExitFET);
                    bool generationResult = generator.Generate();
                    if(generationResult)
                    {
                        string generatedText = generator.GetOptiLightDataArrayInOptiComFormat();
                        wStream.WriteLine(generatedText);
                        wStream.Close();
                        Console.WriteLine("--- OptiLight data saved ---");
                        return;
                    }
                    else
                    {
                        wStream.Close();
                        Console.WriteLine("--- OptiLight data generation failed ---");
                        return;
                    }
                }
            }

            // Start OptiCom
            Console.WriteLine("[INFO] Opening serial port...");
            watch.Start();  // Start stopwatch
            logger = new StreamWriter(DateTime.Now.ToString("ddMMyyyy.HHmmss") + ".log", true); // Set logger
            logger.AutoFlush = true;
            speedLogger = new StreamWriter("speed_" + DateTime.Now.ToString("ddMMyyyy.HHmmss") + ".log", true);
            speedLogger.AutoFlush = true;

            // Open serial port
            try
            {
                ArduinoPort.DtrEnable = true;
                ArduinoPort.RtsEnable = true;
                ArduinoPort.Handshake = Handshake.None;
                ArduinoPort.ReadBufferSize = 8092;
                ArduinoPort.Open();
                ArduinoPort.DataReceived += ArduinoPort_DataReceived;
                ArduinoPort.ErrorReceived += ArduinoPort_ErrorReceived;
            }
            catch (Exception)
            {
                bool retry = PrintError("[ERROR] Failed to open serial port!", true);
                if(retry)
                {
                    Console.Clear();
                    Main(args);
                }
                logger.Close();
                speedLogger.Close();
                return;
            }

            // Wait
            ConsoleKey key = ConsoleKey.A;
            do
            {
                while (!Console.KeyAvailable)
                {
                    Thread.Sleep(1);
                }

                key = Console.ReadKey(true).Key;
                if(key == ConsoleKey.S)
                {
                    ArduinoPort.WriteLine("S");
                    key = ConsoleKey.A;
                }

            } while (key != ConsoleKey.Escape);

            // Exit
            bool closedPort = false;

            while(!closedPort)
            {
                try
                {
                    ArduinoPort.Close();
                    closedPort = true;
                }
                catch (Exception)
                {
                    bool retry = PrintError("[ERROR] Failed to close serial port!", true);
                    if (!retry)
                    {
                        logger.Close();
                        speedLogger.Close();
                        return;
                    }
                }
            }

            Console.ForegroundColor = ConsoleColor.White;
            Console.BackgroundColor = ConsoleColor.Blue;
            Console.WriteLine();
            Console.WriteLine("[INFO] Serial port closed");
            Console.WriteLine("Press R to restart");
            Console.WriteLine("Press any other key to exit");
            Console.ResetColor();

            ConsoleKeyInfo keyPressed = Console.ReadKey();
            if (keyPressed.Key == ConsoleKey.R)
            {
                Process.Start(System.Reflection.Assembly.GetExecutingAssembly().CodeBase);
            }
            logger.Close();
            speedLogger.Close();
            return;
        }

        private static void ArduinoPort_ErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            Console.WriteLine(e.ToString());
        }

        //private static bool receivingSerialData;        // Received serial data
        //private static string serialDataReceived;       // Is receiving serial data?
        /// <summary>
        /// Receive serial data
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void ArduinoPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            // Receive serial data
            //bool executeSerialInput = false;
            try
            {
                string serialData = ArduinoPort.ReadLine();     // Read serial buffer
                serialData = new string(serialData.Where(c => !char.IsControl(c)).ToArray());       // Remove all control chars

                if (serialData.Contains("*"))
                {
                    if (serialData.Contains("G%"))
                    {
                        if (File.Exists(OptiLightDataFile))
                        {
                            Console.WriteLine("[INFO] Reading and sending OptiCom data...");
                            StreamReader rStream = new StreamReader(OptiLightDataFile);
                            string data = rStream.ReadLine();
                            ArduinoPort.Write(data);
                            rStream.Close();
                        }
                        else
                        {
                            ArduinoPort.WriteLine("NODATA");
                        }
                    }
                    else if (serialData.Contains("S%"))
                    {
                        string dataToWrite = serialData.Replace("*S%", "");
                        WriteOptiLightData(dataToWrite);
                        ArduinoPort.WriteLine("SETOK");

                    }
                    else
                    {
                        Console.WriteLine("Received unknown OptiCom command");
                    }
                }
                else
                {
                    Console.WriteLine(watch.ElapsedMilliseconds + " " + serialData);
                    if(logger != null)
                    {
                        logger.WriteLine(watch.ElapsedMilliseconds + " " + serialData);
                    }
                    if(speedLogger != null && serialData.Contains("[INFO] Speed: "))
                    {
                        speedLogger.WriteLine(watch.ElapsedMilliseconds + " " + serialData.Replace("[INFO] Speed: ", ""));
                    }
                }
            }
            catch (Exception)
            {
                Console.WriteLine("[WARNING] Serial I/O error");
                return;
            }
        }

        /// <summary>
        /// Write OptiLight data to file
        /// </summary>
        /// <param name="data">Data to write</param>
        private static void WriteOptiLightData(string data)
        {
            StreamWriter wStream = new StreamWriter(OptiLightDataFile, false);
            wStream.WriteLine(data.Replace(" ", ""));
            wStream.Close();
        }

        /// <summary>
        /// Print error
        /// </summary>
        /// <param name="message">Message</param>
        /// <param name="retry">Can retry?</param>
        /// <returns>Wants to retry?</returns>
        private static bool PrintError(string message, bool retry = false)
        {
            Console.BackgroundColor = ConsoleColor.Red;
            Console.ForegroundColor = ConsoleColor.White;
            Console.WriteLine(message);
            Console.WriteLine();
            if(retry)
            {
                Console.WriteLine("Press R to retry");
                Console.WriteLine("Press any other to exit");

            }
            else
            {
                Console.WriteLine("Press any key to exit...");
            }

            Console.ResetColor();
            ConsoleKeyInfo keyPressed = Console.ReadKey();
            if (keyPressed.Key == ConsoleKey.R)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
    }
}
