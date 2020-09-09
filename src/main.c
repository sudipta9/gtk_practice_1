#include<gtk/gtk.h>

GtkWidget *g_print_hello;
GtkWidget *g_count_hello;

int main(int argc, char *argv[]){
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("../glade/main_window2.glade");
    
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    g_print_hello = GTK_WIDGET(gtk_builder_get_object(builder, "print_hello"));
    g_count_hello = GTK_WIDGET(gtk_builder_get_object(builder, "count_hello"));

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();
    return 0;
}

// will be called when clicked on close button
void on_window_main_destroy(){
    gtk_main_quit();
}

//will called when button clicked
void on_btn_hello_clicked(){
    static unsigned int count = 0;
    char str_count[30] = {0};

    gtk_label_set_text(GTK_LABEL(g_print_hello), "Hello World!");
    count ++;
    sprintf(str_count, "%d", count);
    gtk_label_set_text(GTK_LABEL(g_count_hello), str_count);
}