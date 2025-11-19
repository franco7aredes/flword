#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>

// Definimos una tabla de estilos
// Estilo 'A' = Texto Normal
// Estilo 'B' = Negrita (Bold)
// Estilo 'C' = Rojo
// ... y así sucesivamente.
Fl_Text_Display::Style_Table_Entry estilos[] = {
    // Color,       Fuente,             Tamaño
    { FL_BLACK,     FL_HELVETICA,       16 }, // A - Normal
    { FL_BLACK,     FL_HELVETICA_BOLD,  16 }, // B - Negrita
    { FL_RED,       FL_HELVETICA,       16 }, // C - Rojo
    { FL_BLUE,      FL_COURIER,         16 }  // D - Code/Blue
};

class RichEditor : public Fl_Double_Window {
    Fl_Text_Editor *editor;
    Fl_Text_Buffer *text_buff;
    Fl_Text_Buffer *style_buff; // El buffer "sombra" que guarda el formato

public:
    RichEditor(int w, int h, const char* t) : Fl_Double_Window(w, h, t) {
        // 1. Inicializar Buffers
        text_buff = new Fl_Text_Buffer();
        style_buff = new Fl_Text_Buffer(); // Buffer paralelo

        // 2. Configurar Editor
        editor = new Fl_Text_Editor(10, 40, w-20, h-50);
        editor->buffer(text_buff);
        
        // Magia de FLTK: Ligar el buffer de estilo
        editor->highlight_data(style_buff, estilos, 4, 'A', 0, 0);
        
        // Configuración visual para escribir cómodo
        editor->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
        editor->linenumber_width(30); 

        // 3. Botones de la barra de herramientas
        Fl_Button *btn_bold = new Fl_Button(10, 5, 70, 30, "Negrita");
        btn_bold->callback(ApplyStyleCB, (void*)'B'); // 'B' es el índice en la tabla

        Fl_Button *btn_red = new Fl_Button(90, 5, 70, 30, "Rojo");
        btn_red->callback(ApplyStyleCB, (void*)'C');

        Fl_Button *btn_norm = new Fl_Button(170, 5, 70, 30, "Normal");
        btn_norm->callback(ApplyStyleCB, (void*)'A');

        // Inicializar con texto de prueba
        text_buff->text("Esto es texto normal.\nEsto sera negrita.\nY esto rojo.");
        style_buff->text("AAAAAAAAAAAAAAAAAAAAA\nBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCC");
        
        end();
        resizable(editor);
    }

    // Función estática para aplicar estilo a la selección
    static void ApplyStyleCB(Fl_Widget* w, void* data) {
        RichEditor* win = (RichEditor*)w->window();
        char style_char = (char)(unsigned long)data;
        
        // Obtener dónde está el cursor o la selección
        int start, end;
        if (!win->editor->buffer()->selection_position(&start, &end)) {
            // Si no hay selección, aplicar a todo (o manejar cursor actual)
            return; 
        }

        // Aplicar el caracter de estilo en el buffer paralelo
        // ¡Esto es O(N) en el rango seleccionado, muy rápido!
        for (int i = start; i < end; i++) {
            win->style_buff->replace(i, i+1, &style_char);
        }
        // Forzar redibujado
        win->editor->redisplay_range(start, end);
    }
};

int main() {
    RichEditor win(800, 600, "TinyWord - Math & CS");
    win.show();
    return Fl::run();
}
