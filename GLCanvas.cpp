#include "GLCanvas.h"
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <GLUT/glut.h>

wxBEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
    EVT_PAINT(GLCanvas::OnPaint)
    EVT_SIZE(GLCanvas::OnSize)
    EVT_MOUSE_EVENTS(GLCanvas::OnMouse)
    EVT_MOTION(GLCanvas::OnMouseMove)
    EVT_ERASE_BACKGROUND(GLCanvas::OnEraseBackground)
wxEND_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow* parent, const wxGLAttributes& canvasAttrs)
    : wxGLCanvas(parent, canvasAttrs, wxID_ANY, wxDefaultPosition, 
                wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
      rotationAngle(0.0f), wireframeMode(false) {
    
    static bool glutInitialized = false;
    if (!glutInitialized) {
        int argc = 1;
        char* argv[1] = {(char*)"GLCanvasApp"};
        glutInit(&argc, argv);
        glutInitialized = true;
    }
    
    glContext = new wxGLContext(this);
    SetCurrent(*glContext);
    InitGL();
}

GLCanvas::~GLCanvas() {
    delete glContext;
}

void GLCanvas::InitGL() {
    SetCurrent(*glContext);
    bgColor = wxColour(50, 50, 60);
    objectColor = wxColour(255, 255, 255);
    currentModel = CUBE;
    
    UpdateBackgroundColor();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    
    GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
}

void GLCanvas::UpdateBackgroundColor() {
    glClearColor(bgColor.Red()/255.0f, bgColor.Green()/255.0f, 
                bgColor.Blue()/255.0f, 1.0f);
}

void GLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event)) {
    wxPaintDC(this);
    SetCurrent(*glContext);
    
    RenderScene();
    RenderModernUI();
    
    glFlush();
    SwapBuffers();
}

void GLCanvas::OnSize(wxSizeEvent& event) {
    event.Skip();
    wxSize size = GetClientSize();
    SetCurrent(*glContext);
    
    glViewport(0, 0, size.x, size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)size.x/(float)size.y, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Refresh();
}

void GLCanvas::OnMouse(wxMouseEvent& event) {
    if (event.ButtonDown(wxMOUSE_BTN_LEFT)) {
        wxPoint mousePos = event.GetPosition();
        for (size_t i = 0; i < buttonHoverStates.size(); i++) {
            wxRect bounds(10 + i*120, 10, 100, 40);
            if (bounds.Contains(mousePos)) {
                wxCommandEvent buttonEvent(wxEVT_BUTTON, buttonHoverStates[i].x);
                ProcessWindowEvent(buttonEvent);
            }
        }
    }
    event.Skip();
}

void GLCanvas::OnMouseMove(wxMouseEvent& event) {
    wxPoint mousePos = event.GetPosition();
    bool needRefresh = false;
    
    for (size_t i = 0; i < buttonHoverStates.size(); i++) {
        wxRect bounds(10 + i*120, 10, 100, 40);
        bool hovered = bounds.Contains(mousePos);
        if (hovered != (buttonHoverStates[i].y == 1)) {
            buttonHoverStates[i].y = hovered ? 1 : 0;
            needRefresh = true;
        }
    }
    
    if (needRefresh) Refresh();
    event.Skip();
}

void GLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {}

void GLCanvas::RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
    
    glColor3f(objectColor.Red()/255.0f, objectColor.Green()/255.0f, 
             objectColor.Blue()/255.0f);
    
    switch(currentModel) {
        case CUBE:
            glutSolidCube(2.0);
            break;
        case SPHERE:
            glutSolidSphere(1.0, 32, 32);
            break;
        case TORUS:
            glutSolidTorus(0.5, 1.0, 32, 32);
            break;
        case TEAPOT:
            glutSolidTeapot(1.0);
            break;
    }
}

void GLCanvas::RenderModernUI() {
    wxSize size = GetClientSize();
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, size.x, size.y, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(0.1f, 0.1f, 0.15f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(size.x, 0.0f);
        glVertex2f(size.x, 60.0f);
        glVertex2f(0.0f, 60.0f);
    glEnd();
    
    if (buttonHoverStates.empty()) {
        buttonHoverStates.push_back(wxPoint(1, 0));
        buttonHoverStates.push_back(wxPoint(2, 0));
    }
    
    CreateModernButton(1, "Controls", wxRect(10, 10, 100, 40), buttonHoverStates[0].y == 1);
    CreateModernButton(2, "Settings", wxRect(130, 10, 100, 40), buttonHoverStates[1].y == 1);
    
    wxPoint mousePos = ScreenToClient(wxGetMousePosition());
    for (size_t i = 0; i < buttonHoverStates.size(); i++) {
        wxRect bounds(10 + i*120, 10, 100, 40);
        if (bounds.Contains(mousePos)) {
            RenderTooltip(i == 0 ? "Toggle control panel" : "Open settings", 
                         bounds.GetRight() + 5, bounds.GetTop());
        }
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void GLCanvas::CreateModernButton(int id, const wxString& label, const wxRect& bounds, bool hovered) {
    float x = bounds.x, y = bounds.y, w = bounds.width, h = bounds.height;
    float radius = 5.0f;
    
    glColor4f(0.3f, 0.3f, 0.35f, hovered ? 0.9f : 0.7f);
    if (hovered) glColor4f(0.4f, 0.4f, 0.45f, 0.9f);
    
    glBegin(GL_QUADS);
        glVertex2f(x + radius, y);
        glVertex2f(x + w - radius, y);
        glVertex2f(x + w - radius, y + h);
        glVertex2f(x + radius, y + h);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex2f(x, y + radius);
        glVertex2f(x + radius, y + radius);
        glVertex2f(x + radius, y + h - radius);
        glVertex2f(x, y + h - radius);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex2f(x + w - radius, y + radius);
        glVertex2f(x + w, y + radius);
        glVertex2f(x + w, y + h - radius);
        glVertex2f(x + w - radius, y + h - radius);
    glEnd();
    
    glColor4f(0.6f, 0.6f, 0.65f, 0.8f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x + radius, y);
        glVertex2f(x + w - radius, y);
        glVertex2f(x + w, y + radius);
        glVertex2f(x + w, y + h - radius);
        glVertex2f(x + w - radius, y + h);
        glVertex2f(x + radius, y + h);
        glVertex2f(x, y + h - radius);
        glVertex2f(x, y + radius);
    glEnd();
    glLineWidth(1.0f);
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glRasterPos2f(x + w/2 - label.length()*3, y + h/2 + 4);
    for (const char c : label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void GLCanvas::RenderTooltip(const wxString& text, int x, int y) {
    glColor4f(0.1f, 0.1f, 0.1f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + text.length() * 7 + 10, y);
        glVertex2f(x + text.length() * 7 + 10, y + 20);
        glVertex2f(x, y + 20);
    glEnd();
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glRasterPos2f(x + 5, y + 14);
    for (const char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}