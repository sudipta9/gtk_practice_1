#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *view1;
GtkTreeStore *treeStore;
GtkTreeView *tv1;
GtkTreeViewColumn *cx1;
GtkTreeViewColumn *cx2;
GtkTreeSelection *selection;
GtkCellRenderer *cr1;
GtkCellRenderer *cr2;
GtkBuilder *builder;

void on_destroy();

int main(int argc, char *argv[])
{

	gtk_init(&argc, &argv); // init Gtk

	builder = gtk_builder_new_from_file("../glade/part1_tree.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

	gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	view1 = GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
	treeStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treeStore"));
	tv1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tv1"));
	cx1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx1"));			  // col 1
	cx2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx2"));			  // col 2
	cr1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr1"));			  // col 1 renderer
	cr2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr2"));			  // col 2 renderer
	selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "selection")); // tree view selection

	gtk_tree_view_column_add_attribute(cx1, cr1, "text", 0); // attach the renderer to the column
	gtk_tree_view_column_add_attribute(cx2, cr2, "text", 1); // attach the renderer to the column

	GtkTreeIter iter;		// iterators
	GtkTreeIter iterChild1; // iterators
	GtkTreeIter iterChild2; // iterators

	gtk_tree_store_append(treeStore, &iter, NULL);
	gtk_tree_store_set(treeStore, &iter, 0, "row 1", -1);
	gtk_tree_store_set(treeStore, &iter, 1, "row 1 data", -1);

	gtk_tree_store_append(treeStore, &iterChild1, &iter);
	gtk_tree_store_set(treeStore, &iterChild1, 0, "row 1 child", -1);
	gtk_tree_store_set(treeStore, &iterChild1, 1, "row 1 child data", -1);

	gtk_tree_store_append(treeStore, &iter, NULL);
	gtk_tree_store_set(treeStore, &iter, 0, "row 2", -1);
	gtk_tree_store_set(treeStore, &iter, 1, "row 2 data", -1);

	gtk_tree_store_append(treeStore, &iterChild1, &iter);
	gtk_tree_store_set(treeStore, &iterChild1, 0, "row 2 child", -1);
	gtk_tree_store_set(treeStore, &iterChild1, 1, "row 2 child data", -1);

	gtk_tree_store_append(treeStore, &iterChild2, &iterChild1);
	gtk_tree_store_set(treeStore, &iterChild2, 0, "row 2 child of child", -1);
	gtk_tree_store_set(treeStore, &iterChild2, 1, "row 2 child of child data", -1);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv1));

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
	printf("col 0 = %s; ", value);
	gtk_tree_model_get(model, &iter, 1, &value, -1);
	printf("col 1  = %s\n", value);
}

void on_destroy()
{
	gtk_main_quit();
}
