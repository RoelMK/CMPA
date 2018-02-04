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
        private const string ArduinoPortName = "COM3";  // Name of serial port
        private const int ArduinoBitRate = 38400;      // Bit rate

        private static SerialPort ArduinoPort = new SerialPort(ArduinoPortName, ArduinoBitRate);       // Arduino serial port properties
        private static readonly string OptiLightDataFile = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\OptiLightData.ini";    // Folder to save OptiLight data

        /// <summary>
        /// Main
        /// </summary>
        /// <param name="args">Arguments</param>
        static void Main(string[] args)
        {
            Console.WriteLine("[INFO] Opening serial port...");

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
                    Console.WriteLine(serialData);
                }
            }
            catch (Exception)
            {
                Console.WriteLine("[WARNING] Serial I/O error");
                return;
            }


            //// Check data
            //if (serialData.StartsWith("$") && serialData.EndsWith("#"))
            //{
            //    serialDataReceived = serialData;
            //    receivingSerialData = false;
            //    executeSerialInput = true;
            //}
            //else if (serialData.StartsWith("$"))
            //{
            //    receivingSerialData = true;
            //    serialDataReceived = serialData;
            //}
            //else if (receivingSerialData)
            //{
            //    serialDataReceived += serialData;
            //    if (serialData.EndsWith("#"))
            //    {
            //        receivingSerialData = false;
            //        executeSerialInput = true;
            //    }
            //}
            //else
            //{
            //    Console.WriteLine("[DEBUG] " + serialData);
            //}

            //// Execute serial input commands or print serial input
            //if (executeSerialInput)
            //{
            //    string executeData = serialDataReceived.Replace("$", "");
            //    executeData = executeData.Replace("#", "");

            //    if (executeData.Contains("*"))
            //    {
            //        if (executeData.Contains("G%"))
            //        {
            //            if (File.Exists(OptiLightDataFile))
            //            {
            //                StreamReader rStream = new StreamReader(OptiLightDataFile);
            //                string data = rStream.ReadLine();
            //                ArduinoPort.Write(data);
            //                rStream.Close();
            //            }
            //            else
            //            {
            //                ArduinoPort.Write("NODATA");
            //            }
            //        }
            //        else if (executeData.Contains("S%"))
            //        {
            //            string dataToWrite = executeData.Replace("*S%", "");
            //            WriteOptiLightData(dataToWrite);
            //            ArduinoPort.Write("SETOK");

            //        }
            //        else
            //        {
            //            Console.WriteLine("Received unknown OptiCom command");
            //        }
            //    }
            //    else
            //    {
            //        Console.WriteLine(executeData);
            //    }
            //}
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
