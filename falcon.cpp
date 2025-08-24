#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Box.H>  // Required for status bar

// Global variables
Fl_Double_Window *main_win = nullptr;
Fl_Text_Buffer *textbuf = nullptr;
Fl_Box *status_bar = nullptr;

int main(int argc, char **argv) {
    // Create the main window
    main_win = new Fl_Double_Window(1000, 700, "Simple Editor");
    main_win->begin();

    // Menu bar
    Fl_Menu_Bar *menubar = new Fl_Menu_Bar(0, 0, 1000, 25);
    // i will work later

    // Text editor area
    Fl_Text_Editor *editor = new Fl_Text_Editor(0, 25, 1000, 650);
    textbuf = new Fl_Text_Buffer;
    editor->buffer(textbuf);

    // Status bar
    status_bar = new Fl_Box(0, 675, 1000, 25, "Ready");
    status_bar->box(FL_DOWN_BOX);
    status_bar->labelfont(FL_HELVETICA);
    status_bar->labelsize(12);
    status_bar->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    main_win->resizable(editor);
    main_win->end();
    main_win->show(argc, argv);
    return Fl::run();
}
