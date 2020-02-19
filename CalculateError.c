#include <gtk/gtk.h>
#include <math.h>
#define arraysize(x)  (sizeof(x) / sizeof((x)[0]))

//gcc -Wall -g CalculateError.c -o CalculateError `pkg-config --cflags --libs gtk+-3.0`

void packboxes();
void createwindow();
void create_entries();
void set_spacing(GtkWidget *widget, int colspace, int rowspace);
void createlabels(GtkWidget *label, gchar *labeltext[]);
void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns);
void button_connect_callback(GtkWidget *button, void *button_callback);
void calculate_btn();
void closeprogram();
void reverse();

// compile with gcc -Wall -g connect_btn.c -o connect_btn `pkg-config --cflags --libs gtk+-3.0`
struct widgets
{
	GtkWidget *window;
	GtkWidget *vbox; //widget for the vertical box
	GtkWidget *hbox;
	GtkWidget *buttonbox;
	GtkWidget *buttonhbox;
	GtkWidget *entrygrid;
	GtkWidget *grid;
	GtkWidget *label;
	GtkWidget *entries[2];
	GtkWidget *display;
	GtkTextBuffer *buffer;
	GtkWidget *outputlabel;
}gwidget;

struct variables
{
	const char *approxentry;
	const char *exactentry;
	size_t entrylen;
	size_t buttonlen;
}location;

int main(int argc, char *argv [])
{
gchar *labeltext[] = {"Approximate Value\n","Exact Value\n"};
gchar *buttonlabels[] = {"Calculate", "Reverse", "Close"};
void *buttoncallbacks[] = {calculate_btn, reverse, closeprogram};
location.entrylen = arraysize(labeltext);
location.buttonlen = arraysize(buttonlabels);
gtk_init(&argc, &argv); //starting gtk 

createwindow();

gwidget.buttonbox = gtk_grid_new(); 
createlabels(gwidget.label, labeltext);
createsinglesizegrid(gwidget.buttonbox, buttonlabels, buttoncallbacks, 1, location.buttonlen);
set_spacing(gwidget.buttonbox, 4, 4);
create_entries(location.entrylen);
gwidget.display = gtk_text_view_new (); //sets the display widget as a text display 
gwidget.buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW ( gwidget.display)); //sets the gwidget.buffer widget to the text gwidget.buffer of display
gtk_widget_set_size_request( gwidget.display, 150, 25); //sets the size of the display 
gwidget.outputlabel = gtk_label_new("Percent Error");

packboxes();

g_signal_connect(G_OBJECT(gwidget.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

gtk_widget_show_all(gwidget.window); //shows all widgets 

gtk_main();//gtk main, this is the main loop of GTK
}

void createwindow()
{
	gwidget.window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //creates toplevel window
	gtk_window_set_title(GTK_WINDOW(gwidget.window), "Calculate Error"); //sets a window title 
	gtk_window_set_position(GTK_WINDOW(gwidget.window), GTK_WIN_POS_CENTER); //opens the window in the center of the screen
	gtk_container_set_border_width(GTK_CONTAINER(gwidget.window), 5); //sets the border size of the window
}

void packboxes()
{
	gwidget.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	gwidget.hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gwidget.buttonhbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gtk_container_add(GTK_CONTAINER(gwidget.window), gwidget.vbox); //adds the vbox to the window 
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.hbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.outputlabel, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.grid, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.entrygrid, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.display, FALSE, FALSE, 0); //packs the display into the vbox
}

void create_entries(int arraylen)
{
	gwidget.entrygrid = gtk_grid_new();
	for(int i = 0; i < arraylen; i++)
	{
	gwidget.entries[i]= gtk_entry_new();
	gtk_grid_attach(GTK_GRID(gwidget.entrygrid), gwidget.entries[i], 0, i, 1, 1); //sets the defaults for creating each table button
	}
}

void createlabels(GtkWidget *label, gchar *labeltext[])
{
	gwidget.grid = gtk_grid_new();

for (int i=0; i<location.entrylen; i++)
{
	for(int j = 0; j<1; j++)
	{	
		label = gtk_label_new(labeltext[i]);
		gtk_grid_attach(GTK_GRID(gwidget.grid), label, j, i, 1, 1); //sets the defaults for creating each table button
	}
}
}

void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns)
{
int pos = 0;
GtkWidget *button; //widget for the buttons 
	for (int i=0; i < rows; i++) //for loop for the rows
	{
		for (int j=0; j < columns; j++) //for loop for the columns
		{
		button = gtk_button_new_with_label(labels[pos]); //sets each button label to the respective button 
		button_connect_callback(button, callback[pos]); //attaches the button to the respective callback
		gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1); //sets the defaults for creating each table button
		gtk_widget_set_size_request(button, 70, 30); //sets the size of the buttons
		pos++; //changes the position 
		}
	}
}

void button_connect_callback(GtkWidget *button, void *button_callback)
{
	g_signal_connect(button, "clicked", G_CALLBACK(button_callback), NULL);
}

void set_spacing(GtkWidget *widget, int colspace, int rowspace)
{
	gtk_grid_set_column_spacing(GTK_GRID(widget), colspace);
	gtk_grid_set_row_spacing(GTK_GRID(widget), rowspace);
}

void calculate_btn()
{
float floatanswer;
location.exactentry = gtk_entry_get_text (GTK_ENTRY (gwidget.entries[1]));
float exact = atof(location.exactentry);
location.approxentry = gtk_entry_get_text (GTK_ENTRY (gwidget.entries[0]));
float approximate=atof(location.approxentry);
floatanswer=fabs(approximate - exact)/exact*100;
g_print("Percent error is %f", floatanswer);
char results[16];
sprintf(results,"%f", floatanswer); //copies input.num1 to the preview 
gtk_text_buffer_set_text ( gwidget.buffer, results, -1); //displays input.num1 

}

void closeprogram()
{
exit(1);
}

void reverse()
{
float floatanswer;
location.exactentry = gtk_entry_get_text (GTK_ENTRY (gwidget.entries[1]));
float exact = atof(location.exactentry);
location.approxentry = gtk_entry_get_text (GTK_ENTRY (gwidget.entries[0]));
float approximate=atof(location.approxentry);
floatanswer=fabs(exact - approximate)/approximate*100;
g_print("Percent error is %f", floatanswer);
char results[16];
sprintf(results,"%f", floatanswer); //copies input.num1 to the preview 
gtk_text_buffer_set_text ( gwidget.buffer, results, -1); //displays input.num1 
}