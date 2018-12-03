#include <gtk/gtk.h>

static void
print_hello (GtkWidget *widget,
     gpointer   data)
{
  g_print ("Hello World\n");
}

int main (int argc, char *argv[])
{
  GtkWidget *window;
	GtkWidget *grid;
  GtkWidget *grid_top_right;
	GtkWidget *grid_left_bottom;
  GtkWidget *button;
	GtkWidget *align;

	int windowWidth = 1000, windowHeight = 800;

  /* This is called in all GTK applications. Arguments are parsed
   * from the command line and are returned to the application.
   */
  gtk_init (&argc, &argv);

  /* create a new window, and set its title */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Grid");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_default_size(GTK_WINDOW(window), windowWidth,windowHeight);

  /* Here we construct the container that is going pack our buttons */
  grid_top_right = gtk_grid_new ();
	gtk_grid_set_column_spacing  (GTK_GRID (grid_top_right),5);
	gtk_grid_set_row_spacing (GTK_GRID (grid_top_right),5);

	grid_left_bottom = gtk_grid_new ();
	gtk_grid_set_column_spacing  (GTK_GRID (grid_left_bottom),5);
	gtk_grid_set_row_spacing (GTK_GRID (grid_left_bottom),5);

	grid = gtk_grid_new ();
	gtk_grid_set_column_spacing  (GTK_GRID (grid),5);
	gtk_grid_set_row_spacing (GTK_GRID (grid),5);

  /* Pack the container in the window */
  //gtk_container_add (GTK_CONTAINER (window), grid);

  button = gtk_button_new_with_label ("Spider");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid_top_right), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label ("Dump");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid_top_right), button, 5, 0, 1, 1);

  button = gtk_button_new_with_label ("Quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach (GTK_GRID (grid_left_bottom), button, 0, 1, 2, 1);

	// align = gtk_alignment_new(1, 0, 0, 0);
	// gtk_container_add(GTK_CONTAINER(align), grid_top_right);
	// 
	// gtk_container_add(GTK_CONTAINER(grid), align);
	//
	// align = gtk_alignment_new(0, 1, 0, 0);
	// gtk_container_add(GTK_CONTAINER(align), grid_left_bottom);
	//
	// gtk_container_add(GTK_CONTAINER(grid), align);

	gtk_container_add(GTK_CONTAINER(window), grid);

  /* Now that we are done packing our widgets, we show them all
   * in one go, by calling gtk_widget_show_all() on the window.
   * This call recursively calls gtk_widget_show() on all widgets
   * that are contained in the window, directly or indirectly.
   */
  gtk_widget_show_all (window);

  /* All GTK applications must have a gtk_main(). Control ends here
   * and waits for an event to occur (like a key press or a mouse event),
   * until gtk_main_quit() is called.
   */
  gtk_main ();

  return 0;
}
