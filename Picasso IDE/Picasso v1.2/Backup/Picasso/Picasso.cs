using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Hsf.Utility;
using Hsf.Elements;
using Hsf.Components;
using MementoModel;
using Picasso.Controls;
using Picasso.Forms;
using Picasso.Dialogs;
using Picasso.Results;
using Picasso.Utility;

namespace Picasso
{
    public struct SettingsModel
    {
        public string Runner;
        public string WorkingDirectory;
        public string Arguments;
        public string Author;
        public bool Automatic;
    };

    public partial class Picasso : Form, IMForm, IMessageRouter
    {
        #region Variables

        private bool _isDebugging;                  // Debugging flag for error reporting
        private List<ScenarioForm> _scenarios;      // List of loaded scenarios
        private ScenarioComponent _activeScenario;  // Reference to currently-active scenario
        private SimpleBrowser _documentationForm;   // Form for browsing documentation
        private string _documentationUrl;           // Default URL for documentation resources
        private PicassoSettings _settingsDialog;    // Dialog for modifying project settings
        private SettingsModel _picassoSettings;     // Model for storing, retrieving settings information
        private string _textPrefix;                 // Main form title prefix; comes before name of current scenario
        private SimpleBrowser _xmlBrowser;          // Form for displaying xml serialization of elements
        private string _xmlDumpLocation;            // Path to temporary xml dump cache
        public ScenarioComponent ScenarioInRunner;  // Scenario currently being executed in runner
        public BackgroundWorker Worker;             // Handle / control point for asynch tasks

        #endregion

        #region Constructors

        public Picasso()
        {
            InitializeComponent();

            // Initialize Mdi workspace
            mainMenuStrip.MdiWindowListItem = windowMenuItem;

            // Initialize list of scenarios
            _scenarios = new List<ScenarioForm>();

            // Initialize documentation
            _documentationUrl = "http://tythos.net/horizon/documentation/picasso/";
            _documentationForm = new SimpleBrowser(_documentationUrl, false);
            _documentationForm.MdiParent = this;
            _documentationForm.Text = "Picasso Documentation";
            _documentationForm.Icon = Icon;
            _documentationForm.Persistent = true;

            // Initialize settings
            _settingsDialog = new PicassoSettings();
            _settingsDialog.Icon = Icon;
            UpdateSettings();

            // Initialize window title
            _textPrefix = "Horizon / Picasso";
            Text = _textPrefix;

            // Create a blank scenario when application launches
            ScenarioForm defaultScenario = new ScenarioForm(this);
            _scenarios.Add(defaultScenario);
            UpdateTreeView();

            // Create a SimpleBrower for displaying serialized elements, and set cache target
            _xmlBrowser = new SimpleBrowser("http://tythos.net/horizon/xmlBrowser.html", false);
            _xmlDumpLocation = "C:\\.horizonDump.xml";
            _xmlBrowser.MdiParent = this;
            _xmlBrowser.Persistent = true;

            // Set up background worker
            Worker = new BackgroundWorker();

            // Debugging setting: true will cause errors to be displayed via message box
            _isDebugging = true;
        }

        #endregion

        #region Methods

        public void ReportError(string e)
        {
            // Reports error, if debugging
            if (_isDebugging)
            {
                MessageBox.Show("Error: " + e);
            }
        }

        public void CreateNewScenario()
        {
            // Create a new scenario with the name entered in a textbox dialog
            TextboxDialog dialog = new TextboxDialog("Create new scenario", "New scenario name");
            dialog.ShowDialog();
            if (dialog.Cancelled)
            {
                return;
            }

            // Create new scenario form with given name
            ScenarioForm newScenario = new ScenarioForm(this);
            newScenario.Scenario.ScenarioName = dialog.StringValue;

            // Add new scenario to scenario list and tree view
            _scenarios.Add(newScenario);
            UpdateScenarioNode(newScenario);
            _activeScenario = newScenario.Scenario;
        }

        public void ShowDocumentation()
        {
            // Displays documentation form
            _documentationForm.Show();
        }

        private void UpdateSettings()
        {
            _picassoSettings.Runner = _settingsDialog.Runner;
            _picassoSettings.WorkingDirectory = _settingsDialog.WorkingDirectory;
            _picassoSettings.Arguments = _settingsDialog.Arguments;
            _picassoSettings.Automatic = _settingsDialog.AutoArgs;
            _picassoSettings.Author = _settingsDialog.Author;
        }

        #endregion

        #region Tree Management

        public void UpdateTreeView()
        {
            // Refreshes tree view for all nodes
            foreach (ScenarioForm sf in _scenarios)
            {
                UpdateScenarioNode(sf);
            }
        }

        public void UpdateScenarioNode(ScenarioForm scenario)
        {
            // Refreshes tree for given scenario (a branch is created if one does not exist)
            if (scenario == null) return;
            TreeNode scenarioNode = null;
            foreach (TreeNode node in mainTreeView.Nodes)
            {
                if (scenario == (ScenarioForm)node.Tag)
                {
                    scenarioNode = node;
                    break;
                }
            }

            if (scenarioNode == null)
            {
                // Create a new node for this scenario
                scenarioNode = new TreeNode(scenario.Scenario.ScenarioName);
                scenarioNode.Tag = scenario;
                mainTreeView.Nodes.Add(scenarioNode);
            }

            // Update child nodes, as well: model, environment, targetdeck, and results nodes
            UpdateModelNode(scenario.Scenario.Model, scenarioNode);
            UpdateTargetdeckNode(scenario.Scenario.Targetdeck, scenarioNode);
            UpdateEnvironmentNode(scenario.Scenario.Environment, scenarioNode);
            UpdateResultsNode(scenarioNode);
        }

        public void UpdateResultsNode(TreeNode scenarioNode)
        {
            // Checks to make sure a results node exists for this scenario.
            TreeNode resultsNode = null;
            foreach (TreeNode node in scenarioNode.Nodes)
            {
                if (node.Text == "Results")
                {
                    resultsNode = node;
                    break;
                }
            }
            if (resultsNode == null)
            {
                resultsNode = new TreeNode("Results");
                scenarioNode.Nodes.Add(resultsNode);
            }

            // If no results have been loaded yet, disable
            if (resultsNode.Nodes.Count == 0)
            {
                resultsNode.ForeColor = Color.Gray;
                TreeNode statePlotNode = new TreeNode("State Histories");
                TreeNode visualFigNode = new TreeNode("Visualization");
                TreeNode gndOvrlayNode = new TreeNode("Ground Overlay");
                statePlotNode.Tag = new StateHistories(this);
                visualFigNode.Tag = new ModelView(this);
                gndOvrlayNode.Tag = new GroundTrack(this);
                resultsNode.Nodes.Add(statePlotNode);
                resultsNode.Nodes.Add(visualFigNode);
                resultsNode.Nodes.Add(gndOvrlayNode);
            }
        }

        public void UpdateModelNode(ModelComponent model, TreeNode scenarioNode)
        {
            // Check to see if node exists with given model and for corresponding assets (recursing, etc)
            TreeNode modelNode = null;
            foreach (TreeNode node in scenarioNode.Nodes)
            {
                try
                {
                    if (((ModelForm)node.Tag).Model == model)
                    {
                        modelNode = node;
                    }
                }
                catch (NullReferenceException)
                {
                    // Node doesn't have tag
                }
                catch (InvalidCastException)
                {
                    // Ignore; we just happened to check a node that wasn't a model node
                }
            }

            if (modelNode == null)
            {
                // Create node, since it doesn't exist
                modelNode = new TreeNode("Model");
                ModelForm newModelForm = new ModelForm(model, this, modelNode);
                modelNode.Tag = newModelForm;
                scenarioNode.Nodes.Add(modelNode);
            }

            // Check to see if assets are listed
            foreach (AssetElement ae in model.Assets) {
                TreeNode assetNode = null;
                try
                {
                    foreach (TreeNode node in modelNode.Nodes)
                    {
                        if (((AssetForm)node.Tag).Element == ae)
                        {
                            assetNode = node;
                            break;
                        }
                    }
                }
                catch (NullReferenceException)
                {
                    // Node has just been created; skip and continue to next asset
                    continue;
                }

                if (assetNode == null)
                {
                    // Create new asset node with appropriate new form as tag; make sure parent model node is expanded
                    assetNode = new TreeNode(ae.Name);
                    modelNode.Nodes.Add(assetNode);
                    modelNode.Expand();
                    AssetForm newAssetForm = new AssetForm(ae, this, assetNode);
                    assetNode.Tag = newAssetForm;
                }
            }
        }

        public void UpdateTargetdeckNode(TargetdeckComponent targetdeck, TreeNode scenarioNode) {
            // Check to see if node exists with given targetdeck component and for corresponding targets
            TreeNode targetdeckNode = null;
            foreach (TreeNode node in scenarioNode.Nodes)
            {
                try
                {
                    if (((TargetdeckForm)node.Tag).Targetdeck == targetdeck)
                    {
                        targetdeckNode = node;
                    }
                }
                catch (NullReferenceException)
                {
                    // This node doesn't have a tag
                }
                catch (InvalidCastException)
                {
                    // Ignore; we just happened to check a node that wasn't a targetdeck node
                }
            }

            if (targetdeckNode == null)
            {
                // Create if it doesn't exist
                targetdeckNode = new TreeNode("Target Deck");
                TargetdeckForm newTgForm = new TargetdeckForm(targetdeck, this, targetdeckNode);
                targetdeckNode.Tag = newTgForm;
                scenarioNode.Nodes.Add(targetdeckNode);
            }

            // Check for target nodes
            foreach (TargetElement t in targetdeck.Targets)
            {
                TreeNode targetNode = null;
                foreach (TreeNode node in targetdeckNode.Nodes)
                {
                    if (((TargetForm)node.Tag).Target == t)
                    {
                        targetNode = node;
                        break;
                    }
                }

                if (targetNode == null)
                {
                    // No node exists for this target (meaning no form is known, as well); create a node and form
                    targetNode = new TreeNode(t.TargetName);
                    TargetForm newTForm = new TargetForm(t, this, targetNode);
                    targetNode.Tag = newTForm;
                    targetdeckNode.Nodes.Add(targetNode);
                }
            }
        }

        public void UpdateEnvironmentNode(EnvironmentComponent env, TreeNode scenarioNode)
        {
            // Check to see if node exists with given component. If not, create it with the appropriate form.
            TreeNode envNode = null;
            foreach (TreeNode node in scenarioNode.Nodes)
            {
                try
                {
                    if (((EnvironmentForm)node.Tag).Environment == env)
                    {
                        envNode = node;
                        break;
                    }
                }
                catch (InvalidCastException)
                {
                    // Ignore; we just happened to check a node that wasn't an environment node
                }
            }

            if (envNode == null)
            {
                // Create new node with appropiate form tag
                envNode = new TreeNode("Environment");
                EnvironmentForm newForm = new EnvironmentForm(env, this, envNode);
                envNode.Tag = newForm;
                scenarioNode.Nodes.Add(envNode);
            }
        }

        #endregion

        #region Memento

        public void RefreshMementoMenus()
        {
            // Should update undo / redo menus in form's menu strip
            try
            {
                IMForm child = (IMForm)ActiveMdiChild;
                MementoManager manager = child.GetMementoManager();

                if (manager.hasUndo())
                {
                    undoMenuItem.Enabled = true;
                    undoMenuItem.Text = "Undo " + manager.GetUndoDesc();
                }
                else
                {
                    undoMenuItem.Enabled = false;
                    undoMenuItem.Text = "Undo";
                }

                if (manager.hasRedo())
                {
                    redoMenuItem.Enabled = true;
                    redoMenuItem.Text = "Redo " + manager.GetRedoDesc();
                }
                else
                {
                    redoMenuItem.Enabled = false;
                    redoMenuItem.Text = "Redo";
                }
            }
            catch (NullReferenceException)
            {
                // No active child yet; menus should be disabled
                undoMenuItem.Enabled = false;
                undoMenuItem.Text = "Undo";
                redoMenuItem.Enabled = false;
                redoMenuItem.Text = "Redo";
            }
            catch (InvalidCastException)
            {
                // Active Mdi Child is not an IMForm; disable menus
                undoMenuItem.Enabled = false;
                undoMenuItem.Text = "Undo";
                redoMenuItem.Enabled = false;
                redoMenuItem.Text = "Redo";
            }
        }

        public MementoManager GetMementoManager()
        {
            // Returns the MementoManager belonging to the form; for an Mdi interface, returns a blank manager
            MementoManager toReturn = new MementoManager(this);
            return toReturn;
        }

        public void RefreshValues()
        {
            // Updates value visualizations that draw from clonable objects
            // For an Mdi interface, refresh tree view for active child
            foreach (TreeNode node in mainTreeView.Nodes)
            {
                if (getScenarioFromNode(node) == _activeScenario)
                {
                    UpdateScenarioNode((ScenarioForm)(node.Tag));
                }
            }
        }

        #endregion

        #region Events

        private void newMenuItem_Click(object sender, EventArgs e)
        {
            // Menu item button: new scenario
            CreateNewScenario();
        }

        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            // Tool strip button: new scenario
            CreateNewScenario();
        }

        private ScenarioComponent getScenarioFromNode(TreeNode node)
        {
            // Returns the scenario component of the given node
            TreeNode focus = node;
            while (focus.Parent != null)
            {
                focus = focus.Parent;
            }
            try
            {
                ScenarioForm formReferenced = (ScenarioForm)focus.Tag;
                return formReferenced.Scenario;
            }
            catch (NullReferenceException)
            {
                ReportError("Node has no tag");
            }
            catch (InvalidCastException)
            {
                ReportError("Head node tag is not a scenario form");
            }
            return new ScenarioComponent();
        }

        private List<TreeNode> getAllNodes(TreeNodeCollection nodes)
        {
            // Returns flattened collection of all nodes and descendant nodes
            List<TreeNode> toReturn = new List<TreeNode>();
            foreach (TreeNode node in nodes)
            {
                toReturn.Add(node);
                toReturn.AddRange(getAllNodes(node.Nodes));
            }
            return toReturn;
        }

        private void mainTreeView_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            // Node has been clicked
            try
            {
                // Check for form and show
                PicassoSubform target = (PicassoSubform)e.Node.Tag;
                target.Show();
                target.Focus();

                // Highlight in property grid
                mainPropertyGrid.SelectedObject = target.Element;

                // If it belongs to a different scenario than the one current selected, change it
                ScenarioComponent targetScenario = getScenarioFromNode(e.Node);
                if (targetScenario != _activeScenario)
                {
                    _activeScenario = targetScenario;
                    Text = _textPrefix + " - " + _activeScenario.ScenarioName;

                    // Hide all open forms not underneath this scenario
                    List<TreeNode> allNodes = getAllNodes(mainTreeView.Nodes);
                    foreach (TreeNode node in allNodes)
                    {
                        if (getScenarioFromNode(node) != _activeScenario && ((Form)node.Tag).Visible)
                        {
                            ((Form)node.Tag).Visible = false;
                        }
                    }
                }
            }
            catch (NullReferenceException)
            {
                // Node has no form in tag
            }
            catch (InvalidCastException)
            {
                // Node tag is not a form
            }
        }

        private void exitMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void documentationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowDocumentation();
        }

        private void helpToolStripButton_Click(object sender, EventArgs e)
        {
            ShowDocumentation();
        }

        private void optionsMenuItem_Click(object sender, EventArgs e)
        {
            // Show settings dialog
            _settingsDialog.ShowDialog();
            UpdateSettings();
        }

        private void mainTreeView_MouseUp(object sender, MouseEventArgs e)
        {
            // If left-click, show context menu
            if (e.Button == MouseButtons.Right)
            {
                Point p = new Point(e.X, e.Y);
                TreeNode node = mainTreeView.GetNodeAt(p);
                if (node != null)
                {
                    // Select node that was clicked
                    mainTreeView.SelectedNode = node;

                    // Adjust menu items with node type
                    switch (Convert.ToString(node.Tag.GetType()))
                    {
                        case "Picasso.Forms.TargetdeckForm":
                            // Show 'Add Target', hide all others
                            addTargetMenuItem.Visible = true;
                            addAssetMenuItem.Visible = false;
                            importAssetMenuItem.Visible = false;
                            break;
                        case "Picasso.Forms.ModelForm":
                            // Show 'Add Asset', hide all others
                            addTargetMenuItem.Visible = false;
                            addAssetMenuItem.Visible = true;
                            importAssetMenuItem.Visible = true;
                            break;
                        default:
                            // Hide both Add 'Target' and 'Asset'
                            addTargetMenuItem.Visible = false;
                            addAssetMenuItem.Visible = false;
                            importAssetMenuItem.Visible = false;
                            break;
                    }
                    nodeContextMenu.Show(mainTreeView, p);
                }
            }
        }

        private void addTargetMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void xmlMenuItem_Click(object sender, EventArgs e)
        {
            // Serialize element and show in xml browser
            HsfElement currElement = ((PicassoSubform)mainTreeView.SelectedNode.Tag).Element;
            string serializedContent = currElement.ToXml();

            // Write content to xml cache
            FileStream file = new FileStream(_xmlDumpLocation, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(file);
            sw.Write(serializedContent);
            sw.Close();
            file.Close();

            // Navigate / refresh / show / focus
            _xmlBrowser.Url = _xmlDumpLocation;
            _xmlBrowser.Show();
            _xmlBrowser.Focus();
        }

        private void addAssetMenuItem_Click(object sender, EventArgs e)
        {
            // Get asset name from dialog
            TextboxDialog dialog = new TextboxDialog("Create new asset", "New asset name");
            dialog.ShowDialog();
            if (dialog.Cancelled)
            {
                return;
            }

            // Create new asset element with given name
            AssetElement ae = new AssetElement();
            ae.Name = dialog.StringValue;

            // Add asset to selected model
            TreeNode modelNode = mainTreeView.SelectedNode;
            ModelComponent model = ((ModelForm)mainTreeView.SelectedNode.Tag).Model;
            model.Assets.Add(ae);

            // Refresh to add node
            UpdateModelNode(model, modelNode.Parent);
            TreeNode newNode = getNodeFromElement(ae);
            mainTreeView.SelectedNode = newNode;
            PicassoSubform newForm = (PicassoSubform)newNode.Tag;
            newForm.Show();
        }

        #endregion

        #region Message Routing

        public void OnDoubleClick(object sender, MouseEventArgs e)
        {
            // Check sender type, respond to double-click
            if (sender.GetType() == typeof(SubsystemIcon))
            {
                // Show subsystem editor form
                SubsystemIcon icon = (SubsystemIcon)sender;
                TreeNode blankNode = new TreeNode();
                SubsystemForm newSubForm = new SubsystemForm(icon.subsystem, this, blankNode);
                newSubForm.Show();
            }
            if (sender.GetType() == typeof(TargetIcon))
            {
                // Open target form that corresponds to this icon
                TargetElement target = (TargetElement)(((TargetIcon)sender).Tag);
                TreeNode node = getNodeFromElement(target);
                ((Form)node.Tag).Show();
            }
        }

        private TreeNode getNodeFromElement(HsfElement element)
        {
            // Recursively searches all tree nodes for element in picasso subform tag
            foreach (TreeNode node in mainTreeView.Nodes)
            {
                TreeNode result = getNodeFromElement(element, node);
                if (result != null) { return result; }
            }
            return null;
        }

        private TreeNode getNodeFromElement(HsfElement element, TreeNode targetNode)
        {
            // Searches given node and (recursively) children for matching element
            try
            {
                if (((PicassoSubform)targetNode.Tag).Element == element)
                {
                    return targetNode;
                }
            }
            catch (InvalidCastException)
            {
                // Not a subform, or tag is null; check children anyways
            }
            catch (NullReferenceException)
            {
                // Looking at a node with no tag
            }

            // Check children
            TreeNodeCollection collection = targetNode.Nodes;
            foreach (TreeNode node in collection)
            {
                TreeNode result = getNodeFromElement(element, node);
                if (result != null) { return result; }
            }
            return null;
        }

        public void OnControlMove(object sender, MouseEventArgs e)
        {
            // Somewhere, a control has been moved
        }

        #endregion

        private void importAssetMenuItem_Click(object sender, EventArgs e)
        {
            // Show Import Dialog
            ImportDialog dialog = new ImportDialog();
            dialog.Icon = Icon;
            dialog.ShowDialog();
            if (dialog.Cancelled) return;

            // Create new asset element with given name
            AssetElement ae = new AssetElement();
            ae.FromXml(dialog.Asset.ToXml());

            // Add asset to selected model
            TreeNode modelNode = mainTreeView.SelectedNode;
            ModelComponent model = ((ModelForm)mainTreeView.SelectedNode.Tag).Model;
            model.Assets.Add(ae);

            // Refresh to add node
            UpdateModelNode(model, modelNode.Parent);
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Show About dialog window
            AboutDialog dialog = new AboutDialog();
            dialog.Icon = Icon;
            dialog.ShowDialog();
        }

        private void exportMenuItem_Click(object sender, EventArgs e)
        {
            // Export component from selected node to file
            HsfComponent currComponent;
            try
            {
                currComponent = (HsfComponent)(((PicassoSubform)mainTreeView.SelectedNode.Tag).Element);
            }
            catch (InvalidCastException)
            {
                // The element of this node's form isn't a component; that's ok
                return;
            }

            // Choose location / name of file
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "Xml file (.xml)|*.xml|All files|*.*";
            dialog.Title = "Export Component to File";
            dialog.ShowDialog();
            if (dialog.FileName == "") { return; }

            // Save component to file
            string serializedContent = currComponent.ToXml();
            FileStream fs = (FileStream)dialog.OpenFile();
            StreamWriter sw = new StreamWriter(fs);
            sw.Write(serializedContent);
            sw.Close();
            fs.Close();

            // Report
            MessageBox.Show("Component successfully written to " + dialog.FileName);
        }

        private void openScenarioFromFile()
        {
            // Opens new scenario using open file dialog
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Hsx file (.hsx)|*.hsx|All files|*.*";
            dialog.Title = "Open Scenario";
            if (dialog.ShowDialog() != DialogResult.OK) { return; }

            // Loads new scenario using components specified in given HSX file from dialog
            string filename = dialog.FileName;
            XqlParser myParser = new XqlParser(filename);
            List<Hashtable> result;

            // Determine paths
            result = myParser.Query("SELECT path FROM hsx"); string rootPath = (string)result[0]["path"];
            result = myParser.Query("SELECT filename FROM scenario"); string scenPath = (string)rootPath + result[0]["filename"];
            result = myParser.Query("SELECT filename FROM targetdeck"); string trgtPath = (string)rootPath + result[0]["filename"];
            result = myParser.Query("SELECT filename FROM model"); string modlPath = (string)rootPath + result[0]["filename"];

            // Create new scenario component from file and attach to form
            ScenarioComponent newScenario = new ScenarioComponent();
            newScenario.FromFile(scenPath);
            ScenarioForm scenarioForm = new ScenarioForm(newScenario, this);
            scenarioForm.FileTarget = filename;
            _scenarios.Add(scenarioForm);

            // Form takes care of itself via node update?
            // Create new targetdeck from file and attach to scenario
            TargetdeckComponent newTargetdeck = new TargetdeckComponent();
            newTargetdeck.FromFile(trgtPath);
            newScenario.Targetdeck = newTargetdeck;

            // Create new model from file and attach to scenario
            ModelComponent newModel = new ModelComponent();
            newModel.FromFile(modlPath);
            newScenario.Model = newModel;

            // Update node tree and select scenario
            UpdateScenarioNode(scenarioForm);
            TreeNode node = getNodeFromForm(scenarioForm);
            mainTreeView.SelectedNode = node;
        }

        private TreeNode getNodeFromForm(Form form)
        {
            // Returns the tree node that tags the given component
            foreach (TreeNode node in mainTreeView.Nodes)
            {
                try
                {
                    if (form == (Form)node.Tag)
                    {
                        return node;
                    }
                    else
                    {
                        foreach (TreeNode subnode in node.Nodes)
                        {
                            try
                            {
                                if (form == (Form)subnode.Tag)
                                {
                                    return subnode;
                                }
                            }
                            catch (InvalidCastException)
                            {
                                // Subnode tag isn't a form; that's ok
                            }
                        }
                    }
                }
                catch (InvalidCastException)
                {
                    // Node tag isn't a form; wierd, but ok (should be a scenario)
                }
            }
            ReportError("No node found that corresponds to form " + form.Name);
            return null;
        }

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            openScenarioFromFile();
        }

        private void openMenuItem_Click(object sender, EventArgs e)
        {
            openScenarioFromFile();
        }

        private void contextCloseMenuItem_Click(object sender, EventArgs e)
        {
            // Closes the currently selected scenario
            TreeNode selected = mainTreeView.SelectedNode;
            ScenarioComponent scenario = getScenarioFromNode(selected);
            closeScenario(scenario);
        }

        private TreeNode getNodeFromScenario(ScenarioComponent scenario)
        {
            // Returns the treenode that tags the given scenario's form
            foreach (TreeNode node in mainTreeView.Nodes)
            {
                if (((ScenarioForm)node.Tag).Scenario == scenario)
                {
                    return node;
                }
            }
            ReportError("No node found for the scenario " + scenario.ScenarioName);
            return null;
        }

        private void closeScenario(ScenarioComponent scenario)
        {
            // Closes scenario of given component
            TreeNode node = getNodeFromScenario(scenario);
            if (node == null)
            {
                ReportError("Cannot close scenario; no node found");
                return;
            }

            ScenarioForm form = (ScenarioForm)node.Tag;
            if (!form.IsSaved || form.FileTarget == "")
            {
                // Ask to save form
                if (MessageBox.Show("Save scenario before closing?", "Save Scenario", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    // Save to file
                    saveScenario(scenario);
                }
            }

            // Close all relevant forms, and remove from scenario & node list
            closeNodeForms(node);
            _scenarios.Remove(form);
            mainTreeView.Nodes.Remove(node);

            if (_scenarios.Count == 0)
            {
                // If there are no scenarios left, set active to null and disable close / save options
                _activeScenario = null;
                closeMenuItem.Enabled = false;
                saveMenuItem.Enabled = false;
            }
            else
            {
                // Otherwise, select first scenario
                _activeScenario = _scenarios[0].Scenario;
                _scenarios[0].Show();
            }
        }

        private void closeNodeForms(TreeNode targetNode)
        {
            try
            {
                Form form = (Form)targetNode.Tag;
                form.Close();
            }
            catch (InvalidCastException)
            {
                // Node tag wasn't a form; that's ok
            }
            catch (NullReferenceException)
            {
                // Node tag still wasn't a form; that's ok
            }

            foreach (TreeNode node in targetNode.Nodes)
            {
                closeNodeForms(node);
            }
        }

        private void closeMenuItem_Click(object sender, EventArgs e)
        {
            // Closes the currently active scenario
            closeScenario(_activeScenario);
        }

        private void saveMenuItem_Click(object sender, EventArgs e)
        {
            // Save currently selected scenario
            TreeNode selected = mainTreeView.SelectedNode;
            ScenarioComponent scenario = getScenarioFromNode(selected);
            saveScenario(scenario);
        }

        private void saveScenario(ScenarioComponent scenario)
        {
            // Saves scenario with given component to file
            TreeNode scenarioNode = null;
            foreach (TreeNode node in mainTreeView.Nodes)
            {
                if (((ScenarioForm)node.Tag).Scenario == scenario)
                {
                    scenarioNode = node;
                }
            }
            if (scenarioNode == null) {
                // No node exists with given scenario component
                ReportError("No node exists with given scenario component");
                return;
            }

            // Get file target from form
            ScenarioForm form = (ScenarioForm)scenarioNode.Tag;
            bool incrementVersion = false;
            if (form.FileTarget == "") {
                // Need to specify save file
                incrementVersion = true;
                SaveFileDialog dialog = new SaveFileDialog();
                dialog.Filter = "Hsx file (.hsx)|*.hsx|All files|*.*";
                dialog.Title = "Save Scenario";
                if (dialog.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                form.FileTarget = dialog.FileName;
            }
            Match match = Regex.Match(form.FileTarget, "[\\.a-zA-Z]+$");
            string fileName = Regex.Replace(match.Groups[0].ToString(), "\\.[a-zA-Z]{2,3}$", "");
            string basePath = Regex.Replace(form.FileTarget, "[\\.a-zA-Z]+$", "");

            // Write components to similar files in same directory
            string targetdeckFileName = fileName + "_targetdeck.xml";
            scenario.Targetdeck.ToFile(targetdeckFileName);
            string modelFileName = fileName + "_model.xml";
            scenario.Model.ToFile(modelFileName);
            string envFileName = fileName + "_env.xml";
            scenario.Environment.ToFile(envFileName);
            string scenarioFileName = fileName + "_scenario.xml";
            scenario.ToFile(scenarioFileName);

            // Increment version, if file specified, and gather file info
            if (incrementVersion)
            {
                form.Version += 0.1;
            }
            string fileInfo = "author=\"" + _picassoSettings.Author + "\" lastSaved=\"" + DateTime.Now.ToUniversalTime() + "\" version=\"" + form.Version + "\"";

            // Write hsx to file
            string contents = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE hsx>";
            contents += "<hsx path=\"" + basePath + "\" " + fileInfo + ">";
            contents += "<scenario filename=\"" + scenarioFileName + "\"></scenario>";
            contents += "<targetdeck filename=\"" + targetdeckFileName + "\"></targetdeck>";
            contents += "<model filename=\"" + modelFileName + "\"></model>";
            contents += "<environment filename=\"" + envFileName + "\"></environment>";
            contents += "</hsx>";
            FileStream fs = new FileStream(form.FileTarget, FileMode.OpenOrCreate);
            StreamWriter sw = new StreamWriter(fs);
            sw.Write(contents);
            sw.Close();
            fs.Close();

            // Mark scenario as saved, reset form title and disable save
            form.IsSaved = true;
            Text = _textPrefix + ": " + scenario.ScenarioName;
            saveMenuItem.Enabled = false;
        }

        private void saveAsMenuItem_Click(object sender, EventArgs e)
        {
            // Save currently-selected scenario as a new hsx assembly
            TreeNode selected = mainTreeView.SelectedNode;
            ScenarioComponent scenario = getScenarioFromNode(selected);

            // Clear file target for this scenario form to force save dialog
            TreeNode scenarioNode = null;
            foreach (TreeNode node in mainTreeView.Nodes)
            {
                if (((ScenarioForm)node.Tag).Scenario == scenario)
                {
                    scenarioNode = node;
                    break;
                }
            }
            if (scenarioNode == null)
            {
                // No top-level node exists with this scenario as a tag
                ReportError("No top-level node exists with this scenario as a tag");
                return;
            }
            ((ScenarioForm)scenarioNode.Tag).FileTarget = "";
            saveScenario(scenario);
        }

        private void launchMenuItem_Click(object sender, EventArgs e)
        {
            // Check for saved and launch runner for active scenario
            ScenarioForm form = (ScenarioForm)(getNodeFromElement(_activeScenario).Tag);
            if (form.IsSaved)
            {
                saveScenario(_activeScenario);
            }
            launchScenario(form);
        }

        private void launchScenario(ScenarioForm scenarioForm)
        {
            // Launches given scenario with new horizon runner
            HorizonRunnerForm horizonRunner = new HorizonRunnerForm();
            horizonRunner.FileName = _picassoSettings.Runner;
            horizonRunner.WorkingDirectory = _picassoSettings.WorkingDirectory;
            horizonRunner.OnSuccess = RunnerSucceded;
            horizonRunner.OnFailure = RunnerFailed;

            // Set up arguments (dynamically or not)
            if (_settingsDialog.AutoArgs)
            {
                // Mark file names
                ScenarioComponent baseScenario = (ScenarioComponent)scenarioForm.Element;
                string fileName = _picassoSettings.WorkingDirectory + @"\" + baseScenario.ScenarioName.Replace(" ", "");
                string scenarioFileName = fileName + "_scenario.xml";
                string targetdeckFileName = fileName + "_targetdeck.xml";
                string modelFileName = fileName + "_model.xml";
                string envFileName = fileName + "_env.xml";

                // Save scenario files to working directory
                baseScenario.ToFile(scenarioFileName);
                baseScenario.Targetdeck.ToFile(targetdeckFileName);
                baseScenario.Model.ToFile(modelFileName);
                baseScenario.Environment.ToFile(envFileName);

                // Create argument from file names and output (working) directory
                horizonRunner.Arguments = scenarioFileName + " " + targetdeckFileName + " " + modelFileName + " " + _picassoSettings.WorkingDirectory;
            }
            else
            {
                // Use static arguments from settings form
                horizonRunner.Arguments = _picassoSettings.Arguments;
            }

            // Show and start runner
            horizonRunner.Show();
            RunAndGetOutput(_picassoSettings.Runner);
            Worker.RunWorkerAsync(_picassoSettings.Runner);
            ScenarioInRunner = _activeScenario;
        }

        private void RunAndGetOutput(String pathName)
        {
            // Not entirely sure what this is for
        }

        public void RunnerSucceded()
        {
            // Successful runner execution
            MessageBox.Show("Runner has successfully completed!");
        }

        public void RunnerFailed()
        {
            // Failed runner execution
            MessageBox.Show("Runner encountered an error");
        }

        private void undoMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                IMForm child = (IMForm)ActiveMdiChild;
                MementoManager manager = child.GetMementoManager();
                manager.Undo();
                child.RefreshValues();
            }
            catch (NullReferenceException)
            {
                // There is no child form yet
            }
            catch (InvalidCastException)
            {
                // Mdi child is not an IMForm
            }
        }

        private void redoMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                IMForm child = (IMForm)ActiveMdiChild;
                MementoManager manager = child.GetMementoManager();
                manager.Redo();
                child.RefreshValues();
            }
            catch (NullReferenceException)
            {
                // There is no child form yet
            }
            catch (InvalidCastException)
            {
                // Mdi child is not an IMForm
            }
        }

        private void Picasso_MdiChildActivate(object sender, EventArgs e)
        {
            // When mdi child form changes, update menus
            RefreshMementoMenus();
        }
    }
}
