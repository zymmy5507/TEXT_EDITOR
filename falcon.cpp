#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Editor.H>

int main(int argc, char **argv) {
    main_win = new EditorWindow(1000, 700, "Simple Editor");
    main_win->begin();
    
    Fl_Menu_Bar *menubar = new Fl_Menu_Bar(0, 0, 1000, 25);
    // Add basic menu structure with placeholder callbacks
    
    Fl_Text_Editor *editor = new Fl_Text_Editor(0, 25, 1000, 675);
    textbuf = new Fl_Text_Buffer;
    editor->buffer(textbuf);
    
    main_win->end();
    main_win->show(argc, argv);
    return Fl::run();
}