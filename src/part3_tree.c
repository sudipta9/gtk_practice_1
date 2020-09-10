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

	builder = gtk_builder_new_from_file("../glade/part3_tree.glade");

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

	gtk_tree_view_column_add_attribute(cx1, cr1, "text", 0); // attach renderer to column

	gtk_tree_view_column_add_attribute(cx2, cr2,
									   "active", 1); // attach renderer to column & make box alterable

	GtkTreeIter iter;		// iterators
	GtkTreeIter iterChild1; // iterators
	GtkTreeIter iterChild2; // iterators

	//	parent 1
	//		child 1
	//			grandchild 1
	//			grandchild 2
	//		child 2
	//			grandchild 1
	//			grandchild 2
	//	parent 2
	//	parent 3

	//	note the displayed paths for the children in the running example

	//	row 1 -------------

	gtk_tree_store_append(treeStore, &iter, NULL);
	gtk_tree_store_set(treeStore, &iter, 0, "row 1", -1);
	gtk_tree_store_set(treeStore, &iter, 1, TRUE, -1); // initially checked

	//	row 1 child 1 -----

	gtk_tree_store_append(treeStore, &iterChild1, &iter); // child row
	gtk_tree_store_set(treeStore, &iterChild1, 0, "child1 of row 1", -1);
	gtk_tree_store_set(treeStore, &iterChild1, 1, TRUE, -1); // initially checked

	//	row 1 child 1 grandchild 1

	gtk_tree_store_append(treeStore, &iterChild2, &iterChild1); // grand child row
	gtk_tree_store_set(treeStore, &iterChild2, 0, "grandchild 1 of row 1 child 1", -1);
	gtk_tree_store_set(treeStore, &iterChild2, 1, TRUE, -1); // initially checked

	//	row 1 child 1 grandchild 2

	gtk_tree_store_append(treeStore, &iterChild2, &iterChild1); // grand child row
	gtk_tree_store_set(treeStore, &iterChild2, 0, "grandchild 2 of row 1 child 1", -1);
	gtk_tree_store_set(treeStore, &iterChild2, 1, TRUE, -1); // initially checked

	//	row 1 child 2 -----

	gtk_tree_store_append(treeStore, &iterChild1, &iter); // child row
	gtk_tree_store_set(treeStore, &iterChild1, 0, "child 2 of row 1", -1);
	gtk_tree_store_set(treeStore, &iterChild1, 1, TRUE, -1); // initially checked

	//	row 1 child 2 grandchild 1

	gtk_tree_store_append(treeStore, &iterChild2, &iterChild1); // grand child row
	gtk_tree_store_set(treeStore, &iterChild2, 0, "grandchild 1 of row 1 child 2", -1);
	gtk_tree_store_set(treeStore, &iterChild2, 1, TRUE, -1); // initially checked

	//	row 1 child 2 grandchild 2

	gtk_tree_store_append(treeStore, &iterChild2, &iterChild1); // grand child row
	gtk_tree_store_set(treeStore, &iterChild2, 0, "grandchild 2 of row 1 child 2", -1);
	gtk_tree_store_set(treeStore, &iterChild2, 1, TRUE, -1); // initially checked

	//	row 2 -------------

	gtk_tree_store_append(treeStore, &iter, NULL);
	gtk_tree_store_set(treeStore, &iter, 0, "row 2", -1);
	gtk_tree_store_set(treeStore, &iter, 1, TRUE, -1);

	//	row 3 -------------

	gtk_tree_store_append(treeStore, &iter, NULL);
	gtk_tree_store_set(treeStore, &iter, 0, "row 3", -1);
	gtk_tree_store_set(treeStore, &iter, 1, TRUE, -1);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv1));

	gtk_widget_show_all(window);

	gtk_main();
	return EXIT_SUCCESS;
}

void on_select_changed(GtkTreeSelection *c)
{

	gchar *value;
	gboolean box;
	GtkTreeIter iter, tmp;
	GtkTreeModel *model;

	//	Function 'get_tree_selection_get_selected' sets the model
	//	and iter from the selection c

	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(c), &model, &iter) == FALSE)
		return;

	gtk_tree_model_get(model, &iter, 0, &value, -1); // get column 0
	printf("Select signal received: col 0 = \"%s\"; ", value);

	gtk_tree_model_get(model, &iter, 1, &box, -1); // get column 1
	printf("col 1  = \"%d\"\n", box);
}

void on_cr2_toggled(GtkCellRendererToggle *cell, gchar *path_string)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gboolean t = FALSE;
	gchar *text;

	printf("---------------------------------------\n");

	printf("box toggle signal received: path = \"%s\"\n", path_string); // path gives row and child data

	model = gtk_tree_view_get_model(tv1); // get the tree model

	gtk_tree_model_get_iter_from_string(model, &iter, path_string); // get iter from path

	gtk_tree_model_get(model, &iter, 0, &text, -1); // get the text pointer of col 0

	printf("For row text  = \"%s\"\n", text);

	gtk_tree_model_get(model, &iter, 1, &t, -1); // get the boolean value of col 1

	if (t == FALSE)
		t = TRUE;
	else
		t = FALSE; // toggle

	gtk_tree_store_set(treeStore, &iter, 1, t, -1); // alter col 1 check box

	printf("---------------------------------------\n");

	return;
}

void on_destroy()
{
	gtk_main_quit();
}
