#include <stdio.h>
#include <gtk/gtk.h>
#include "memory.c"
#include "processor.c"
#include "gpu.c"
#include "input.c"

void startEmulation();
static void print_hello(GtkWidget*, gpointer);
static void activate(GtkApplication*, gpointer);
int createGTKApp(int, char**);
void createImage();
void consoleStart();
void consoleEnd();


int main (int argc, char **argv) {
	consoleStart();

	struct memory m;
	printf("Memory found at: 0x%x\n", &m);
	printf("Memory size is: %d (int bytes) and 0x%x (hex bytes)\n", sizeof(m), sizeof(m));

	int status = createGTKApp(argc, argv);

	createImage();

	consoleEnd();

	return status;
}

void startEmulation(){
	while(true){
		executeCPU(5);
		//generateInterrupts();
		//emulateGraphics();
		//emulateSound();
		//emulateOtherSoftware();
		//timeSynchronization();
	}
}

static void print_hello(GtkWidget *widget, gpointer data) {
	g_print ("Starting emulation...\n");
}

static void activate(GtkApplication* app, gpointer user_data) {
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *button_box;

	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "TinyGBoyC");
	gtk_window_set_default_size (GTK_WINDOW (window), HORIZONTAL_RESOLUTION*4, VERTICAL_RESOLUTION*4);

	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add (GTK_CONTAINER (window), button_box);

	button = gtk_button_new_with_label ("Start emulation...");
	g_signal_connect (button, "clicked", G_CALLBACK (startEmulation), NULL);
	g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	gtk_container_add (GTK_CONTAINER (button_box), button);

	gtk_widget_show_all (window);
}

int createGTKApp(int argc, char **argv){
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}

void createImage(){
	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 80);
	cairo_t *cr = cairo_create (surface);

	cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 32.0);
	cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
	cairo_move_to (cr, 10.0, 50.0);
	cairo_show_text (cr, "Hello, world");

	cairo_destroy (cr);
	cairo_surface_write_to_png (surface, "hello.png");
	cairo_surface_destroy (surface);
}

void consoleStart(){
	printf("\n");
	printf("###########################################################\n");
	printf("###################### TinyGBoyC ##########################\n");
	printf("###########################################################\n");
	printf("\n");
}

void consoleEnd(){
	printf("\n");
	printf("\n");
	printf("\n");
}