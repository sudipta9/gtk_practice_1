#include <gtk/gtk.h>
GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *view1;
GtkWidget *tree1;
GtkWidget *search;
GtkTreeStore *treeStore;
GtkTreeView *tv1;
GtkTreeViewColumn *cx1;
GtkTreeViewColumn *cx2;
GtkTreeSelection *selection;
GtkCellRenderer *cr1;
GtkCellRenderer *cr2;
GtkBuilder *builder;
GtkAdjustment *adjustment1, *adjustment2;

int findRow(GtkTreeModel *model, GtkTreeIter iter, const gchar *srch);
void on_destroy();
int rowNbr, rowMax;

int main(int argc, char *argv[])
{

	gtk_init(&argc, &argv); // init Gtk

	builder = gtk_builder_new_from_file("../glade/part4_tree.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

	gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	view1 = GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
	search = GTK_WIDGET(gtk_builder_get_object(builder, "search"));
	treeStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treeStore"));
	tv1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tv1"));
	cx1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx1"));
	cx2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx2"));
	cr1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr1"));
	cr2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr2"));
	adjustment1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment1"));
	adjustment2 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment2"));
	selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "selection"));

	gtk_tree_view_column_add_attribute(cx1, cr1, "text", 0); // attach the renderer to the column
	gtk_tree_view_column_add_attribute(cx2, cr2, "text", 1); // attach the renderer to the column

	GtkTreeIter iter;		// iterators
	GtkTreeIter iterChild1; // iterators
	GtkTreeIter iterChild2; // iterators
	GtkTreeIter iterChild3; // iterators
	GtkTreeIter iterChild4; // iterators
	GtkTreeIter iterChild5; // iterators

	char line[1024], desc[1024], code[1024], *p1, *p2;
	int level;
	FILE *f1 = fopen("../files/mesh1.txt", "r");

	if (f1 == NULL)
	{
		printf("Missing file\n");
		return EXIT_FAILURE;
	}

	rowNbr = 0;

	while (1)
	{
		if (fgets(line, 1024, f1) == NULL)
		{
			fclose(f1);
			break;
		}

		rowNbr++; // count

		line[strlen(line) - 1] = 0; // remove newline char

		p1 = strstr(line, ";");
		*p1 = 0;
		strcpy(desc, line);
		p1++;
		strcpy(code, p1);

		level = 0;

		for (int i = 0; code[i] != 0; i++)
			if (code[i] == '.')
				level++;

		level++;

		printf("level = %d desc = %s; code = %s\n", level, desc, code);

		if (level == 1)
		{
			gtk_tree_store_append(treeStore, &iter, NULL);
			gtk_tree_store_set(treeStore, &iter, 0, desc, -1);
			gtk_tree_store_set(treeStore, &iter, 1, code, -1);
		}

		if (level == 2)
		{
			gtk_tree_store_append(treeStore, &iterChild1, &iter);
			gtk_tree_store_set(treeStore, &iterChild1, 0, desc, -1);
			gtk_tree_store_set(treeStore, &iterChild1, 1, code, -1);
		}

		if (level == 3)
		{
			gtk_tree_store_append(treeStore, &iterChild2, &iterChild1);
			gtk_tree_store_set(treeStore, &iterChild2, 0, desc, -1);
			gtk_tree_store_set(treeStore, &iterChild2, 1, code, -1);
		}

		if (level == 4)
		{
			gtk_tree_store_append(treeStore, &iterChild3, &iterChild2);
			gtk_tree_store_set(treeStore, &iterChild3, 0, desc, -1);
			gtk_tree_store_set(treeStore, &iterChild3, 1, code, -1);
		}

		if (level == 5)
		{
			gtk_tree_store_append(treeStore, &iterChild4, &iterChild3);
			gtk_tree_store_set(treeStore, &iterChild4, 0, desc, -1);
			gtk_tree_store_set(treeStore, &iterChild4, 1, code, -1);
		}

		if (level == 6)
		{
			gtk_tree_store_append(treeStore, &iterChild5, &iterChild4);
			gtk_tree_store_set(treeStore, &iterChild5, 0, desc, -1);
			gtk_tree_store_set(treeStore, &iterChild5, 1, code, -1);
		}
	}

	printf("%d rows\n", rowNbr);

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
	printf("Description:  %s; ", value);
	gtk_tree_model_get(model, &iter, 1, &value, -1);
	printf("MeSH Code:  %s\n", value);
}

void on_destroy()
{
	gtk_main_quit();
}

void on_search_search_changed(GtkSearchEntry *s)
{
	const gchar *srch;
	GtkTreeIter iter;

	GtkTreeModel *model = gtk_tree_view_get_model(tv1); // get the tree model

	srch = gtk_entry_get_text(GTK_ENTRY(s));

	if (strlen(srch) == 0)
		return;

	printf("\nSearch for: %s\n", srch);

	gtk_tree_model_get_iter_first(model, &iter);

	rowNbr = 0;
	findRow(model, iter, srch);
}

int findRow(GtkTreeModel *model, GtkTreeIter iter, const gchar *srch)
{

	const gchar *text;
	GtkTreeIter iterChild;
	GtkTreePath *path;

	while (1)
	{

		gtk_tree_model_get(model, &iter, 0, &text, -1); // get ptr to col 0 text
		rowNbr++;										// row number

		if (strncasecmp(text, srch, strlen(srch)) == 0)
		{ // case insensitive search
			printf("Found: %d %s\n", rowNbr, text);
			path = gtk_tree_model_get_path(model, &iter);
			gtk_tree_view_expand_all(tv1);

			gtk_tree_view_scroll_to_cell(tv1, path, NULL, TRUE, 0.5, 0.0);

			//			gtk_tree_view_set_drag_dest_row
			//				(tv1, path, GTK_TREE_VIEW_DROP_AFTER); // alt highlight row

			gtk_tree_view_set_cursor_on_cell(tv1, path, NULL, NULL, FALSE); // highlight
			return 1;
		}

		if (gtk_tree_model_iter_has_child(model, &iter))
		{ // children?
			gtk_tree_model_iter_children(model, &iterChild, &iter);
			if (findRow(model, iterChild, srch))
				return 1;
		}

		if (!gtk_tree_model_iter_next(model, &iter))
		{
			return 0;
		}
	}
}

// void	on_tv1_start_interactive_search(GtkTreeView *t) {
// 	printf("interactive search\n");
// 	gtk_tree_view_expand_all (t);
// 	}

gboolean on_tv1_key_press_event(GtkTreeView *t)
{
	printf("key press event\n");
	gtk_tree_view_expand_all(t);
	return FALSE;
}
