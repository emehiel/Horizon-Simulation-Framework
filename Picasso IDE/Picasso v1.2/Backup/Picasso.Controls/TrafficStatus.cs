using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Controls
{
    public partial class TrafficStatus : UserControl
    {
        public TrafficStatus()
        {
            InitializeComponent();
        }

        public void Go()
        {
            // Turn green light on, others off
            goButton.On();
            yieldButton.Off();
            stopButton.Off();
        }

        public void Yield()
        {
            // Turn yellow light on, others off
            goButton.Off();
            yieldButton.On();
            stopButton.Off();
        }

        public void Stop()
        {
            // Turn red light on, others off
            goButton.Off();
            yieldButton.Off();
            stopButton.On();
        }
    }
}
