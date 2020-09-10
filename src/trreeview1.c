#include <gtk/gtk.h>

GtkWidget *main_window;
GtkFixed *fixed;
// GtkWidget *view1;
GtkTreeStore *treestore;
GtkScrolledWindow *view1;
GtkTreeView *tv1;
GtkViewport *view2;
GtkTreeSelection *select;
GtkTreeViewColumn *cx1;
GtkTreeViewColumn *cx2;
GtkCellRendererText *cr1;
GtkCellRendererText *cr2;

// window closing function
void on_main_window_destroy();
// {
// gtk_main_quit();
// }

// main window
int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv); // init GTK
    builder = gtk_builder_new_from_file("../glade/main_window3.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    g_signal_connect(window, "distroy", G_CALLBACK(on_main_window_destroy), NULL);

    fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
    view1 = GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
    treestore = GTK_WIDGET(gtk_builder_get_object(builder, "treestore"));
    tv1 = GTK_WIDGET(gtk_builder_get_object(builder, "tv1"));
    cx1 = GTK_WIDGET(gtk_builder_get_object(builder, "cx1"));
    cx2 = GTK_WIDGET(gtk_builder_get_object(builder, "cx2"));
    cr1 = GTK_WIDGET(gtk_builder_get_object(builder, "cr1"));
    cr2 = GTK_WIDGET(gtk_builder_get_object(builder, "cr2"));
    select = GTK_WIDGET(gtk_builder_get_object(builder, "select"));

    // gtk add tree attribute
    gtk_tree_view_column_add_attribute(cx1, cr1, "text", 0);
    gtk_tree_view_column_add_attribute(cx2, cr2, "text", 1);

    GtkTreeIter iter;
    GtkTreeIter iterChild1;
    GtkTreeIter iterChild2;
    GtkTreeIter iterChild3;
    GtkTreeIter iterChild4;
    GtkTreeIter iterChild5;

    char line[1024], desc[1024], code[1024], *p1, *p2;
    int level;
    FILE *f1 = fopen("../files/mesh.txt", "r");

    if (f1 == NULL)
    {
        printf("missing File");
        return EXIT_FAILURE;
    }

    while (TRUE)
    {
        if (fgets(line, 1024, f1) == NULL)
        {
            fclose(f1);
            break;
        }

        line[strlen(line) - 1] = 0; //removeing the new line

        p1 = strstr(line, ";");
        *p1 = 0;
        strcpy(desc, line);
        p1++;
        strcpy(code, p1);

        level = 0;
        for (int i = 0; code[i] != 0; i++)
        {
            if (code[i] == ".")
            {
                level++;
            }
        }
        level++;
        printf("level = %d word = %s; code = %s\n", level, desc, code);

        if (level == 1)
        {
            gtk_tree_store_append(treestore, &iter, NULL);
            gtk_tree_store_set(treestore, &iter, 0, desc, -1);
            gtk_tree_store_set(treestore, &iter, 1, code, -1);
        }

        if (level == 2)
        {
            gtk_tree_store_append(treestore, &iterChild1, &iter);
            gtk_tree_store_set(treestore, &iterChild1, 0, desc, -1);
            gtk_tree_store_set(treestore, &iterChild1, 1, code, -1);
        }
        if (level == 3)
        {
            gtk_tree_store_append(treestore, &iterChild2, &iterChild1);
            gtk_tree_store_set(treestore, &iterChild2, 0, desc, -1);
            gtk_tree_store_set(treestore, &iterChild2, 1, code, -1);
        }
        if (level == 4)
        {
            gtk_tree_store_append(treestore, &iterChild3, &iterChild2);
            gtk_tree_store_set(treestore, &iterChild3, 0, desc, -1);
            gtk_tree_store_set(treestore, &iterChild3, 1, code, -1);
        }
        if (level == 5)
        {
            gtk_tree_store_append(treestore, &iterChild4, &iterChild3);
            gtk_tree_store_set(treestore, &iterChild4, 0, desc, -1);
            gtk_tree_store_set(treestore, &iterChild4, 1, code, -1);
        }
        if (level == 6)
        {
            gtk_tree_store_append(treestore, &iterChild5, &iterChild4);
            gtk_tree_store_set(treestore, &iterChild5, 0, desc, -1);
            gtk_tree_store_set(treestore, &iterChild5, 1, code, -1);
        }
    }

    // gtk_tree_view_set_model(GTK_TREE_VIEW(tv1), GTK_TREE_MODEL(treestore));

    select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv1));

    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}

void on_select_changed(GtkWidget *c)
{
    gchar *value;
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(c), &model, &iter) == FALSE)
        return;
    gtk_tree_model_get(model, &iter, 0, &value, -1);
    printf("Description %s", value);
    gtk_tree_model_get(model, &iter, 1, &value, -1);
    printf("Mesh Code %s\n", value);
}

void on_main_window_destroy()
{
    gtk_main_quit();
}