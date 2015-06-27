using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.XPath;
using PicassoGUI.Util;
using PicassoGUI.Controls;
using PicassoGUI.Forms;
using PicassoGUI.Components;
using System.Runtime.Serialization.Formatters.Binary;
using System.Diagnostics;
using MementoModel;
using Hsf.Utility;

namespace PicassoGUI
{
    public struct SettingsModel
    {
        public string Runner;
        public string WorkingDirectory;
        public string Arguments;
        public bool Automatic;
    };

    public partial class MainForm : Form, IMForm
    {
        #region Data Members
        public BackgroundWorker backgroundWorker1;
        //public List<DesignItem> DesignItems { get; set; }
        //public List<Connector> Connectors { get; set; }

        public DesignItem SelectedDesignItem { get; set; }
        public Connector SelectedConnector { get; set; }

        public List<PicassoGUI.Util.Asset> Assets { get; set; }

        public ScenarioDocument ScenarioDocument { get; set; }
        public DirectoryInfo ProjectDirectory { get; set; }
        public String ProjectName { get; set; }        
        public String scenarioName
        {
            get { return this.ScenarioDocument.ScenarioParameters.ScenarioName; }
            set { this.ScenarioDocument.ScenarioParameters.ScenarioName = value; }
        }
        public DateTime simStartTime { get; set; }
        public DateTime simEndTime { get; set; }
        public decimal simTimeStep;
        public decimal minSchedules;
        public decimal maxSchedules;

        public int SubsystemID { get; set; }
        public int DepCollectorID { get; set; }

        public String ScenarioFileExtension { get; set; }

        private Point previousSubsystemLocation;

        // TODO:  Should this be in the document?
        private bool hasChanged = false;
        public bool HasChanged
        {
            get { return this.hasChanged;}
            set
            {
                this.hasChanged = value;
                if (this.hasChanged)
                {
                    if (!this.Text.EndsWith("*"))
                        this.Text += "*";
                }
                else
                {
                    if (this.Text.EndsWith("*"))
                        this.Text = this.Text.Remove(this.Text.Length - 1, 1);
                }
            }
        }

        //private DrawingCanvas canvas;
        //private TargetCanvas targetCanvas;

        // Tree Items
        private TreeNode systemModelNode;
        private TreeNode selectedAssetNode = new TreeNode();
        List<ScenarioComponent> ApplicationScenarios;

        // Permenant Xml Window (sometimes hidden)
        private XmlBrowser _xmlWindow;

        // Temporary file for serialization storage
        private string _dumpFileName;

        // Picasso only has one settings dialog
        public PicassoSettings settingsForm;
        private SettingsModel _settings;

        // Only one scenario's windows are visible at a time; must keep track of currently-active scenario
        public ScenarioComponent currentScenarioFocus;

        // Need to keep track of scenario being executed by runner
        public ScenarioComponent hScenarioInRunner;

        // Specify default window title
        private string _textPrefix ;

        // Import utility
        private ImportUtility _importUtility;

        #endregion

        #region Constructors

        //private COEs myCOEs;
        public MainForm()
        {
            InitializeComponent();

            // ??
            SubsystemID = 0;
            DepCollectorID = 0;
            previousSubsystemLocation = new Point(20, 20);
            SelectedDesignItem = null;
            SelectedConnector = null;
            drawingCanvas.DesignItems = new List<DesignItem>();
            drawingCanvas.Connectors = new List<Connector>();
            ScenarioDocument = new ScenarioDocument();
            Assets = new List<PicassoGUI.Util.Asset>();

            // Set file extension to be used for loading HSF scenarios
            ScenarioFileExtension = ".xml";

            // List view hidden?
            this.targetListView.Visible = false;

            // Initialize window title
            _textPrefix = "Horizon / Picasso";
            Text = _textPrefix;

            // Create sample scenario and add to list; match window name to focus
            ApplicationScenarios = new List<ScenarioComponent>();
            ScenarioComponent sampleScenario = new ScenarioComponent();
            ApplicationScenarios.Add(sampleScenario);
            Text = _textPrefix + " - " + sampleScenario.ScenarioName;

            // Tree view should be expanded at first; show only empty scenario
            TreeNode blankNode = new TreeNode();
            ApplicationScenarios[0].PopulateTree(picassoTreeView, blankNode);
            picassoTreeView.Nodes.RemoveAt(0);
            picassoTreeView.ExpandAll();
            currentScenarioFocus = ApplicationScenarios[0];

            // Create xml window
            _xmlWindow = new XmlBrowser("http://tythos.net/horizon/xmlBrowser.html", false);
            _xmlWindow.MdiParent = this;

            // Set temporary serialization storage
            _dumpFileName = "C:\\.horizonDump.xml";

            // Initialize background worker
            backgroundWorker1 = new BackgroundWorker();

            // Initialize settings dialog, but do not display
            settingsForm = new PicassoSettings();

            // Runner is initially inactive
            hScenarioInRunner = null;

            // Initialize import utility
            _importUtility = new ImportUtility();

            // Initialize Memento menus and blank manager
            _blankManager = new MementoManager(this);
            RefreshMementoMenus();
        }

        #endregion

        #region Methods

        private void AddNewSubsystem()
        {
            String name = "Subsystem " + this.SubsystemID.ToString();
            PicassoGUI.Controls.Subsystem newSubsystem = new PicassoGUI.Controls.Subsystem((PicassoGUI.Util.Asset)this.picassoTreeView.SelectedNode.Tag, name, this.previousSubsystemLocation);

            this.AddSubsystem(newSubsystem);

            this.SubsystemID++;

            //if (this.previousSubsystemLocation.X < this.systemTabPage.Size.Width && this.previousSubsystemLocation.Y < this.systemTabPage.Size.Height)
            if (true)
                this.previousSubsystemLocation = Point.Add(this.previousSubsystemLocation, new Size(20, 20));
            /* else
                this.previousSubsystemLocation = new Point(20, 20); */
        }

        private void AddSubsystem(PicassoGUI.Controls.Subsystem newSubsystem)
        {
            TreeNode tempNode = new TreeNode(newSubsystem.Name);
            tempNode.Tag = newSubsystem;
            tempNode.Text = newSubsystem.Name;
            this.picassoTreeView.SelectedNode.Nodes.Add(tempNode);

            //this.DesignItems.Add(temp);
            this.drawingCanvas.AddObject(newSubsystem);
            this.SelectedDesignItem = newSubsystem;

            this.mainFormPropertyGrid.SelectedObject = newSubsystem.Properties;
            //this.tabControl.SelectedTab = this.systemTabPage;

            this.HasChanged = true;
            //this.systemTabPage.Refresh();
        }

        private bool BuildSimulation(String path, String fileName)
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;

            XmlWriter writer = XmlWriter.Create(path + fileName, settings);

            writer.WriteStartElement("SCENARIO");
            writer.WriteAttributeString("scenarioName", this.scenarioName);

            DateTime start = DateTime.Now; //this.simStartTime.Value;
            Double startUT = Utility.HMS2UT(start.Hour, start.Minute, start.Second);
            Double startJD = Utility.YMDUT2JD(start.Year, start.Month, start.Day, startUT);
            DateTime end = DateTime.Now; // this.simEndTime.Value;
            double dt = (end - start).TotalSeconds;

            writer.WriteStartElement("SIMULATION_PARAMETERS");
            writer.WriteAttributeString("SimStartJD", startJD.ToString());
            writer.WriteAttributeString("simStart", "0.0");
            writer.WriteAttributeString("SimEnd", dt.ToString());
            writer.WriteEndElement();

            writer.WriteStartElement("SCHEDULER_PARAMETERS");
            writer.WriteAttributeString("MaxSchedules", "10"); //this.maxSchedules.Value.ToString());
            writer.WriteAttributeString("CropTo", "10"); //this.minSchedules.Value.ToString());
            writer.WriteAttributeString("SimStep", "0.10"); //this.simTimeStep.Value.ToString());

            writer.WriteEndElement();

            writer.Close();

            return true;
        }

        private bool BuildModel(String path, String fileName)
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;

            XmlWriter writer = XmlWriter.Create(path + fileName, settings);

            writer.WriteStartElement("MODEL");

            bool scripted = false;
            bool depWriten = false;
            foreach (DesignItem d in this.drawingCanvas.DesignItems)
                if (d.GetType() == typeof(PicassoGUI.Controls.Subsystem) && ((SubsystemProperties)d.Properties).Scripted)
                {
                    writer.WriteStartElement("LUA");
                    writer.WriteAttributeString("enableScripting", "True");
                    writer.WriteStartElement("LUA_FILE");
                    writer.WriteAttributeString("src", "..\\Subsystems\\" + ((PicassoGUI.Controls.Subsystem)d).ScriptFileName);
                    writer.WriteEndElement(); // LUA_FILE
                    scripted = true;
                }
            foreach (Connector c in this.drawingCanvas.Connectors)
                if (c.Dependency.Scripted  && !depWriten)
                {
                    if (!scripted)
                    {
                        writer.WriteStartElement("LUA");
                        writer.WriteAttributeString("enableScripting", "True");
                    }

                    writer.WriteStartElement("LUA_FILE");
                    if (c.Dependency.CustomLogicFile == null)
                        writer.WriteAttributeString("src", "..\\Dependencies\\" + fileName.Replace("_model.xml", "") + "_dependencies.txt");
                    else
                        writer.WriteAttributeString("src", c.Dependency.CustomLogicFile);
                    writer.WriteEndElement(); // LUA_FILE
                    scripted = true;
                    depWriten = true;
                }

            if (scripted)
                writer.WriteEndElement(); //LUA

            writer.WriteStartElement("ENVIRONMENT");
            writer.WriteStartElement("SUN");
            writer.WriteAttributeString("isSunVecConstant", "True");
            writer.WriteEndElement(); //SUN
            writer.WriteEndElement(); //ENVIRONMENT

            writer.WriteStartElement("SCHEDULE_EVALUATOR");
            writer.WriteAttributeString("type", "scripted");
            writer.WriteAttributeString("evaluateLUAFcn", "ScheduleEvaluator");
            writer.WriteEndElement(); //SCHEDULE_EVALUATOR

            foreach (PicassoGUI.Util.Asset asset in this.Assets)
            {
                writer.WriteStartElement("ASSET");
                writer.WriteStartElement("POSITION");
                // TODO:  Add Environment COntrols and object
                writer.WriteAttributeString("PositionType", "PredeterminedECI");
                String temp = asset.Position.COEs.RString + ", " + asset.Position.COEs.VString;
                writer.WriteAttributeString("ICs", "Matrix(6,1,{" + temp + "})");
                writer.WriteStartElement("EOMS");
                writer.WriteAttributeString("EOMSType", "orbital_EOMS");
                writer.WriteEndElement(); //EOMS
                writer.WriteEndElement(); //POSITION

                foreach (DesignItem designItem in this.drawingCanvas.DesignItems)
                {
                    if (designItem.GetType() == typeof(PicassoGUI.Controls.Subsystem))
                    {
                        SubsystemProperties subprops = (SubsystemProperties)designItem.Properties;
                        if (subprops.Asset.Name == asset.Name)
                        {
                            writer.WriteStartElement("SUBSYSTEM");
                            if (subprops.Scripted)
                            {
                                writer.WriteAttributeString("Type", "Scripted");
                                writer.WriteAttributeString("initLUAFcn", ((PicassoGUI.Controls.Subsystem)designItem).InitFunction.Name);
                                writer.WriteAttributeString("canPerformLUAFcn", ((PicassoGUI.Controls.Subsystem)designItem).CanPerformFunction.Name);
                                writer.WriteAttributeString("canExtendLUAFcn", ((PicassoGUI.Controls.Subsystem)designItem).CanExtendFucntion.Name);
                            }
                            else
                            {
                                writer.WriteAttributeString("Type", "Static");
                            }
                            writer.WriteAttributeString("Name", subprops.Name);
                            writer.WriteAttributeString("SubId", subprops.IntID.ToString());
                            foreach (StateVariable sv in subprops.States)
                            {
                                writer.WriteStartElement("IC");
                                writer.WriteAttributeString("type", sv.Type.ToString());
                                writer.WriteAttributeString("key", sv.Name);
                                writer.WriteAttributeString("value", sv.InitialValue);
                                writer.WriteEndElement();//IC
                            }
                            foreach (Connector connector in this.drawingCanvas.Connectors)
                            {
                                    if (subprops.ID == connector.InputSubsystemID)//.OutputSubsystemID)
                                    {
                                        //if (connector.InputSubsystem.GetType() == typeof(Subsystem))
                                        if (connector.OutputSubsystem.GetType() == typeof(PicassoGUI.Controls.Subsystem))
                                        {
                                            writer.WriteStartElement("DEPENDENCY");
                                            writer.WriteAttributeString("SubId", connector.OutputSubsystemID.GetHashCode().ToString());// connector.InputSubsystemID.GetHashCode().ToString());
                                            writer.WriteEndElement();//DEPENDENCY
                                        }
                                        else if (connector.OutputSubsystem.GetType() == typeof(DependencyCollector))
                                        {
                                            foreach (Connector c in this.drawingCanvas.Connectors)
                                                foreach (Port port in connector.OutputSubsystem.Ports)
                                                    if (port.PortID == c.InputPort.PortID)
                                                    {
                                                        writer.WriteStartElement("DEPENDENCY");
                                                        writer.WriteAttributeString("SubId", c.OutputSubsystemID.GetHashCode().ToString());
                                                        writer.WriteEndElement();//DEPENDENCY
                                                    }
                                        }
                                        if (connector.Dependency.HasFunction)
                                        {
                                            writer.WriteStartElement("DEPENDENCY_FCN");
                                            if (connector.Dependency.Scripted)
                                                writer.WriteAttributeString("Scripted", "true");
                                            else
                                                writer.WriteAttributeString("Scripted", "false");

                                            writer.WriteAttributeString("Type", connector.Dependency.OutputType.ToString());
                                            String callKey = (connector.Dependency.InputSubsystemName + "_" + connector.Dependency.InterperterName).Replace(" ", "_");
                                            String Key = (connector.Dependency.InputAssetName + "_" + callKey).Replace(" ", "_");
                                            writer.WriteAttributeString("Key", Key);
                                            writer.WriteAttributeString("CallKey", callKey);
                                            writer.WriteEndElement();//DEPENDENCY_FCN
                                        }
                                    }
                                        /*
                                        else if (connector.OutputSubsystem.GetType() == typeof(DependencyCollector))
                                            foreach (Connector c in this.drawingCanvas.Connectors)
                                                foreach (Port port in connector.OutputSubsystem.Ports)
                                                    if (port.PortID == c.InputPort.PortID)
                                                    {
                                                        writer.WriteStartElement("DEPENDENCY");
                                                        writer.WriteAttributeString("SubId", c.OutputSubsystemID.GetHashCode().ToString());
                                                        writer.WriteEndElement();//DEPENDENCY
                                                        if (c.Dependency.HasFunction)
                                                        {
                                                            writer.WriteStartElement("DEPENDENCY_FCN");
                                                            if (connector.Dependency.Scripted)
                                                                writer.WriteAttributeString("Scripted", "true");
                                                            else
                                                                writer.WriteAttributeString("Scripted", "false");

                                                            writer.WriteAttributeString("Type", connector.Dependency.OutputType.ToString());
                                                            String callKey = (connector.Dependency.InputSubsystemName + "_" + connector.Dependency.InterperterName).Replace(" ", "_");
                                                            String Key = (connector.Dependency.InputAssetName + "_" + callKey).Replace(" ", "_");
                                                            writer.WriteAttributeString("Key", Key);
                                                            writer.WriteAttributeString("CallKey", callKey);
                                                            writer.WriteEndElement();//DEPENDENCY_FCN
                                                        }
                                                    }
                                    }
                            */
                            //   }
                            }
                            writer.WriteEndElement(); //SUBSYSTEM
                        }
                        foreach (StateVariable sv in subprops.States)
                            foreach (PicassoGUI.Util.Constraint c in sv.Constraints)
                            {
                                writer.WriteStartElement("CONSTRAINT");
                                writer.WriteAttributeString("Value", c.Value.ToString());
                                writer.WriteAttributeString("SubId", sv.SubsystemID.GetHashCode().ToString());
                                writer.WriteAttributeString("Type", c.Type.ToString());
                                writer.WriteEndElement(); //CONSTRAINT
                            }
                    }
                }
                writer.WriteEndElement(); //ASSET
            }
            writer.WriteEndElement(); //MODEL

            writer.Close();

            return true;
        }

        private bool BuildDependencyFile(String path, String fileName)
        {
            StreamWriter writer = File.CreateText(path + fileName);

            String buffer;

            foreach (Connector c1 in this.drawingCanvas.Connectors)
            {
                if (c1.InputSubsystem.GetType() == typeof(PicassoGUI.Controls.Subsystem) && c1.OutputSubsystem.GetType() == typeof(PicassoGUI.Controls.Subsystem))
                {
                    if (c1.Dependency.Scripted)
                    {
                        buffer = c1.Dependency.InputAssetName + "_" + c1.Dependency.InputSubsystemName + "SUB_" + c1.Dependency.InterperterName + "()";
                        buffer = buffer.Replace(" ", "_");
                        buffer = "function " + buffer;
                        writer.WriteLine(buffer);
                        if (c1.Dependency.CustomDependency != null)
                            foreach (String s in c1.Dependency.CustomDependency.Function)
                                writer.WriteLine("\t" + s);
                        writer.Write(writer.NewLine);
                        writer.WriteLine("end");
                        writer.WriteLine(writer.NewLine);
                    }
                }
                    // Write out the dependency collector functions
                else if (c1.InputSubsystem.GetType() == typeof(PicassoGUI.Controls.Subsystem) && c1.OutputSubsystem.GetType() == typeof(DependencyCollector))
                //foreach (Dependency d2 in c1.Dependencies)
                {
                    bool isScripted = false;
                    // TODO:  Do this with a Find Predicate Function
                    //this.canvas.Connectors.Find(
                    foreach (Connector c2 in this.drawingCanvas.Connectors)
                    {

                        if (c2.InputSubsystemID == c1.OutputSubsystemID)
                        {
                            if (c2.Dependency.Scripted)
                            {
                                isScripted = true;
                                break;
                            }
                        }
                    }
                    if (isScripted)
                    {
                        buffer = c1.Dependency.InputAssetName + "_" + c1.Dependency.InputSubsystemName + "SUB_" + c1.Dependency.InterperterName + "()";
                        buffer = buffer.Replace(" ", "_");
                        buffer = "function " + buffer;
                        writer.WriteLine(buffer);
                        int i = 0;
                        foreach (Connector c3 in this.drawingCanvas.Connectors)
                            if (c3.InputSubsystemID == c1.OutputSubsystemID)
                            {
                                if (c3.Dependency.Scripted)
                                {
                                    buffer = c1.Dependency.InputSubsystemName + "SUB_" + c1.Dependency.InputType.ToString() + "Profile_" + c3.OutputSubsystem.Name + "SUB()";
                                    buffer = buffer.Replace(" ", "_");
                                    buffer = "prof" + i.ToString() + " = " + buffer;
                                    writer.WriteLine("\t" + buffer);

                                }
                                i++;
                            }

                        buffer = "return ";
                        i = 0;
                        foreach (Connector c3 in this.drawingCanvas.Connectors)
                        {
                            if (c3.InputSubsystemID == c1.OutputSubsystemID)
                            {

                                if (c3.Dependency.Scripted)
                                {
                                    buffer += ((DependencyCollector)c1.OutputSubsystem).DependencyCombination.Substring(i, 1) + "prof" + i.ToString();
                                    i++;
                                }
                            }
                        }
                        writer.WriteLine("\t" + buffer);
                        writer.WriteLine("end");
                        writer.WriteLine(writer.NewLine);
                        isScripted = false;
                    }
                }
            }
            foreach (Connector c1 in this.drawingCanvas.Connectors)
                if (c1.InputSubsystem.GetType() == typeof(PicassoGUI.Controls.Subsystem) && c1.OutputSubsystem.GetType() == typeof(DependencyCollector))
                //foreach (Dependency d2 in c1.Dependencies)
                {
                    foreach (Connector c2 in this.drawingCanvas.Connectors)
                    if (c2.InputSubsystemID == c1.OutputSubsystemID)
                    {
                        if (c2.Dependency.Scripted)
                        {
                            buffer = c1.Dependency.InputSubsystemName + "SUB_" + c1.Dependency.InputType.ToString() + "Profile_" + c2.OutputSubsystem.Name + "SUB()";
                            buffer = buffer.Replace(" ", "_");
                            buffer = "function " + buffer;
                            writer.WriteLine(buffer);
                            if (c1.Dependency.CustomDependency.Function != null)
                                foreach (String s in c1.Dependency.CustomDependency.Function)
                                    writer.WriteLine("\t" + s);
                            writer.WriteLine("end");
                            writer.WriteLine(writer.NewLine);
                        }
                    }
                }
            
            writer.Close();

            return true;
        }

        private bool BuildSubsystems(String path, String fileName)
        {
            String buffer = null;
            foreach (DesignItem s in this.drawingCanvas.DesignItems)
            {
                if (s.GetType() == typeof(PicassoGUI.Controls.Subsystem))
                {
                    if (((SubsystemProperties)s.Properties).Scripted)
                    {
                        StreamWriter writer = File.CreateText(path + "..\\Subsystems\\" + ((PicassoGUI.Controls.Subsystem)s).ScriptFileName);
                        writer.WriteLine("function " + ((PicassoGUI.Controls.Subsystem)s).InitFunction.Name + "(sub, subParams)");
                        foreach (StateVariable sv in ((SubsystemProperties)s.Properties).States)
                        {
                            if (sv.OutputFlag)
                            {
                                buffer = sv.ToStringShort().ToLower();
                                buffer = buffer + "StateVarKey('";
                                writer.WriteLine("\tsub:addKey(" + buffer + sv.Name + "'))");
                            }
                        }
                        foreach (SubsystemParameter sp in ((PicassoGUI.Controls.Subsystem)s).Parameters)
                            writer.WriteLine("\tsubParams:add" + sp.Type.ToString() + "Parameter('" + sp.Name.Replace(" ", "_") + "', " + sp.Value + ")");
                        if (((PicassoGUI.Controls.Subsystem)s).InitFunction.Function != null)
                            foreach (String logicLine in ((PicassoGUI.Controls.Subsystem)s).InitFunction.Function)
                                writer.WriteLine(logicLine);
 
                        writer.WriteLine("end");
                        writer.WriteLine(writer.NewLine);

                        writer.WriteLine("function " + ((PicassoGUI.Controls.Subsystem)s).CanPerformFunction.Name + "(oldState, newState, task, position, environment, deps, subParams)");
                        if (((PicassoGUI.Controls.Subsystem)s).CanPerformFunction.Function != null)
                            foreach (String logicLine in ((PicassoGUI.Controls.Subsystem)s).CanPerformFunction.Function)
                                writer.WriteLine("\t" + logicLine);
                        
                        writer.WriteLine("end");
                        writer.WriteLine(writer.NewLine);

                        writer.WriteLine("function " + s.Name + "_canExtend(newState, position, environment, evalToTime, deps, subParams)");
                        if (((PicassoGUI.Controls.Subsystem)s).CanExtendFucntion.Function != null)
                            foreach (String logicLine in ((PicassoGUI.Controls.Subsystem)s).CanExtendFucntion.Function)
                                writer.WriteLine("\t" + logicLine);

                        writer.WriteLine("end");
                        writer.WriteLine(writer.NewLine);

                        foreach (LUAScript func in ((PicassoGUI.Controls.Subsystem)s).OtherFunctions)
                        {
                            //writer.WriteLine("function " + funcName);
                            int i = 0;
                            foreach(String logicLine in func.Function)
                            {
                                if (i == 0)
                                    writer.WriteLine(logicLine);
                                else
                                    writer.WriteLine("\t" + logicLine);
                                i++;
                            }
                            writer.WriteLine("end");
                            writer.WriteLine(writer.NewLine);
                        }

                        writer.Close();
                    }
                }
            }

            return true;
        }

        /*
        private void BuildTargetDeckFile(String path, String fileName)
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;

            XmlWriter writer = XmlWriter.Create(path + fileName, settings);

            writer.WriteStartElement("TARGETDECK");
            /*
            foreach (Target t in this.targetCanvas.Targets)
            {
                writer.WriteStartElement("TARGET");
                writer.WriteAttributeString("TargetName", t.Name);
                writer.WriteAttributeString("TargetType", t.TargetType.ToString());
                writer.WriteAttributeString("TaskType", t.TaskType.ToString());
                writer.WriteAttributeString("MaxTimes", t.MaxTimes.ToString());
                writer.WriteAttributeString("Value", t.Value.ToString());
                writer.WriteStartElement("POSITION");
                writer.WriteAttributeString("PositionType", t.Position.Type.ToString());
                String buffer = "";
                foreach (double d in t.Position.InitialConditions)
                    buffer += d.ToString() + ", ";
                buffer = buffer.Remove(buffer.Length - 2, 2);

                writer.WriteAttributeString("ICs", buffer);
                writer.WriteEndElement(); // POSITION
                writer.WriteEndElement(); // TARGET

            }
             */
        /*
            writer.WriteEndElement(); // TargetDeck

            writer.Close();
     
        }
         */

        #region Open Scenario
        private void openDocument_Click(object sender, EventArgs e)
        {
            OpenFileDialog openDiag = new OpenFileDialog();
            openDiag.Filter = "Horizon Scenario (eXtensible) (*.hsx)|*.hsx|All files (*.*)|*.*";
            openDiag.Title = "Open Scenario";
            if (openDiag.ShowDialog() == DialogResult.OK)
            {
                openScenarioFromFile(openDiag.FileName);
            }
            else
            {
                // Cancelled?
            }
        }

        private void openScenarioFromFile(string filename)
        {
            // Loads new scenario using components specified in given HSX file
            XqlParser myParser = new XqlParser(filename);
            List<Hashtable> result;

            // Determine paths
            result = myParser.Query("SELECT path FROM hsx"); string rootPath = (string)result[0]["path"];
            result = myParser.Query("SELECT filename FROM scenario"); string scenPath = (string)rootPath + result[0]["filename"];
            result = myParser.Query("SELECT filename FROM targetdeck"); string trgtPath = (string)rootPath + result[0]["filename"];
            result = myParser.Query("SELECT filename FROM model"); string modlPath = (string)rootPath + result[0]["filename"];

            // Create new scenario from file
            ScenarioComponent newScenario = new ScenarioComponent();
            newScenario.Deserialize(scenPath);
            ApplicationScenarios.Add(newScenario);

            // Create new targetdeck from file
            TargetdeckComponent newTargetdeck = new TargetdeckComponent();
            newTargetdeck.Deserialize(trgtPath);
            newScenario.TargetDeck = newTargetdeck;

            // Create new model from file
            ModelComponent newModel = new ModelComponent();
            newModel.Deserialize(modlPath);
            newScenario.Model = newModel;

            // Refresh node tree to show new scenario; select
            TreeNode blankNode = new TreeNode();
            newScenario.PopulateTree(picassoTreeView, blankNode);
            setScenarioFocus(newScenario);
        }

        private void setScenarioFocus(ScenarioComponent scenario)
        {
            // Only one scenario's windows should be visible at a time. When a new scenario is focused, restore its windows and hide others
            currentScenarioFocus = scenario;
            foreach (Form f in MdiChildren)
            {
                try
                {
                    IPicassoComponent picComp = (IPicassoComponent)f.Tag;
                    ScenarioComponent s = scenarioOfComponent(picComp);
                    if (s == scenario)
                    {
                        f.Visible = true;
                    }
                    else
                    {
                        f.Visible = false;
                    }
                }
                catch (NullReferenceException)
                {
                    // Form f does not have a tag t
                    //MessageBox.Show("Form f does not have a tag t");
                }
                catch (InvalidCastException)
                {
                    // Form f's tag t isn't a Picasso component
                    MessageBox.Show("Form f's tag t isn't a Picasso component");
                }
            }
        }

        private void openScenarioDialog_FileOk(object sender, CancelEventArgs e)
        {
            String path = "";

            String[] name = this.openScenarioDialog.FileName.Split('\\');
            this.ProjectName = name.Last().Replace(".xml", "");

            int c = name.Count();
            if (name[c - 2] == this.ProjectName)
                path = this.openScenarioDialog.FileName.Replace(name.Last(), "");
            else
                path = this.openScenarioDialog.FileName.Replace(".xml", "\\");
            this.ProjectDirectory = new DirectoryInfo(path);

            this.openScenario();
        }

        private void openScenario()
        {
            // Clear out the current Scenario
            this.SelectedDesignItem = null;
            this.SelectedConnector = null;
            this.drawingCanvas.DesignItems.Clear();
            this.drawingCanvas.Connectors.Clear();
            this.ScenarioDocument.Connectors.Clear();
            this.ScenarioDocument.DesignItems.Clear();
            this.Assets.Clear();

            this.systemModelNode = this.picassoTreeView.Nodes[1];
            this.systemModelNode.Nodes.Clear();

            XmlSerializer designItemSerializer = new XmlSerializer(typeof(ScenarioDocument),
                            new Type[] {typeof(List<DesignItem>), typeof(List<Connector>), typeof(DesignItem),
                                    typeof(PicassoGUI.Controls.Subsystem), typeof(DependencyCollector), typeof(Connector), typeof(Corner),
                                    typeof(DesignItemProperties), typeof(SubsystemProperties), typeof(AssetPosition.COEs),
                                    typeof(DependencyCollectorProperties), typeof(LUAScript)});
            String filename = this.openScenarioDialog.FileName;
            TextReader reader = new StreamReader(filename);

            this.ScenarioDocument = (ScenarioDocument)designItemSerializer.Deserialize(reader);

            reader.Close();

            this.drawingCanvas.Connectors = this.ScenarioDocument.Connectors;
            this.drawingCanvas.DesignItems = this.ScenarioDocument.DesignItems;

            this.scenarioName = this.ScenarioDocument.ScenarioParameters.ScenarioName;
            this.simStartTime = this.ScenarioDocument.ScenarioParameters.simStartTime;
            this.simEndTime = this.ScenarioDocument.ScenarioParameters.simEndTime;
            this.simTimeStep = this.ScenarioDocument.ScenarioParameters.simTimeStep;
            this.minSchedules = this.ScenarioDocument.ScenarioParameters.minSchedules;
            this.maxSchedules = this.ScenarioDocument.ScenarioParameters.maxSchedules;

            this.drawingCanvas.Invalidate();
            this.addSubToolStripButton.Enabled = true;
            this.addDepToolStripButton.Enabled = true;

            foreach (DesignItem designItem in this.drawingCanvas.DesignItems)
            {
                foreach (Connector connector in this.drawingCanvas.Connectors)
                {
                    if (connector.OutputSubsystemID == designItem.ID)
                    {
                        connector.OutputSubsystem = designItem;

                        foreach (Port p in designItem.Ports)
                            if (connector.OutputPortID == p.PortID)
                            {
                                connector.OutputPort = p;
                                break;
                            }
                    }
                    if (connector.InputSubsystemID == designItem.ID)
                    {
                        connector.InputSubsystem = designItem;
                        foreach (Port p in designItem.Ports)
                            if (connector.InputPortID == p.PortID)
                            {
                                connector.InputPort = p;
                                break;
                            }
                    }
                }

                if (designItem.GetType() == typeof(PicassoGUI.Controls.Subsystem))
                {
                    if (!this.Assets.Contains(((SubsystemProperties)designItem.Properties).Asset))
                    {
                        this.Assets.Add(((SubsystemProperties)designItem.Properties).Asset);
                        TreeNode newAssetNode = new TreeNode(((SubsystemProperties)designItem.Properties).Asset.Name);
                        newAssetNode.Name = ((SubsystemProperties)designItem.Properties).Asset.Name;
                        newAssetNode.Tag = ((SubsystemProperties)designItem.Properties).Asset;
                        ((SubsystemProperties)designItem.Properties).Asset.Position.RefreshBoxes();
                        this.systemModelNode.Nodes.Add(newAssetNode);
                        //this.systemModelNode.Nodes.Add(((SubsystemProperties)designItem.Properties).Asset.Name);
                    }
                }
            }

            foreach (DesignItem d in this.drawingCanvas.DesignItems)
                if (d.GetType() == typeof(PicassoGUI.Controls.Subsystem))
                {

                    foreach (TreeNode t in this.systemModelNode.Nodes)
                        if (((SubsystemProperties)d.Properties).Asset.Name == t.Text)
                            t.Nodes.Add(d.Name);
                }
            this.systemModelNode.Expand();
        }

        #endregion

        #region Save Scenario
        private void saveDocument_Click(object sender, EventArgs e)
        {
            if (this.ProjectDirectory != null)
                this.saveScenarioDialog.InitialDirectory = this.ProjectDirectory.FullName;

            if (this.ProjectName == null)
                this.saveScenarioDialog.ShowDialog();
            else
            {
                if (this.HasChanged)
                    this.saveScenario();
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.saveScenarioDialog.ShowDialog();
        }

        private void saveScenarioDialog_FileOk(object sender, CancelEventArgs e)
        {

            String[] name = this.saveScenarioDialog.FileName.Split('\\');
            this.ProjectName = name.Last().Replace(".xml", "");
            this.Text = this.ProjectName;
            String path = this.saveScenarioDialog.FileName.Replace(".xml", "\\");
            this.ProjectDirectory = new DirectoryInfo(path);

            this.saveScenario();
        }

        private void saveScenario()
        {
            this.ScenarioDocument.Connectors = this.drawingCanvas.Connectors;
            this.ScenarioDocument.DesignItems = this.drawingCanvas.DesignItems;
            this.ScenarioDocument.ScenarioParameters = new ScenarioParameters(this.scenarioName, this.simStartTime, this.simEndTime,
                this.simTimeStep, this.maxSchedules, this.minSchedules);

            XmlSerializer designItemSerializer = new XmlSerializer(typeof(ScenarioDocument),
                new Type[] {typeof(List<DesignItem>), typeof(List<Connector>), typeof(DesignItem),
                typeof(PicassoGUI.Controls.Subsystem), typeof(DependencyCollector), typeof(Connector), typeof(Corner),
                typeof(DesignItemProperties), typeof(SubsystemProperties), typeof(AssetPosition.COEs),
                typeof(DependencyCollectorProperties), typeof(LUAScript)});

            if (!Directory.Exists(this.ProjectDirectory.FullName))
                Directory.CreateDirectory(this.ProjectDirectory.FullName);

            TextWriter writer = new StreamWriter(this.ProjectDirectory.FullName + this.ProjectName + this.ScenarioFileExtension);
            designItemSerializer.Serialize(writer, this.ScenarioDocument);

            writer.Close();
            this.HasChanged = false;
            this.saveToolStripButton.Enabled = false;
        }
        #endregion

        private bool IsSerializable(object obj)
        {
          System.IO.MemoryStream mem = new System.IO.MemoryStream();
          BinaryFormatter bin = new BinaryFormatter();
          try
          {
            bin.Serialize(mem, obj);
            return true;
          }
          catch(Exception ex)
          {
            MessageBox.Show("Your object cannot be serialized." + 
                             " The reason is: " + ex.ToString());
            return false;
          }
        }

        private void RunAndGetOutput(String pathName)
        {
            //StreamReader sr;
            //Process p = new Process();
            //ProcessStartInfo psi = new ProcessStartInfo(pathName);
            //psi.UseShellExecute = false;
            //psi.RedirectStandardOutput = true;
            //psi.CreateNoWindow = true;
            //this.process1.StartInfo.FileName = pathName;
            //this.process1.StartInfo.Arguments = "\"C:\\Documents and Settings\\emehiel\\Desktop\\CPStar\\Horizon\\Code Base\\v2.1\\Runs\\io\\DSAC.xml\" \"C:\\Documents and Settings\\emehiel\\Desktop\\CPStar\\Horizon\\Code Base\\v2.1\\Runs\\io\\300targets.xml\" \"C:\\Documents and Settings\\emehiel\\Desktop\"";//\\CPStar\\Horizon\\Code Base\\v2.1\\Runs\\io\"";
            //this.process1.StartInfo.Arguments = @"io/DSAC.xml io/300targets.xml io";
            //p.StartInfo = psi;

            //this.process1.Start();
            
            /*
            ProcessCaller processCaller = new ProcessCaller(this);
            //processCaller.FileName = @"..\..\hello.bat";
            processCaller.FileName = pathName;
            processCaller.Arguments = "\"C:\\Documents and Settings\\emehiel\\Desktop\\CPStar\\Horizon\\Code Base\\v2.1\\Runs\\io\\DSAC.xml\" \"C:\\Documents and Settings\\emehiel\\Desktop\\CPStar\\Horizon\\Code Base\\v2.1\\Runs\\io\\300targets.xml\" \"C:\\Documents and Settings\\emehiel\\Desktop\\CPStar\\Horizon\\Code Base\\v2.1\\Runs\\io\"";
            //processCaller.StdErrReceived += new DataReceivedHandler(writeStreamInfo);
            //processCaller.StdOutReceived += new DataReceivedHandler(writeStreamInfo);
            //processCaller.Completed += new EventHandler(processCompletedOrCanceled);
            //processCaller.Cancelled += new EventHandler(processCompletedOrCanceled);
            // processCaller.Failed += no event handler for this one, yet.

            this.textBox1.Text = "Started function.  Please stand by.." + Environment.NewLine;

            // the following function starts a process and returns immediately,
            // thus allowing the form to stay responsive.
            processCaller.Start();

            processCaller.WaitUntilDone();
             */
        }
        
        private void writeStreamInfo(object sender, PicassoGUI.Util.DataReceivedEventArgs e)
        {
            //this.textBox1.AppendText(e.Text + Environment.NewLine);
        }

        private void UpdateSettings()
        {
            _settings.Runner = settingsForm.Runner;
            _settings.WorkingDirectory = settingsForm.WorkingDirectory;
            _settings.Arguments = settingsForm.Arguments;
            _settings.Automatic = settingsForm.AutoArgs;
        }

        #endregion

        #region Events

        /* private void systemTabPage_DoubleClick(object sender, EventArgs e)
        {
            if (this.SelectedDesignItem != null)
            {
                if (this.SelectedDesignItem.GetType() == typeof(Subsystem))
                {
                    UserControl1 sf = new UserControl1();
                    //UserControl1 sf = new Us
                    //SubsystemForm sf = new SubsystemForm((Subsystem)this.SelectedDesignItem);
                    sf.ShowDialog(this);
                    sf.Dispose();
                }
                else if (this.SelectedDesignItem.GetType() == typeof(DependencyCollector))
                {
                    //DependencyForm df = new DependencyForm();
                    //df.ShowDialog();
                }
            }
            else if (this.SelectedConnector != null)
            {
                DependencyForm df = new DependencyForm(this.SelectedConnector);
                df.ShowDialog(this);
                df.Dispose();
            }
        }
        
        private void scenarioTabPage_Enter(object sender, EventArgs e)
        {
            this.propertyGrid1.Enabled = false;
        }

        private void systemTabPage_Enter(object sender, EventArgs e)
        {
            this.propertyGrid1.Enabled = true;
        } */

        private void buildScenarioToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.ProjectName == null)
            {
                this.buildScenarioFileDialog.ShowDialog();

                // If the file name is not an empty string open it for saving.
                if (this.buildScenarioFileDialog.FileName != "")
                {
                    String[] name = this.buildScenarioFileDialog.FileName.Split('\\');
                    this.ProjectName = name.Last().Replace(".xml", "");
                    String tempPath = this.buildScenarioFileDialog.FileName.Replace(this.ProjectName, "");

                    tempPath += this.ProjectName + "\\";

                    if (!Directory.Exists(tempPath))
                        Directory.CreateDirectory(tempPath);

                    this.ProjectDirectory = new DirectoryInfo(tempPath);
                }
            }

            String projectName = this.ProjectName;
            String path = this.ProjectDirectory.FullName;

            if (!Directory.Exists(path))
                Directory.CreateDirectory(path);

            projectName += "_simulation.xml";
            this.BuildSimulation(path, projectName);
            projectName = projectName.Replace("_simulation.xml", "_model.xml");
            this.BuildModel(path, projectName);

            if (!Directory.Exists(path + "\\Dependencies"))
                Directory.CreateDirectory(path + "\\Dependencies");
            projectName = projectName.Replace("_model.xml", "_dependencies.txt");
            this.BuildDependencyFile(path + "\\Dependencies\\", projectName);
            projectName = projectName.Replace("_dependencies.txt", "");

            if (!Directory.Exists(path + "\\Subsystems"))
                Directory.CreateDirectory(path + "\\Subsystems");
            this.BuildSubsystems(path + "\\Subsystems\\", projectName);
        }

        private void addSubToolStripButton_Click(object sender, EventArgs e)
        {
            //int assetIndex = 0;
            //if (this.picassoTreeView.SelectedNode.Parent == this.systemModelNode)
            //    assetIndex = this.picassoTreeView.SelectedNode.Index;

            //((SubsystemProperties)temp.Properties).Asset = this.Assets[assetIndex];

            if (this.picassoTreeView.SelectedNode.Tag.GetType() == typeof(PicassoGUI.Util.Asset))
            {
                //String name = "Subsystem " + this.SubsystemID.ToString();
                //Subsystem temp = new Subsystem((Asset)this.picassoTreeView.SelectedNode.Tag, name, this.previousSubsystemLocation);
                this.AddNewSubsystem();

                /*
                TreeNode tempNode = new TreeNode(temp.Name);
                tempNode.Tag = temp;
                tempNode.Text = temp.Name;
                this.picassoTreeView.SelectedNode.Nodes.Add(tempNode);

                this.drawingCanvas.AddObject(temp);
                this.SelectedDesignItem = temp;
                */

                /*
                this.SubsystemID++;
                if (this.previousSubsystemLocation.X < this.systemTabPage.Size.Width && this.previousSubsystemLocation.Y < this.systemTabPage.Size.Height)
                    this.previousSubsystemLocation = Point.Add(this.previousSubsystemLocation, new Size(20, 20));
                else
                    this.previousSubsystemLocation = new Point(20, 20);
                */

                /*
                this.mainFormPropertyGrid.SelectedObject = temp.Properties;
                this.tabControl.SelectedTab = this.systemTabPage;

                this.HasChanged = true;
                this.systemTabPage.Refresh();
                 */
                //this.Refresh();
            }

        }

        private void addDepToolStripButton_Click(object sender, EventArgs e)
        {
            // TODO; Make this like the addSubToolStrip... (add a constructor!)
            String name = "Dependency Collector " + this.DepCollectorID.ToString();
            DependencyCollector temp = new DependencyCollector(name, this.previousSubsystemLocation);
            temp.MinimumSize = new Size(40, 80);

            int assetIndex = 0;
            if (this.picassoTreeView.SelectedNode.Parent == this.systemModelNode)
                assetIndex = this.picassoTreeView.SelectedNode.Index;

            // TODO:  This breaks when a depCollector Asset is null
            ((DependencyCollectorProperties)temp.Properties).Asset = this.Assets[assetIndex];

            //this.DesignItems.Add(temp);
            this.drawingCanvas.AddObject(temp);
            this.SelectedDesignItem = temp;

            this.DepCollectorID++;
            //if (this.previousSubsystemLocation.X < this.systemTabPage.Size.Width && this.previousSubsystemLocation.Y < this.systemTabPage.Size.Height)
            if (true)
                this.previousSubsystemLocation = Point.Add(this.previousSubsystemLocation, new Size(20, 20));
            /* else
                this.previousSubsystemLocation = new Point(20, 20); */

            this.mainFormPropertyGrid.SelectedObject = temp.Properties;
            //this.tabControl.SelectedTab = this.systemTabPage;

            this.HasChanged = true;
            //this.systemTabPage.Refresh();
            //this.Refresh();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.HasChanged)
            {
                String message;
                if (this.ProjectName != null)
                    message = ("Would you like to Save changes to " + this.ProjectName + "?");
                else
                    message = ("Would you like to save the current scenario?");

                string caption = "Picasso Scenario";
                MessageBoxButtons buttons = MessageBoxButtons.YesNoCancel;
                DialogResult result;

                // Displays the MessageBox.
                result = MessageBox.Show(message, caption, buttons);

                if (result == DialogResult.Yes)
                {
                    this.saveDocument_Click(sender, e);
                    if (this.ProjectDirectory != null)
                        this.openScenarioDialog.InitialDirectory = this.ProjectDirectory.FullName;

                    this.Close();
                }
                else if (result == DialogResult.No)
                {
                    this.Close();
                }
            }
            else
                this.Close();
        }

        private void mainFormPropertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            this.drawingCanvas.Invalidate();
        }

        private void targetTabPage_Enter(object sender, EventArgs e)
        {
            //this.targetListView.Visible = true;
            //this.mainFormPropertyGrid.Visible = false;
        }

        private void scenarioTabPage_Enter(object sender, EventArgs e)
        {
            //this.mainFormPropertyGrid.Visible = true;
            //this.targetListView.Visible = false;
        }

        private void buildTargetDeckToolStripMenuItem_Click(object sender, EventArgs e)
        {

            String projectName = this.ProjectName;
            String path = this.ProjectDirectory.FullName;

            if (!Directory.Exists(path + "\\TargetDeck"))
                Directory.CreateDirectory(path + "\\TargetDeck");
            projectName = projectName + "_targetDeck.txt";
            this.targetViewer1.WriteTargets(path + "\\TargetDeck\\" + projectName);
            //this.BuildTargetDeckFile(path + "\\TargetDeck\\", projectName);
        }

        private void picassoTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            // Temporary for DX demo
            /*if (e.Node.Name == "resultsNode")
            {
                ResultsForm frmChild = new ResultsForm();
                frmChild.MdiParent = this;
                frmChild.Location = new Point(100, 100);
                frmChild.Show();
            }*/

            /* if (e.Node.Tag != null)
            {
                if (e.Node.Tag.GetType() == typeof(Subsystem))
                    this.mainFormPropertyGrid.SelectedObject = ((SubsystemProperties)((Subsystem)e.Node.Tag).Properties);
                else if (e.Node.Tag.GetType() == typeof(DependencyCollector))
                    this.mainFormPropertyGrid.SelectedObject = ((DependencyCollectorProperties)((DependencyCollector)e.Node.Tag).Properties);
                else if (e.Node.Tag.GetType() == typeof(Asset))
                    this.mainFormPropertyGrid.SelectedObject = (Asset)e.Node.Tag;
                else
                    this.mainFormPropertyGrid.SelectedObject = null;

                if (e.Node.Tag.GetType() == typeof(Asset))
                {
                    //this.tabControl.SelectedTab = this.environmentTabPage;
                    //this.environmentTabPage.Controls.Clear();
                    //this.environmentTabPage.Controls.Add(((Asset)e.Node.Tag).Position);
                }
                else if (e.Node.Tag.GetType() == typeof(TabPage))
                {
                    //this.tabControl.SelectedTab = (TabPage)e.Node.Tag;
                }
            } */

            //this.picassoTreeView.Focus();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Ask for new scenario name and add to form window title
            TextboxDialog nameDialog = new TextboxDialog("New Scenario", "Enter Scenario Name:");
            nameDialog.ShowDialog();
            string newName = nameDialog.StringValue;
            nameDialog.Dispose();
            Text = _textPrefix + " - " + newName;

            // Add new scenario object with default environment, etc
            ScenarioComponent newScenario = new ScenarioComponent(newName);
            ApplicationScenarios.Add(newScenario);

            // Add 3 random mission target and 1 random comm target as demo
            newScenario.TargetDeck.AddRandomTarget(PicassoGUI.Components.TargetType.LocationTarget);
            newScenario.TargetDeck.AddRandomTarget(PicassoGUI.Components.TargetType.LocationTarget);
            newScenario.TargetDeck.AddRandomTarget(PicassoGUI.Components.TargetType.LocationTarget);
            newScenario.TargetDeck.AddRandomTarget(PicassoGUI.Components.TargetType.FacilityTarget);

            // Add 1 subsystem and 1 constraint to initial model & asset as demo
            // Draw nodes at head of tree
            TreeNode blankNode = new TreeNode();
            newScenario.PopulateTree(picassoTreeView, blankNode);
        }

        private void addAssetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AssetForm af = new AssetForm();
            af.ShowDialog();

            if (af.DialogResult == DialogResult.OK)
            {
                PicassoGUI.Util.Asset newAsset = new PicassoGUI.Util.Asset(af.AssetName);
                //newAsset.Name = af.AssetName;
                newAsset.AssetID = af.AssetID;
                Assets.Add(newAsset);
                TreeNode newAssetNode = new TreeNode(newAsset.Name);
                newAssetNode.Name = newAsset.Name;
                newAssetNode.Tag = newAsset;
                this.systemModelNode.Nodes.Add(newAssetNode);

                //this.environmentTabPage.Controls.Clear();
                //this.environmentTabPage.Controls.Add(newAsset.Position);

                this.systemModelNode.Expand();

                this.addSubToolStripButton.Enabled = true;
                this.addDepToolStripButton.Enabled = true;

                this.HasChanged = true;
                this.saveToolStripButton.Enabled = true;
            }

            af.Dispose();
        }

        private void loadTargetDeckToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.targetViewer1.ImportTargets();
        }

        private void drawingCanvas_DoubleClick(object sender, EventArgs e)
        {
            if (this.drawingCanvas.SelectedDesignItem != null)
            {
                if (this.drawingCanvas.SelectedDesignItem.GetType() == typeof(PicassoGUI.Controls.Subsystem))
                {
                    SubsystemFormCC sf = new SubsystemFormCC((PicassoGUI.Controls.Subsystem)this.drawingCanvas.SelectedDesignItem);
                    sf.ShowDialog(this);
                    sf.Dispose();
                }
                else if (this.drawingCanvas.SelectedDesignItem.GetType() == typeof(DependencyCollector))
                {
                    DependencyCollectorForm df = new DependencyCollectorForm(((DependencyCollector)this.drawingCanvas.SelectedDesignItem));

                    df.ShowDialog(this);
                    df.Dispose();
                }
            }
            else if (this.drawingCanvas.SelectedDesignItem != null)
            {
                DependencyForm df = new DependencyForm(this.drawingCanvas.SelectedConnector);
                df.ShowDialog(this);
                df.Dispose();
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.drawingCanvas.SelectedDesignItem != null)
            {
                ((PicassoGUI.Controls.Subsystem)this.drawingCanvas.SelectedDesignItem).CopyToClipboard();
            }
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Paste object from clipboard and register event
        }

        private void launchScenarioToolStripMenuItem_Click(object sender, EventArgs e)
        {
            HorizonRunnerForm horizonRunner = new HorizonRunnerForm();
            horizonRunner.FileName = _settings.Runner;
            horizonRunner.WorkingDirectory = _settings.WorkingDirectory;
            horizonRunner.Arguments = _settings.Arguments;
            horizonRunner.OnSuccess = RunnerSucceded;
            horizonRunner.OnFailure = RunnerFailed;
            horizonRunner.Show();
            this.RunAndGetOutput(_settings.Runner);
            this.backgroundWorker1.RunWorkerAsync(_settings.Runner);
            hScenarioInRunner = currentScenarioFocus;
        }

        private void picassoTreeView_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            // When a node is clicked, reference it's tag object to create window, fill property grid
            if (e.Node.Tag == null)
            {
                MessageBox.Show("Node " + e.Node.Name + " (" + e.Node.Text + ") tag is null");
                return;
            }

            try
            {
                // If tag is a scenario, update form title
                if (e.Node.Tag.GetType() == typeof(ScenarioComponent))
                {
                    Text = _textPrefix + " - " + ((ScenarioComponent)(e.Node.Tag)).ScenarioName;
                }

                // Fill property grid
                IPicassoComponent clickedNode = (IPicassoComponent)e.Node.Tag;
                clickedNode.FillProperties(mainFormPropertyGrid);

                // Make sure window doesn't already exist! If it does, just set focus and return.
                foreach (Form childForm in MdiChildren)
                {
                    if (childForm.Tag == clickedNode)
                    {
                        childForm.Focus();

                        // Check to see if newly-selected node has same scenario as current; if not, switch
                        ScenarioComponent newScen = scenarioOfComponent(clickedNode);
                        if (currentScenarioFocus != newScen)
                        {
                            setScenarioFocus(newScen);
                        }
                        return;
                    }
                }
                clickedNode.FillWindow(this);

                // Check to see if newly-selected node has same scenario as current; if not, switch
                ScenarioComponent s = scenarioOfComponent(clickedNode);
                if (currentScenarioFocus != s)
                {
                    setScenarioFocus(s);
                }
            }
            catch (InvalidCastException)
            {
                MessageBox.Show("Node " + e.Node.Name + " (" + e.Node.Text + ") tag is not an IPicassoComponent");
            }
        }

        private ScenarioComponent scenarioOfComponent(IPicassoComponent comp)
        {
            // Returns the scenario component associated with (equal to or a child of) the given component
            Type tType = comp.GetType();
            if (tType == typeof(ScenarioComponent))
            {
                foreach (ScenarioComponent scen in ApplicationScenarios)
                {
                    if (scen == (ScenarioComponent)comp) return scen;
                }
                return null;
            }
            if (tType == typeof(ModelComponent))
            {
                foreach (ScenarioComponent scen in ApplicationScenarios)
                {
                    if (scen.Model == (ModelComponent)comp) return scen;
                }
                return null;
            }
            if (tType == typeof(EnvironmentComponent))
            {
                foreach (ScenarioComponent scen in ApplicationScenarios)
                {
                    if (scen.Environment == (EnvironmentComponent)comp) return scen;
                }
                return null;
            }
            if (tType == typeof(TargetdeckComponent))
            {
                foreach (ScenarioComponent scen in ApplicationScenarios)
                {
                    if (scen.TargetDeck == (TargetdeckComponent)comp) return scen;
                }
                return null;
            }
            return null;
        }

        private void picassoTreeView_MouseUp(object sender, MouseEventArgs e)
        {
            // Show menu on right mouse button click
            if (e.Button == MouseButtons.Right)
            {
                Point p = new Point(e.X, e.Y);
                TreeNode node = picassoTreeView.GetNodeAt(p);
                if (node != null)
                {
                    // Select node that was clicked
                    picassoTreeView.SelectedNode = node;

                    // Menu items will change with node type
                    switch (Convert.ToString(node.Tag))
                    {
                        case "PicassoGUI.Components.ModelComponent":
                            // Show "Add Asset", hide "Add Target"
                            assetMenuItem.Visible = true;
                            targetMenuItem.Visible = false;
                            queryMenuItem.Visible = true;
                            break;
                        case "PicassoGUI.Components.TargetdeckComponent":
                            // Show "Add Target", hide "Add Asset"
                            assetMenuItem.Visible = false;
                            targetMenuItem.Visible = true;
                            queryMenuItem.Visible = false;
                            break;
                        default:
                            assetMenuItem.Visible = false;
                            targetMenuItem.Visible = false;
                            queryMenuItem.Visible = false;
                            break;
                    }
                    nodeContextMenu.Show(picassoTreeView, p);
                }
            }
        }

        private void xmlMenuItem_Click(object sender, EventArgs e)
        {
            // Serialize node temporarily and display in window
            try
            {
                PicassoNode targetNode = (PicassoNode)(picassoTreeView.SelectedNode.Tag);
                Type nodeType = targetNode.GetType();
                XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
                ns.Add("", "");
                XmlSerializer xs = new XmlSerializer(nodeType);
                XmlTextWriter dumpWriter = new XmlTextWriter(_dumpFileName, Encoding.UTF8);
                xs.Serialize(dumpWriter, targetNode, ns);
                dumpWriter.Close();
                _xmlWindow.GoTo(_dumpFileName);
                _xmlWindow.Visible = true;
                _xmlWindow.Focus();
            }
            catch (NullReferenceException)
            {
                // Node does not have component assigned
                MessageBox.Show("Error: This node does not reference a PicassoGUI.Component instance");
            }
        }

        private void targetMenuItem_Click(object sender, EventArgs e)
        {
            // Create new target at random location in given targetdeck
            try
            {
                TargetdeckComponent targetDeck = (TargetdeckComponent)(picassoTreeView.SelectedNode.Tag);
                // Find form for this targetdeck
                foreach (Form currForm in MdiChildren)
                {
                    if (currForm.GetType() != typeof(TargetdeckForm))
                    {
                        continue;
                    }
                    if (((TargetdeckForm)currForm).Tag == targetDeck)
                    {
                        // Add new target to random location
                        Int32 seed = Convert.ToInt32(DateTime.Now.Ticks % 0x7fffffff);
                        Random generator = new Random(seed);
                        double longitude = generator.NextDouble() * 360.0 - 180.0;
                        double latitude = generator.NextDouble() * 180.0 - 90.0;
                        ((TargetdeckForm)currForm).AddNewTarget(latitude, longitude);

                        // Expand list, if not expanded
                        picassoTreeView.SelectedNode.Expand();
                        break;
                    }
                }
            }
            catch (NullReferenceException)
            {
                // Node does not have a component assigned
                MessageBox.Show("Error: Attempting to add target to object that is not a target deck");
            }
            catch (InvalidCastException)
            {
                // Not a TargetdeckComponent; ignore...
            }
        }

        private void exportMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "Xml files (*.xml)|*.xml|All files (*.*)|*.*";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                // Serialize node and write to file
                try
                {
                    PicassoNode targetNode = (PicassoNode)(picassoTreeView.SelectedNode.Tag);
                    Type nodeType = targetNode.GetType();
                    XmlSerializer xs = new XmlSerializer(nodeType);
                    XmlTextWriter dumpWriter = new XmlTextWriter(dialog.FileName, Encoding.UTF8);
                    xs.Serialize(dumpWriter, targetNode);
                    dumpWriter.Close();
                }
                catch (NullReferenceException)
                {
                    // Node does not have component assigned
                    MessageBox.Show("Error: This node does not reference a PicassoGUI.Component instance");
                }
            }
        }
        
        private void importMenuItem_Click(object sender, EventArgs e)
        {
            // Attempt to import (deserialize) component in selected node from given file
            OpenFileDialog openDiag = new OpenFileDialog();
            openDiag.Filter = "Xml files (*.xml)|*.xml|All files (*.*)|*.*";
            openDiag.FilterIndex = 1;
            openDiag.RestoreDirectory = true;

            if (openDiag.ShowDialog() == DialogResult.OK)
            {
                // Attempt to deserialize from file
                PicassoNode targetNode = (PicassoNode)picassoTreeView.SelectedNode.Tag;
                targetNode.Deserialize(openDiag.FileName);

                // Refresh values of active child
                try
                {
                    IMForm activeChild = (IMForm)ActiveMdiChild;
                    activeChild.RefreshValues();
                }
                catch (InvalidCastException)
                {
                    // Active Mdi child not an IMForm
                    MessageBox.Show("Active Mdi child not an IMForm");
                }
            }
            else
            {
                // Something wrong happene in file dialog
                MessageBox.Show("Unable to import from file " + openDiag.FileName);
            }
        }

        private void assetMenuItem_Click(object sender, EventArgs e)
        {
            // Add new asset to selected model
            try
            {
                // Find model component
                ModelComponent model = (ModelComponent)(picassoTreeView.SelectedNode.Tag);

                // Create new asset component and add
                string assetName = "Asset " + (model.Assets.Count + 1).ToString();
                AssetComponent newAsset = new AssetComponent(assetName);
                model.Assets.Add(newAsset);

                // Create new asset window and focus
                newAsset.FillWindow(this);

                // Add asset node to node list
                newAsset.PopulateTree(picassoTreeView, model.hNode);
                model.hNode.Expand();
            }
            catch (NullReferenceException)
            {
                // Node does not have a component assigned
                MessageBox.Show("Error: Attempting to add an asset to a node that has no component");
            }
            catch (InvalidCastException)
            {
                // Not a ModelComponent; ignore...
                MessageBox.Show("Error: Attempting to add an asset to a node that is not a model");
            }
        }

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Show settings dialog
            settingsForm.ShowDialog();

            // Update when closed
            UpdateSettings();
        }

        public void RunnerSucceded()
        {
            // Runner has succeeded; retrive results location and enable / initialize results node
            hScenarioInRunner.Results.Enable();
            MessageBox.Show("Runner succeded!");
            return;
        }

        public void RunnerFailed()
        {
            // Runner has failed; disable and empty results node
            hScenarioInRunner.Results.Disable();
            MessageBox.Show("Runner failed!");
            return;
        }

        #endregion

        #region Memento

        private MementoManager _blankManager;

        public void RefreshMementoMenus()
        {
            // Should update undo / redo menus in form's menu strip
            try
            {
                IMForm childForm = (IMForm)ActiveMdiChild;
                MementoManager activeManager = childForm.GetMementoManager();

                // Update undo menu item
                if (activeManager.hasUndo())
                {
                    undoToolStripMenuItem.Enabled = true;
                    undoToolStripMenuItem.Text = "Undo " + activeManager.GetUndoDesc();
                }
                else
                {
                    undoToolStripMenuItem.Enabled = false;
                    undoToolStripMenuItem.Text = "Undo";
                }

                // Update redo menu item
                if (activeManager.hasRedo())
                {
                    redoToolStripMenuItem.Enabled = true;
                    redoToolStripMenuItem.Text = "Redo " + activeManager.GetRedoDesc();
                }
                else
                {
                    redoToolStripMenuItem.Enabled = false;
                    redoToolStripMenuItem.Text = "Redo";
                }
            }
            catch (InvalidCastException)
            {
                // Active Mdi child is not an IMForm; disable menus
                //MessageBox.Show("Active Mdi child is not an IMForm");
                undoToolStripMenuItem.Enabled = false;
                undoToolStripMenuItem.Text = "Undo";
                redoToolStripMenuItem.Enabled = false;
                redoToolStripMenuItem.Text = "Redo";
            }
            catch (NullReferenceException)
            {
                // There is no currently-active Mdi child
                //MessageBox.Show("There is no currently-active Mdi child");
                undoToolStripMenuItem.Enabled = false;
                undoToolStripMenuItem.Text = "Undo";
                redoToolStripMenuItem.Enabled = false;
                redoToolStripMenuItem.Text = "Redo";
            }
        }

        public MementoManager GetMementoManager()
        {
            // Returns the MementoManager belonging to the form
            // Not applicable for main form in Picasso, because all Memento events happen per Mdi child
            return _blankManager;
        }

        public void RefreshValues()
        {
            // Updates value visualizations that draw from clonable objects
            // In the case of the main form, this refers to the node tree branch of active child
            try
            {
                IPicassoComponent activeComponent = (IPicassoComponent)picassoTreeView.SelectedNode.Tag;
            }
            catch (InvalidCastException)
            {
                // Currently-selected node does not have a IPicassoComponent tag
                MessageBox.Show("Currently-selected node does not have a IPicassoComponent tag");
            }
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Call undo of manager in currently-active child
            try
            {
                IMForm child = (IMForm)ActiveMdiChild;
                child.GetMementoManager().Undo();
                child.RefreshValues();
            }
            catch (InvalidCastException)
            {
                // Active Mdi child is not an IMForm
                MessageBox.Show("Active Mdi child is not an IMForm");
            }
            catch (NullReferenceException)
            {
                // Child or child's memento manager has not been initialized
                MessageBox.Show("Child or child's memento manager has not been initialized");
            }
        }
        #endregion

        private void queryMenuItem_Click(object sender, EventArgs e)
        {
            // Asset import query takes place from a dialog
            ImportDialog diag = new ImportDialog();
            if (diag.ShowDialog() == DialogResult.OK)
            {

            }
            else
            {
                MessageBox.Show("There was an error attempting to import an asset");
            }
        }
    }
}