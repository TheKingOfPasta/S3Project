#include "gui.h"
#include "../img_processing/gaussian_blur.h"
#include "../img_processing/adaptive_thresholding.h"
#include "../img_processing/rotate.h"
#include "../img_processing/canny_edge_detector.h"
#include "../img_processing/img_color.h"
#include "../img_processing/invert_colors.h"
#include "../hough/utils.h"
#include "../hough/hough.h"
#include "../hough/visualization.h"
#include "../hough/square_detection.h"
#include "../img_processing/erosion.h"
#include "../hough/grid_detection.h"

#define Blursize 13
#define BlurIntensity 3
#define AdaptiveThreshold 3
#define Splitsize 75000

typedef struct widgets
{
    GtkWidget* w1;
    GtkWidget* w2;

    GtkImage* ImagePicking;
    GtkImage* ImageDisplay;

    GtkFileChooser* FilePicker;

    GtkButton* NextPageButton;
    char* path;
} widgets;

int i = 0;

gboolean DoNextFunc(GtkButton* btn, gpointer ptr)
{
    GtkImage* ImageDisplay = ptr;

    SDL_Surface* img = IMG_Load("temp.png");

    switch (i)
    {
        case 0:
            img = IMGC_surface_to_grayscale(img);
            printf("Applied grayscaling\n");
            break;
        case 1:
            img = IMGA_GaussianBlur(img, Blursize, BlurIntensity);
            printf("Applied gaussian blur\n");
            break;
        case 2:
            img = IMGA_ApplyThreshold(img, AdaptiveThreshold, Splitsize);
            printf("Applied thresholding\n");
            break;
        case 3:
            img = CheckInvert(img);
            printf("Inverted colors if necessary\n");
            break;
        case 4:
            img = IMGA_Erosion(img);
            printf("Applied erosion\n");
            break;
    }
    i += 1;

    IMG_SavePNG(img, "temp.png");
    printf("Saved\n");

    gtk_image_set_from_file(ImageDisplay, "temp.png");

    if (i == 5)//TODO put the actual number of functions we use
        gtk_widget_hide(GTK_WIDGET(btn));


    return TRUE;
}

gboolean DoAllFunc(GtkButton* btn, gpointer ptr)
{
    for (; i < 5;)
    {
        DoNextFunc(btn, ptr);
    }

    printf("All has been applied to the image\n");

    return TRUE;
}

gboolean ChangeWindow(GtkButton* btn, gpointer ptr)
{
    *btn = *btn;//Can't remove the btn param but can't leave a warning

    widgets* h = ptr;

    gtk_widget_hide(GTK_WIDGET(h->w1));
    gtk_widget_show(GTK_WIDGET(h->w2));

    gtk_image_set_from_file(h->ImageDisplay, h->path);

    IMG_SavePNG(IMG_Load(h->path), "temp.png");

    return FALSE;
}

gboolean ShowImage(GtkFileChooser* file_picker, gpointer ptr)
{
    widgets* h = ptr;

    char* path = gtk_file_chooser_get_filename(file_picker);

    h->path = path;

    gtk_image_set_from_file(h->ImagePicking, path);
    gtk_widget_show(GTK_WIDGET(h->NextPageButton));

    return TRUE;
}

gpointer MyQuit()
{
    if (i)//If you applied any function (a temp file was created => remove it)
        remove("temp.png");

    gtk_main_quit();

    return FALSE;
}

// Main function.
int main ()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return EXIT_FAILURE;
    }

    // Gets the widgets.
    widgets w =
    {
        w1 : GTK_WIDGET(gtk_builder_get_object(builder, "StartWindow")),
        w2 : GTK_WIDGET(gtk_builder_get_object(builder, "gui")),
        ImagePicking : GTK_IMAGE(gtk_builder_get_object(builder, "ImagePicking")),
        ImageDisplay : GTK_IMAGE(gtk_builder_get_object(builder, "ImageDisplay")),
        FilePicker : GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "FilePicker")),
        NextPageButton : GTK_BUTTON(gtk_builder_get_object(builder, "ApplyOnImage")),
        path : NULL
    };

    gtk_widget_hide(GTK_WIDGET(w.NextPageButton));

    GtkButton* ApplyOnImage = GTK_BUTTON(gtk_builder_get_object(builder, "ApplyOnImage"));

    g_signal_connect(ApplyOnImage, "clicked", G_CALLBACK(ChangeWindow), &w);

    GtkButton* DoNext = GTK_BUTTON(gtk_builder_get_object(builder, "DoNext"));
    GtkButton* DoAll = GTK_BUTTON(gtk_builder_get_object(builder, "DoAll"));

    // Connects signal handlers.
    g_signal_connect(w.w1, "destroy", G_CALLBACK(MyQuit), NULL);
    g_signal_connect(w.w2, "destroy", G_CALLBACK(MyQuit), NULL);
    g_signal_connect(w.FilePicker, "file-set", G_CALLBACK(ShowImage), &w);

    g_signal_connect(DoNext, "clicked", G_CALLBACK(DoNextFunc), w.ImageDisplay);
    g_signal_connect(DoAll, "clicked", G_CALLBACK(DoAllFunc), w.ImageDisplay);

    gtk_main();

    return EXIT_SUCCESS;
}