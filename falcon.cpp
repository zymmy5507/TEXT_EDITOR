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
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <cstring>
#include <cerrno>
#include <cstdio>

// Global variables
Fl_Double_Window *main_win = nullptr;
Fl_Text_Editor *editor = nullptr;
Fl_Text_Buffer *textbuf = nullptr;
Fl_Box *status_bar = nullptr;

Fl_Window *find_dlg = nullptr;
Fl_Input *find_input = nullptr;
char search[256];

Fl_Window *replace_dlg = nullptr;
Fl_Input *replace_find = nullptr;
Fl_Input *replace_with = nullptr;

int changed = 0;
int loading = 0;
char filename[256] = "";

// --- Utility ---
void set_title(Fl_Window *w) {
    if (!w) return;
    static char title[512];
    const char *base = filename[0] ? strrchr(filename, '/') : nullptr;
    base = base ? base + 1 : (filename[0] ? filename : "Untitled");

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

// --- File Menu ---
void new_cb(Fl_Widget *, void *) {
    if (!check_save()) return;
    filename[0] = '\0';
    textbuf->select(0, textbuf->length());
    textbuf->remove_selection();
    changed = 0;
    set_title(main_win);
    status_bar->label("New file");
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

void undo_cb(Fl_Widget *, void *) {
    textbuf->undo();
    changed = 1;
    status_bar->label("Undo");
    set_title(main_win);
}

void redo_cb(Fl_Widget *, void *) {
    fl_alert("Redo not implemented.");
}

void save_cb(Fl_Widget *, void *) {
    if (filename[0] == '\0') {
        saveas_cb(nullptr, nullptr);
        return;
    }
    if (textbuf->savefile(filename) == 0) {
        changed = 0;
        status_bar->label("File saved");
        set_title(main_win);
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
            status_bar->label("File saved");
            set_title(main_win);
        } else {
            fl_alert("Error writing to file '%s':\n%s.", newfile, strerror(errno));
        }
    }
}

void changed_cb(int, int nInserted, int nDeleted, int, const char *, void *) {
    if ((nInserted || nDeleted) && !loading) {
        changed = 1;
        set_title(main_win);
        status_bar->label("Modified");
    }
}

// --- Edit Menu ---
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

// --- Find/Replace ---
void find_next(const char *s) {
    if (!s || !*s) return;
    int pos = editor->insert_position();
    int found;
    if (textbuf->search_forward(pos, s, &found)) {
        textbuf->select(found, found + strlen(s));
        editor->insert_position(found + strlen(s));
        editor->show_insert_position();
        status_bar->label("Match found");
    } else {
        fl_alert("No more occurrences of '%s' found.", s);
    }
}

void find_ok_cb(Fl_Widget *, void *) {
    const char *s = find_input->value();
    if (s && *s) {
        strcpy(search, s);
        find_next(search);
    }
    find_dlg->hide();
}

void find_cancel_cb(Fl_Widget *, void *) {
    find_dlg->hide();
}

void find_cb(Fl_Widget *, void *) {
    if (!find_dlg) {
        find_dlg = new Fl_Window(350, 100, "Find");
        find_dlg->begin();
        new Fl_Box(10, 10, 60, 25, "Find:");
        find_input = new Fl_Input(70, 10, 200, 25);
        find_input->value(search);
        Fl_Button *find_btn = new Fl_Button(280, 10, 60, 25, "Find");
        find_btn->callback(find_ok_cb);
        Fl_Button *cancel_btn = new Fl_Button(280, 45, 60, 25, "Cancel");
        cancel_btn->callback(find_cancel_cb);
        find_dlg->end();
        find_dlg->set_modal();
    }
    find_dlg->show();
}

void replace_ok_cb(Fl_Widget *, void *) {
    const char *f = replace_find->value();
    const char *r = replace_with->value();
    if (!f || !*f) return;

    int pos = editor->insert_position();
    int found;
    if (textbuf->search_forward(pos, f, &found)) {
        textbuf->select(found, found + strlen(f));
        textbuf->remove_selection();
        textbuf->insert(found, r);
        editor->insert_position(found + strlen(r));
        editor->show_insert_position();
        changed = 1;
        status_bar->label("One replaced");
        set_title(main_win);
    } else {
        fl_alert("No occurrences of '%s' found.", f);
    }
}

void replace_all_cb(Fl_Widget *, void *) {
    const char *f = replace_find->value();
    const char *r = replace_with->value();
    if (!f || !*f) return;

    int count = 0;
    int pos = 0, found;
    while (textbuf->search_forward(pos, f, &found)) {
        textbuf->select(found, found + strlen(f));
        textbuf->remove_selection();
        textbuf->insert(found, r);
        pos = found + strlen(r);
        ++count;
    }
    if (count) {
        changed = 1;
        status_bar->label("Replaced multiple");
        set_title(main_win);
        fl_message("Replaced %d occurrence(s).", count);
    } else {
        fl_alert("No occurrences of '%s' found.", f);
    }
}

void replace_cancel_cb(Fl_Widget *, void *) {
    replace_dlg->hide();
}

void replace_cb(Fl_Widget *, void *) {
    if (!replace_dlg) {
        replace_dlg = new Fl_Window(350, 130, "Replace");
        replace_dlg->begin();
        new Fl_Box(10, 10, 60, 25, "Find:");
        replace_find = new Fl_Input(70, 10, 200, 25);
        new Fl_Box(10, 40, 60, 25, "Replace:");
        replace_with = new Fl_Input(70, 40, 200, 25);
        Fl_Button *replace_btn = new Fl_Button(280, 10, 60, 25, "Replace");
        replace_btn->callback(replace_ok_cb);
        Fl_Button *all_btn = new Fl_Button(280, 40, 60, 25, "All");
        all_btn->callback(replace_all_cb);
        Fl_Button *cancel_btn = new Fl_Button(280, 75, 60, 25, "Cancel");
        cancel_btn->callback(replace_cancel_cb);
        replace_dlg->end();
        replace_dlg->set_modal();
    }
    replace_dlg->show();
}

// --- Main ---
int main(int argc, char **argv) {
    main_win = new Fl_Double_Window(1000, 700, "Simple Editor");
    main_win->begin();

    Fl_Menu_Bar *menubar = new Fl_Menu_Bar(0, 0, 1000, 25);
    menubar->add("File/New", FL_CTRL + 'n', nullptr);
    menubar->add("File/Open...", FL_CTRL + 'o', nullptr);
    menubar->add("File/Save", FL_CTRL + 's', nullptr);
    menubar->add("File/Save As...", FL_CTRL + FL_SHIFT + 's', nullptr);
    menubar->add("File/Quit", FL_CTRL + 'q', quit_cb);

    menubar->add("Edit/Undo", FL_CTRL + 'z', nullptr);
    menubar->add("Edit/Redo", FL_CTRL + 'y', nullptr);
    menubar->add("Edit/Cut", FL_CTRL + 'x', nullptr);
    menubar->add("Edit/Copy", FL_CTRL + 'c', nullptr);
    menubar->add("Edit/Paste", FL_CTRL + 'v', nullptr);
    menubar->add("Edit/Find...", FL_CTRL + 'f', nullptr);
    menubar->add("Edit/Replace...", FL_CTRL + 'h', nullptr);

    menubar->add("Help/About", 0, about_cb);

    editor = new Fl_Text_Editor(0, 25, 1000, 650);
    textbuf = new Fl_Text_Buffer;
    editor->buffer(textbuf);
    textbuf->add_modify_callback(changed_cb, nullptr);

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
