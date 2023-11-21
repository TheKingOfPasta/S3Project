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
    GtkButton* DoAllButton;
    GtkButton* DoNextButton;
    GtkProgressBar* ProgressBar;
    char* path;
} widgets;

int i = 0;

gboolean DoNextFunc(GtkButton* btn, gpointer ptr)
{
    widgets* h = ptr;
    GtkImage* ImageDisplay = h->ImageDisplay;

    SDL_Surface* img = IMG_Load("temp.png");

    switch (i)
    {
        case 0:
            img = IMGC_surface_to_grayscale(img);
            gtk_button_set_label(btn, "Next step (Gaussian blur)");
            break;
        case 1:
            img = IMGA_GaussianBlur(img, Blursize, BlurIntensity);
            gtk_button_set_label(btn, "Next step (Adaptive thresholding)");
            break;
        case 2:
            img = IMGA_ApplyThreshold(img, AdaptiveThreshold, Splitsize);
            gtk_button_set_label(btn, "Next step (Invert if necessary)");
            break;
        case 3:
            img = CheckInvert(img);
            gtk_button_set_label(btn, "Next step (Erode)");
            break;
        case 4:
            img = IMGA_Erosion(img);
            gtk_widget_hide(GTK_WIDGET(h->DoNextButton));
            gtk_widget_hide(GTK_WIDGET(h->DoAllButton));
            break;
    }
    i += 1;

    gtk_progress_bar_set_fraction(h->ProgressBar, (float)i / 5);

    IMG_SavePNG(img, "temp.png");

    gtk_image_set_from_file(ImageDisplay, "temp.png");

    return TRUE;
}

gboolean DoAllFunc(GtkButton* btn, gpointer ptr)
{
    btn = btn;
    widgets* h = ptr;

    for (; i < 5;)
    {
        DoNextFunc(h->NextPageButton, h);
    }

    return TRUE;
}

gboolean ChangeWindow(GtkButton* btn, gpointer ptr)
{
    *btn = *btn;//Can't remove the btn param but can't leave a warning

    widgets* h = ptr;

    i = 0;

//Hide many things and show many things
    gtk_widget_hide(GTK_WIDGET(h->w1));
    gtk_widget_show(GTK_WIDGET(h->w2));

    gtk_image_set_from_file(h->ImageDisplay, h->path);

    gtk_button_set_label(h->DoNextButton, "Next step (Grayscale)");

    gtk_widget_show(GTK_WIDGET(h->DoNextButton));
    gtk_widget_show(GTK_WIDGET(h->DoAllButton));

    IMG_SavePNG(IMG_Load(h->path), "temp.png");

    gtk_progress_bar_set_fraction(h->ProgressBar, 0);

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

gboolean Save()
{
    IMG_SavePNG(IMG_Load("temp.png"), "output.png");

    printf("Saved into output.png\n");

    return FALSE;
}

gboolean GoBack(GtkButton* btn, gpointer ptr)
{
    btn = btn;

    widgets* h = ptr;

    if (i)//If you applied any function (a temp file was created => remove it)
        remove("temp.png");

    gtk_widget_show(GTK_WIDGET(h->w1));
    gtk_widget_hide(GTK_WIDGET(h->w2));

    gtk_image_set_from_file(h->ImagePicking, "");//Reset the image

    gtk_file_chooser_set_filename(h->FilePicker, "");

    gtk_widget_hide(GTK_WIDGET(h->NextPageButton));

    return FALSE;
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
        DoAllButton : GTK_BUTTON(gtk_builder_get_object(builder, "DoAll")),
        DoNextButton : GTK_BUTTON(gtk_builder_get_object(builder, "DoNext")),
        ProgressBar : GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "ProgressBar")),
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

    g_signal_connect(
        GTK_BUTTON(gtk_builder_get_object(builder, "CancelButton")),
        "clicked",
        G_CALLBACK(GoBack),
        &w
        );

    g_signal_connect(
        GTK_BUTTON(gtk_builder_get_object(builder, "SaveButton")),
        "clicked",
        G_CALLBACK(Save),
        &w
        );

    g_signal_connect(DoNext, "clicked", G_CALLBACK(DoNextFunc), &w);
    g_signal_connect(DoAll, "clicked", G_CALLBACK(DoAllFunc), &w);

    gtk_main();

    return EXIT_SUCCESS;
}