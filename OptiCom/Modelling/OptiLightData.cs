using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OptiCom.Modelling
{
    public class OptiLightData
    {
        public readonly double VLow;
        public readonly double VHigh;
        public readonly double EstimatedFETOffTime;

        public OptiLightData(double vLow, double vHigh, double estimatedFETOffTime)
        {
            VLow = vLow;
            VHigh = vHigh;
            EstimatedFETOffTime = estimatedFETOffTime;
        }

        public override string ToString()
        {
            //return OptiLightGenerator.MAGIC_NUMBER + ";" + VLow.ToString("0.0") + ";" + VHigh.ToString("0.0") + ";" + EstimatedFETOffTime.ToString("0.00000") + ";0;0";
            return VLow.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture) + "," + VHigh.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture) + "," + EstimatedFETOffTime.ToString("0.00000", System.Globalization.CultureInfo.InvariantCulture);
        }
    }
}
