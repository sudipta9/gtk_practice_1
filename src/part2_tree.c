#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/mman.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *view1;
GtkWidget *tree1;
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

	builder = gtk_builder_new_from_file("../glade/part2_tree.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

	gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	view1 = GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
	treeStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treeStore"));
	tv1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tv1"));
	cx1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx1"));
	cx2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx2"));
	cr1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr1"));
	cr2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr2"));
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

	while (1)
	{
		if (fgets(line, 1024, f1) == NULL)
		{
			fclose(f1);
			break;
		}

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

	// gtk_tree_view_set_model(GTK_TREE_VIEW(tv1), GTK_TREE_MODEL(treeStore));

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
	printf("Description:  %s; ", value);
	gtk_tree_model_get(model, &iter, 1, &value, -1);
	printf("MeSH Code:  %s\n", value);
}

void on_destroy()
{
	gtk_main_quit();
}
