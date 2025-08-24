#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Box.H>

#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Globals
char filename[256] = "";
int changed = 0;
int loading = 0;
bool is_fullscreen = false;
bool dark_mode = false;

Fl_Text_Buffer *textbuf = nullptr;
Fl_Box *status_bar = nullptr;
Fl_Double_Window *main_win = nullptr;
std::vector<std::string> recent_files;
const char *RECENT_FILENAME = "recent.txt";
const double AUTOSAVE_INTERVAL = 60.0;

// Forward declarations
void set_title();
void update_status();
void save_cb(Fl_Widget*, void*);
void saveas_cb(Fl_Widget*, void*);
void new_cb(Fl_Widget*, void*);
void open_cb(Fl_Widget*, void*);
void quit_cb(Fl_Widget*, void*);
void copy_cb(Fl_Widget*, void*);
void cut_cb(Fl_Widget*, void*);
void paste_cb(Fl_Widget*, void*);
void about_cb(Fl_Widget*, void*);
void toggle_darkmode_cb(Fl_Widget*, void*);
void toggle_fullscreen_cb(Fl_Widget*, void*);
void find_cb(Fl_Widget*, void*);
void replace_cb(Fl_Widget*, void*);
void load_recent();
void save_recent();
void add_recent(const char*);
int check_save();
void schedule_autosave(void*);

// EditorWindow class
struct EditorWindow : public Fl_Double_Window {
    Fl_Text_Editor *editor;
    Fl_Window *find_dlg, *replace_dlg;
    Fl_Input *find_input, *replace_find, *replace_with;
    char search[256];

    EditorWindow(int w, int h, const char *t)
        : Fl_Double_Window(w, h, t), editor(nullptr), find_dlg(nullptr), 
          replace_dlg(nullptr), find_input(nullptr), replace_find(nullptr), replace_with(nullptr)
    {
        search[0] = '\0';
    }
};

// Callbacks
void changed_cb(int, int nInserted, int nDeleted, int, const char *, void *) {
    if ((nInserted || nDeleted) && !loading) {
        changed = 1;
        set_title();
        update_status();
    }
}

void set_title() {
    static char title[512];
    const char *base = (filename[0]) ? strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename : "Untitled";
    snprintf(title, sizeof(title), "%s%s - Simple Editor", base, changed ? " *" : "");
    if (main_win) main_win->label(title);
}

void update_status() {
    if (!status_bar || !main_win || !textbuf) return;
    EditorWindow *ew = static_cast<EditorWindow*>(main_win);
    int pos = ew->editor->insert_position(), line = 1, col = 1;
    const char *text = textbuf->text();
    for (int i = 0; i < pos && text[i]; i++) {
        if (text[i] == '\n') { line++; col = 1; } else col++;
    }
    free((void*)text);
    char buf[512];
    snprintf(buf, sizeof(buf), "File: %s | Line: %d, Col: %d | Length: %d",
             filename[0] ? filename : "Untitled", line, col, textbuf->length());
    status_bar->copy_label(buf);
}

void save_cb(Fl_Widget*, void*) {
    if (!filename[0]) return saveas_cb(nullptr, nullptr);
    if (textbuf->savefile(filename) == 0) {
        changed = 0;
        add_recent(filename);
        set_title();
        update_status();
    }
}

void saveas_cb(Fl_Widget*, void*) {
    const char *newfile = fl_file_chooser("Save As", "*", filename);
    if (newfile) {
        if (textbuf->savefile(newfile) == 0) {
            strcpy(filename, newfile);
            changed = 0;
            add_recent(newfile);
            set_title();
            update_status();
        }
    }
}

void new_cb(Fl_Widget*, void*) {
    if (!check_save()) return;
    filename[0] = '\0';
    textbuf->select(0, textbuf->length());
    textbuf->remove_selection();
    changed = 0;
    set_title();
    update_status();
}

void open_cb(Fl_Widget*, void*) {
    if (!check_save()) return;
    const char *newfile = fl_file_chooser("Open", "*", filename);
    if (newfile) {
        loading = 1;
        if (textbuf->loadfile(newfile) == 0) {
            strcpy(filename, newfile);
            changed = 0;
            add_recent(newfile);
        }
        loading = 0;
        set_title();
        update_status();
    }
}

void quit_cb(Fl_Widget*, void*) {
    if (check_save()) exit(0);
}

int check_save() {
    if (!changed) return 1;
    int r = fl_choice("Save changes?", "Cancel", "Save", "Don't Save");
    if (r == 1) { save_cb(nullptr, nullptr); return !changed; }
    return (r == 2);
}

void copy_cb(Fl_Widget*, void*) {
    EditorWindow *ew = static_cast<EditorWindow*>(main_win);
    Fl_Text_Editor::kf_copy(0, ew->editor);
}

void cut_cb(Fl_Widget*, void*) {
    EditorWindow *ew = static_cast<EditorWindow*>(main_win);
    Fl_Text_Editor::kf_cut(0, ew->editor);
    changed = 1;
    set_title();
    update_status();
}

void paste_cb(Fl_Widget*, void*) {
    EditorWindow *ew = static_cast<EditorWindow*>(main_win);
    Fl_Text_Editor::kf_paste(0, ew->editor);
    changed = 1;
    set_title();
    update_status();
}

void about_cb(Fl_Widget*, void*) {
    fl_message("Simple Editor\nFLTK-based\nWith Autosave, Dark Mode, Fullscreen, Find & Replace");
}

void toggle_darkmode_cb(Fl_Widget*, void*) {
    dark_mode = !dark_mode;
    EditorWindow *ew = static_cast<EditorWindow*>(main_win);
    if (dark_mode) {
        ew->editor->color(fl_rgb_color(30,30,30));
        ew->editor->textcolor(FL_WHITE);
        ew->editor->cursor_color(FL_WHITE);
        status_bar->color(fl_rgb_color(20,20,20));
        status_bar->labelcolor(FL_WHITE);
        main_win->color(fl_rgb_color(30,30,30));
    } else {
        ew->editor->color(FL_WHITE);
        ew->editor->textcolor(FL_BLACK);
        ew->editor->cursor_color(FL_BLACK);
        status_bar->color(FL_WHITE);
        status_bar->labelcolor(FL_BLACK);
        main_win->color(FL_WHITE);
    }
    main_win->redraw();
}

void toggle_fullscreen_cb(Fl_Widget*, void*) {
    if (is_fullscreen) main_win->fullscreen_off();
    else main_win->fullscreen();
    is_fullscreen = !is_fullscreen;
}

void load_recent() {
    std::ifstream in(RECENT_FILENAME);
    std::string line;
    while (std::getline(in, line)) if (!line.empty()) recent_files.push_back(line);
}

void save_recent() {
    std::ofstream out(RECENT_FILENAME);
    for (const auto &f : recent_files) out << f << "\n";
}

void add_recent(const char *f) {
    recent_files.erase(std::remove(recent_files.begin(), recent_files.end(), f), recent_files.end());
    recent_files.insert(recent_files.begin(), f);
    if (recent_files.size() > 5) recent_files.resize(5);
    save_recent();
}

void schedule_autosave(void*) {
    if (changed && filename[0]) {
        char backup[300];
        snprintf(backup, sizeof(backup), "%s.autosave", filename);
        textbuf->savefile(backup);
    }
    Fl::repeat_timeout(AUTOSAVE_INTERVAL, schedule_autosave);
}

// Menu
Fl_Menu_Item menuitems[] = {
    {"&File", 0, 0, 0, FL_SUBMENU},
        {"&New", FL_CTRL + 'n', new_cb},
        {"&Open", FL_CTRL + 'o', open_cb},
        {"&Save", FL_CTRL + 's', save_cb},
        {"Save &As", FL_CTRL + FL_SHIFT + 's', saveas_cb},
        {"&Quit", FL_CTRL + 'q', quit_cb},
        {0},
    {"&Edit", 0, 0, 0, FL_SUBMENU},
        {"Cut", FL_CTRL + 'x', cut_cb},
        {"Copy", FL_CTRL + 'c', copy_cb},
        {"Paste", FL_CTRL + 'v', paste_cb},
        {0},
    {"&View", 0, 0, 0, FL_SUBMENU},
        {"Fullscreen", FL_F + 11, toggle_fullscreen_cb},
        {"Dark Mode", FL_CTRL + 'd', toggle_darkmode_cb},
        {0},
    {"&Help", 0, 0, 0, FL_SUBMENU},
        {"About", 0, about_cb},
        {0},
    {0}
};

int main(int argc, char **argv) {
    Fl::scheme("gtk+");
    load_recent();

    main_win = new EditorWindow(1000, 700, "Simple Editor");
    main_win->begin();

    Fl_Menu_Bar *menubar = new Fl_Menu_Bar(0, 0, 1000, 25);
    menubar->copy(menuitems);

    EditorWindow *ew = static_cast<EditorWindow*>(main_win);
    ew->editor = new Fl_Text_Editor(0, 25, 1000, 650);
    ew->editor->textfont(FL_COURIER);
    ew->editor->textsize(14);
    textbuf = new Fl_Text_Buffer;
    ew->editor->buffer(textbuf);
    textbuf->add_modify_callback(changed_cb, main_win);

    status_bar = new Fl_Box(0, 675, 1000, 25, "Ready");
    status_bar->box(FL_DOWN_BOX);
    status_bar->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    main_win->resizable(ew->editor);
    main_win->end();
    main_win->show(argc, argv);

    set_title();
    update_status();
    Fl::add_timeout(AUTOSAVE_INTERVAL, schedule_autosave);
    return Fl::run();
}
