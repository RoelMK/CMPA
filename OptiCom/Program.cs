using System;
using System.Collections.Generic;
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
        private const int ArduinoBitRate = 115200;      // Bit rate

        private static SerialPort ArduinoPort = new SerialPort(ArduinoPortName, ArduinoBitRate, Parity.None);       // Arduino serial port properties
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
                ArduinoPort.Open();
                ArduinoPort.DataReceived += ArduinoPort_DataReceived;
            }
            catch (Exception)
            {
                bool retry = PrintError("[INFO] Failed to open serial port!", true);
                if(retry)
                {
                    Console.Clear();
                    Main(args);
                }
                return;
            }

            // Wait
            do
            {
                while (!Console.KeyAvailable)
                {
                    Thread.Sleep(1);
                }
            } while (Console.ReadKey(true).Key != ConsoleKey.Escape);

            // Exit
            ArduinoPort.Close();
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
                Console.Clear();
                Main(args);
            }
            return;
        }

        private static bool receivingSerialData;        // Received serial data
        private static string serialDataReceived;       // Is receiving serial data?

        /// <summary>
        /// Receive serial data
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void ArduinoPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            // Receive serial data
            bool executeSerialInput = false;
            string serialData = ArduinoPort.ReadExisting();     // Read serial buffer
            serialData = serialData.Replace(" ", "");           // Remove all spaces
            serialData = new string(serialData.Where(c => !char.IsControl(c)).ToArray());       // Remove all control chars

            // Check data
            if (serialData.StartsWith("$") && serialData.EndsWith("#"))
            {
                serialDataReceived = serialData;
                receivingSerialData = false;
                executeSerialInput = true;
            }
            else if (serialData.StartsWith("$"))
            {
                receivingSerialData = true;
                serialDataReceived = serialData;
            }
            else if(receivingSerialData)
            {
                serialDataReceived += serialData;
                if (serialData.EndsWith("#"))
                {
                    receivingSerialData = false;
                    executeSerialInput = true;
                }
            }

            // Execute serial input commands or print serial input
            if(executeSerialInput)
            {
                string executeData = serialDataReceived.Replace("$", "");
                executeData = executeData.Replace("#", "");

                if (executeData.Contains("*"))
                {
                    if (executeData.Contains("G%"))
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
                            ArduinoPort.Write("NODATA");
                        }
                    }
                    else if (executeData.Contains("S%"))
                    {
                        string dataToWrite = executeData.Replace("*S%", "");
                        WriteOptiLightData(dataToWrite);

                    }
                    else
                    {
                        Console.WriteLine("Received unknown OptiCom command");
                    }
                }
                else
                {
                    Console.WriteLine(executeData);
                }
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
