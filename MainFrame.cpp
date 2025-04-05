
#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(wxID_ANY, MainFrame::OnToggleControlPanel)
    EVT_SLIDER(wxID_ANY, MainFrame::OnSliderChange)
    EVT_CHECKBOX(wxID_ANY, MainFrame::OnCheckBox)
    EVT_COLOURPICKER_CHANGED(wxID_ANY, MainFrame::OnColorChanged)
    EVT_CHOICE(wxID_ANY, MainFrame::OnModelSelected)
    EVT_TEXT_ENTER(wxID_ANY, MainFrame::OnPrecisionChange)
wxEND_EVENT_TABLE()

MainFrame::MainFrame() 
    : wxFrame(nullptr, wxID_ANY, "Modern OpenGL Canvas", wxDefaultPosition, wxSize(800, 700)) {
    
    SetupMenuBar();
    
    #ifdef __WXMSW__
    wxArtProvider::PushBack(new wxWindowsArtProvider);
    #endif
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxGLAttributes glAttrs;
    glAttrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(24).EndList();
    glCanvas = new GLCanvas(this, glAttrs);
    mainSizer->Add(glCanvas, 1, wxEXPAND);
    
    controlPanel = new wxPanel(this, wxID_ANY);
    CreateModernControls();
    
    mainSizer->Add(controlPanel, 0, wxEXPAND);
    controlPanel->Hide();
    
    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 300));
}

void MainFrame::CreateModernControls() {
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* rotationBox = new wxStaticBoxSizer(wxVERTICAL, controlPanel, "Rotation");
    rotationSlider = new wxSlider(rotationBox->GetStaticBox(), wxID_ANY, 0, 0, 360);
    rotationBox->Add(rotationSlider, 0, wxEXPAND | wxALL, 5);
    
    wxBoxSizer* precisionSizer = new wxBoxSizer(wxHORIZONTAL);
    precisionSizer->Add(new wxStaticText(rotationBox->GetStaticBox(), wxID_ANY, "Angle:"), 0, wxALIGN_CENTER_VERTICAL);
    precisionInput = new wxTextCtrl(rotationBox->GetStaticBox(), wxID_ANY, "0", 
                                  wxDefaultPosition, wxSize(60, -1), wxTE_PROCESS_ENTER);
    precisionSizer->Add(precisionInput, 0, wxLEFT, 5);
    rotationBox->Add(precisionSizer, 0, wxEXPAND | wxALL, 5);
    panelSizer->Add(rotationBox, 0, wxEXPAND | wxALL, 5);
    
    wxStaticBoxSizer* colorBox = new wxStaticBoxSizer(wxVERTICAL, controlPanel, "Colors");
    colorBox->Add(new wxStaticText(colorBox->GetStaticBox(), wxID_ANY, "Background:"), 0, wxTOP, 5);
    bgColorPicker = new wxColourPickerCtrl(colorBox->GetStaticBox(), wxID_ANY, *wxBLACK);
    colorBox->Add(bgColorPicker, 0, wxEXPAND | wxALL, 5);
    
    colorBox->Add(new wxStaticText(colorBox->GetStaticBox(), wxID_ANY, "Object:"), 0, wxTOP, 5);
    objColorPicker = new wxColourPickerCtrl(colorBox->GetStaticBox(), wxID_ANY, *wxWHITE);
    colorBox->Add(objColorPicker, 0, wxEXPAND | wxALL, 5);
    panelSizer->Add(colorBox, 0, wxEXPAND | wxALL, 5);
    
    wxStaticBoxSizer* modelBox = new wxStaticBoxSizer(wxVERTICAL, controlPanel, "Model");
    modelSelector = new wxChoice(modelBox->GetStaticBox(), wxID_ANY);
    modelSelector->Append("Cube");
    modelSelector->Append("Sphere");
    modelSelector->Append("Torus");
    modelSelector->Append("Teapot");
    modelSelector->SetSelection(0);
    modelSelector->Bind(wxEVT_CHOICE, &MainFrame::OnModelSelected, this);
    modelBox->Add(modelSelector, 0, wxEXPAND | wxALL, 5);
    panelSizer->Add(modelBox, 0, wxEXPAND | wxALL, 5);
    
    wireframeCheckBox = new wxCheckBox(controlPanel, wxID_ANY, "Wireframe Mode");
    panelSizer->Add(wireframeCheckBox, 0, wxEXPAND | wxALL, 5);
    
    controlPanel->SetSizer(panelSizer);
}

void MainFrame::SetupMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);
    
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxTheApp->Exit(); }, wxID_EXIT);
}

void MainFrame::OnToggleControlPanel(wxCommandEvent& event) {
    controlPanel->Show(!controlPanel->IsShown());
    Layout();
    Refresh();
}

void MainFrame::OnSliderChange(wxCommandEvent& event) {
    float angle = rotationSlider->GetValue();
    glCanvas->SetRotationAngle(angle);
    precisionInput->SetValue(wxString::Format("%.1f", angle));
    glCanvas->Refresh();
}

void MainFrame::OnCheckBox(wxCommandEvent& event) {
    glCanvas->SetWireframeMode(wireframeCheckBox->IsChecked());
    glCanvas->Refresh();
}

void MainFrame::OnColorChanged(wxColourPickerEvent& event) {
    if (event.GetEventObject() == bgColorPicker)
        glCanvas->SetBackgroundColor(event.GetColour());
     else 
        glCanvas->SetObjectColor(event.GetColour());
}

void MainFrame::OnModelSelected(wxCommandEvent& event) {
    glCanvas->SetCurrentModel(modelSelector->GetSelection());
}

void MainFrame::OnPrecisionChange(wxCommandEvent& event) {
    double value;
    if (precisionInput->GetValue().ToDouble(&value)) {
        value = fmod(value, 360.0);
        if (value < 0) value += 360.0;
        rotationSlider->SetValue(static_cast<int>(value));
        glCanvas->SetRotationAngle(value);
        glCanvas->Refresh();
    }
}