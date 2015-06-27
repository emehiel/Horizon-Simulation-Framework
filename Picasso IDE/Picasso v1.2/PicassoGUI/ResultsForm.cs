using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace PicassoGUI
{
    public partial class ResultsForm : Form
    {
        public ResultsForm()
        {
            InitializeComponent();
        }
    }

    public partial class DXViewer : Control
    {
        public Device device = null;
        PresentParameters presentParams = new PresentParameters();
        public bool initFailure = true;
        private bool deviceLost = false;
        private bool rendering = false;

        public DXViewer()
        {
            InitDevice();
            this.BackColor = Color.Black;
        }

        public void InitDevice()
        {
            AdapterInformation adapterInfo = Manager.Adapters[0];
            presentParams.BackBufferCount = 1;
            presentParams.BackBufferFormat = adapterInfo.CurrentDisplayMode.Format;
            presentParams.BackBufferWidth = this.Width;
            presentParams.BackBufferHeight = this.Height;
            presentParams.SwapEffect = SwapEffect.Discard;
            presentParams.AutoDepthStencilFormat = DepthFormat.D24S8;
            presentParams.EnableAutoDepthStencil = true;
            presentParams.Windowed = true;

            device = new Device(0, DeviceType.Hardware, this.Handle, CreateFlags.SoftwareVertexProcessing, presentParams);
            device.DeviceResizing += new System.ComponentModel.CancelEventHandler(OnResize);
            initFailure = false;
        }

        public void Render()
        {
            if (device == null) return;
            if (rendering) return;
            if (deviceLost) AttemptRecovery();
            if (deviceLost) return;

            rendering = true;
            device.BeginScene();

            // Render stuff here

            device.EndScene();

            try
            {
                device.Present();
            }
            catch (DeviceLostException)
            {
                deviceLost = true;
            }
            finally
            {
                rendering = false;
            }
        }

        protected void AttemptRecovery()
        {
            if (device == null) return;
            try
            {
                device.TestCooperativeLevel();
            }
            catch (DeviceLostException)
            {
            }
            catch (DeviceNotResetException)
            {
                try
                {
                    device.Reset(presentParams);
                    deviceLost = false;
                }
                catch (DeviceLostException)
                {
                }
            }
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
            if (device == null) return;
            this.Render();
        }

        private void OnResize(Object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = true;
        }
    }
}
