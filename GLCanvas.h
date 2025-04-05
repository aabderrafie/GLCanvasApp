#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <vector>

class GLCanvas : public wxGLCanvas {
public:
    GLCanvas(wxWindow* parent, const wxGLAttributes& canvasAttrs);
    ~GLCanvas();
    
    void SetRotationAngle(float angle) { rotationAngle = angle; }
    void SetWireframeMode(bool enabled) { wireframeMode = enabled; }
    void SetBackgroundColor(const wxColour& color) { bgColor = color; UpdateBackgroundColor(); Refresh(); }
    void SetObjectColor(const wxColour& color) { objectColor = color; Refresh(); }
    void SetCurrentModel(int model) { currentModel = static_cast<ModelType>(model); Refresh(); }
    
protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    
private:
    enum ModelType { CUBE, SPHERE, TORUS, TEAPOT };
    
    wxGLContext* glContext;
    float rotationAngle;
    bool wireframeMode;
    wxColour bgColor;
    wxColour objectColor;
    ModelType currentModel;
    std::vector<wxPoint> buttonHoverStates;
    
    void InitGL();
    void UpdateBackgroundColor();
    void RenderScene();
    void RenderModernUI();
    void RenderTooltip(const wxString& text, int x, int y);
    void CreateModernButton(int id, const wxString& label, const wxRect& bounds, bool hovered);
    
    wxDECLARE_EVENT_TABLE();
};