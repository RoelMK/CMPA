using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OptiCom.Modelling
{
    public class TimeCurrent
    {
        private List<double> timeList;
        private List<double> currentList;

        public TimeCurrent()
        {
            timeList = new List<double>();
            currentList = new List<double>();
        }


        public double GetCurrent(double time)
        {
            double minDistance = 0; 
            int minIndex = -1;

            for (int i = 0; i < timeList.Count; i++)
            {
                var distance = Math.Abs(time - timeList[i]);
                if (minIndex == -1 || distance < minDistance)
                {
                    minDistance = distance;
                    minIndex = i;

                    if (minDistance == 0)
                        break;
                }
            }

            if(minIndex == -1)
            {
                return 0;
            }
            else
            {
                try
                {
                    return currentList[minIndex];
                }
                catch (Exception)
                {
                    return 0;
                }
            }
        }


        public void Add(double time, double current)
        {
            timeList.Add(time);
            currentList.Add(current);
        }
    }
}
