#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/clrpicker.h>
#include <wx/numdlg.h>
#include "GLCanvas.h"

class MainFrame : public wxFrame {
public:
    MainFrame();
    
    void OnToggleControlPanel(wxCommandEvent& event);
    void OnSliderChange(wxCommandEvent& event);
    void OnCheckBox(wxCommandEvent& event);
    void OnColorChanged(wxColourPickerEvent& event);
    void OnModelSelected(wxCommandEvent& event);
    void OnPrecisionChange(wxCommandEvent& event);
    
    GLCanvas* glCanvas;
    wxPanel* controlPanel;
    wxSlider* rotationSlider;
    wxCheckBox* wireframeCheckBox;
    wxColourPickerCtrl* bgColorPicker;
    wxColourPickerCtrl* objColorPicker;
    wxChoice* modelSelector;
    wxTextCtrl* precisionInput;
    
private:
    void SetupMenuBar();
    void CreateModernControls();
    wxDECLARE_EVENT_TABLE();
};