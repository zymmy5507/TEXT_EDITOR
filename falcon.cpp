#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>

#include <cstring>
#include <cerrno>
#include <cstdio>

// Global variables
Fl_Double_Window *main_win = nullptr;
Fl_Text_Editor *editor = nullptr;
Fl_Text_Buffer *textbuf = nullptr;
Fl_Box *status_bar = nullptr;

int changed = 0;
int loading = 0;
char filename[256] = "";

// --- File Menu Functions ---
void set_title(Fl_Window *w) {
    if (!w) return;
    static char title[512];
    const char *base;

    if (filename[0] == '\0') {
        base = "Untitled";
    } else {
        const char *slash = strrchr(filename, '/');
        base = slash ? slash + 1 : filename;
    }

    snprintf(title, sizeof(title), "%s%s - Simple Editor", base, changed ? " *" : "");
    w->label(title);
}

int check_save() {
    if (!changed) return 1;

    int r = fl_choice("The current file has not been saved.\nWould you like to save it now?",
                      "Cancel", "Save", "Don't Save");
    if (r == 1) {
        save_cb(nullptr, nullptr);
        return !changed;
    }
    return (r == 2) ? 1 : 0;
}

void new_cb(Fl_Widget *, void *) {
    if (!check_save()) return;

    filename[0] = '\0';
    textbuf->select(0, textbuf->length());
    textbuf->remove_selection();
    changed = 0;
    set_title(main_win);
    status_bar->label("New file created");
}

void open_cb(Fl_Widget *, void *) {
    if (!check_save()) return;

    const char *newfile = fl_file_chooser("Open File", "*", filename);
    if (newfile) {
        loading = 1;
        if (textbuf->loadfile(newfile) == 0) {
            strcpy(filename, newfile);
            changed = 0;
            status_bar->label("File opened");
        } else {
            fl_alert("Error reading from file '%s':\n%s.", newfile, strerror(errno));
        }
        loading = 0;
        set_title(main_win);
    }
}

void save_cb(Fl_Widget *, void *) {
    if (filename[0] == '\0') {
        saveas_cb(nullptr, nullptr);
        return;
    }

    if (textbuf->savefile(filename) == 0) {
        changed = 0;
        set_title(main_win);
        status_bar->label("File saved");
    } else {
        fl_alert("Error writing to file '%s':\n%s.", filename, strerror(errno));
    }
}

void saveas_cb(Fl_Widget *, void *) {
    const char *newfile = fl_file_chooser("Save File As?", "*", filename);
    if (newfile) {
        if (textbuf->savefile(newfile) == 0) {
            strcpy(filename, newfile);
            changed = 0;
            set_title(main_win);
            status_bar->label("File saved");
        } else {
            fl_alert("Error writing to file '%s':\n%s.", newfile, strerror(errno));
        }
    }
}

void changed_cb(int, int nInserted, int nDeleted, int, const char *, void *) {
    if ((nInserted || nDeleted) && !loading) {
        changed = 1;
        set_title(main_win);
        status_bar->label("Unsaved changes");
    }
}

// --- Edit Menu Functions ---
void cut_cb(Fl_Widget *, void *) {
    Fl_Text_Editor::kf_cut(0, editor);
    changed = 1;
    status_bar->label("Cut");
    set_title(main_win);
}

void copy_cb(Fl_Widget *, void *) {
    Fl_Text_Editor::kf_copy(0, editor);
    status_bar->label("Copied");
}

void paste_cb(Fl_Widget *, void *) {
    Fl_Text_Editor::kf_paste(0, editor);
    changed = 1;
    status_bar->label("Pasted");
    set_title(main_win);
}

int main(int argc, char **argv) {
    main_win = new Fl_Double_Window(1000, 700, "Simple Editor");
    main_win->begin();

    // Menu Bar
    Fl_Menu_Bar *menubar = new Fl_Menu_Bar(0, 0, 1000, 25);
    menubar->add("File/New",        FL_CTRL + 'n', new_cb);
    menubar->add("File/Open...",    FL_CTRL + 'o', open_cb);
    menubar->add("File/Save",       FL_CTRL + 's', save_cb);
    menubar->add("File/Save As...", FL_CTRL + FL_SHIFT + 's', saveas_cb);
    
    menubar->add("Edit/Cut",   FL_CTRL + 'x', cut_cb);
    menubar->add("Edit/Copy",  FL_CTRL + 'c', copy_cb);
    menubar->add("Edit/Paste", FL_CTRL + 'v', paste_cb);

    // Editor Area
    editor = new Fl_Text_Editor(0, 25, 1000, 650);
    textbuf = new Fl_Text_Buffer;
    editor->buffer(textbuf);
    textbuf->add_modify_callback(changed_cb, nullptr);

    // Status Bar
    status_bar = new Fl_Box(0, 675, 1000, 25, "Ready");
    status_bar->box(FL_DOWN_BOX);
    status_bar->labelfont(FL_HELVETICA);
    status_bar->labelsize(12);
    status_bar->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    main_win->resizable(editor);
    main_win->end();
    main_win->show(argc, argv);

    set_title(main_win);
    return Fl::run();
}
