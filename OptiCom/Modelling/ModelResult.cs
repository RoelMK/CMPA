using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OptiCom.Modelling
{
    public class ModelResult
    {
        public readonly double StartSpeed;
        public readonly double EstimatedFETOffTime;

        public ModelResult(double startSpeed, double estimatedFETOffTime)
        {
            StartSpeed = startSpeed;
            EstimatedFETOffTime = estimatedFETOffTime;
        }
    }
}
