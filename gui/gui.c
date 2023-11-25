#include "gui.h"

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
Quadrilateral* quad;//The quad used by FindAngle and returned by FindGrid
tuple coords[81];//The list of coordinates, filled in by Split
short digits[9][9];

// Writes 'digit' on the surface at (x, y)
void WriteDigit(SDL_Surface* s, int x, int y, int digit)
{
    
}

gboolean DoNextFunc(GtkButton* btn, gpointer ptr)
{
    widgets* h = ptr;
    GtkImage* ImageDisplay = h->ImageDisplay;

    char* file;
    if (asprintf(&file, "temp%02i.png", i) == -1)
        errx(EXIT_FAILURE, "asprintf failed");

    SDL_Surface* img = IMG_Load(file);

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
            img = CheckInvert(img);
            img = IMGA_Erosion(img);
            gtk_button_set_label(btn, "Next step (Sobel gradient)");
            break;
        case 3:
            img = sobel_gradient(img);
            gtk_button_set_label(btn, "Next step (Hough transform)");
            break;
        case 4:
            quad = Find_Grid(img);

            gtk_button_set_label(btn, "Next step (Rotation)");

            break;
        case 5:
            double angle = FindAngle(quad);
            img = IMGA_Rotate(img, angle);
            gtk_button_set_label(btn, "Next step (Splitting)");
            break;
        case 6:
            mkdir("temp_split", S_IRWXU);

            Split(img, "temp_split", coords);

            gtk_button_set_label(btn, "Next step (Digit recognition)");
            break;
        case 7:
            for (int j = 1; j <= 81; j++)
            {
                char* f;
                if (asprintf(&f, "temp_split/split_%02i.png", j) == -1)
                    errx(EXIT_FAILURE, "asprintf failed");


                //SDL_Surface* s = IMG_Load(f);
                short foundDigit = 0;

                // Do neural network stuff
                // knowing that s is your image
                // You need to fill foundDigit
                // @Ilan

                //Maybe -1 for no digit

                digits[j % 9][j / 9] = foundDigit;

                free(f);
            }

            gtk_button_set_label(btn, "Next step (Solving)");
            break;
        case 8:
            SLV_solve(digits);
            //digits is filled in place with values from the neural network

            for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
            {
                WriteDigit(img,
                           coords[j * 9 + k].x,
                           coords[j * 9 + k].y,
                           digits[i][j]);
            }

            gtk_widget_hide(GTK_WIDGET(h->DoNextButton));
            gtk_widget_hide(GTK_WIDGET(h->DoAllButton));
            break;
    }
    i += 1;

    char* newFile;
    if (asprintf(&newFile, "temp%02i.png", i) == -1)
        errx(EXIT_FAILURE, "asprintf failed");

    IMG_SavePNG(img, newFile);

    gtk_image_set_from_file(ImageDisplay, newFile);

    gtk_progress_bar_set_fraction(h->ProgressBar, (float)i / 9);

    free(newFile);
    free(file);

    return TRUE;
}

gboolean DoAllFunc(GtkButton* btn, gpointer ptr)
{
    btn = btn;
    widgets* h = ptr;

    for (; i < 9;)
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

    IMG_SavePNG(IMG_Load(h->path), "temp00.png");

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
    IMG_SavePNG(IMG_Load("temp00.png"), "output.png");

    printf("Saved into output.png\n");

    return FALSE;
}

gboolean Undo(GtkButton* btn, gpointer ptr)
{
    btn = btn;
    if (i == 0)
        return FALSE;

    i -= 1;
    widgets* h = ptr;

    gtk_widget_show(GTK_WIDGET(h->DoNextButton));
    gtk_widget_show(GTK_WIDGET(h->DoAllButton));

    if (i == 0)
    {
        //Can't go backwards to call DoNextFunc, so copy paste :/

        gtk_image_set_from_file(h->ImageDisplay, "temp00.png");

        gtk_button_set_label(h->DoNextButton, "Next step (Grayscale)");

        gtk_progress_bar_set_fraction(h->ProgressBar, 0);
    }
    else
    {
        i -= 1;
        DoNextFunc(h->DoNextButton, h);
        //DoNextFunc will handle the i increase, the image loading
        // and the button labels update
    }


    return FALSE;
}

void RemoveTemps()
{
    for (int j = 0; j < 9; j++)
    {
        char* removing;
        if (asprintf(&removing, "temp%02i.png", j) == -1)
            errx(EXIT_FAILURE, "asprintf failed");
        remove(removing);
        free(removing);
    }
    remove("temp_split/");
}

gboolean GoBack(GtkButton* btn, gpointer ptr)
{
    btn = btn;

    widgets* h = ptr;

    if (i)//If you applied any function (a temp file was created => remove it)
        RemoveTemps();

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
        RemoveTemps();

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
        w1 : GTK_WIDGET(
            gtk_builder_get_object(builder, "StartWindow")),

        w2 : GTK_WIDGET(
            gtk_builder_get_object(builder, "gui")),

        ImagePicking : GTK_IMAGE(
            gtk_builder_get_object(builder, "ImagePicking")),

        ImageDisplay : GTK_IMAGE(
            gtk_builder_get_object(builder, "ImageDisplay")),

        FilePicker : GTK_FILE_CHOOSER(
            gtk_builder_get_object(builder, "FilePicker")),

        NextPageButton : GTK_BUTTON(
            gtk_builder_get_object(builder, "ApplyOnImage")),

        DoAllButton : GTK_BUTTON(
            gtk_builder_get_object(builder, "DoAll")),

        DoNextButton : GTK_BUTTON(
            gtk_builder_get_object(builder, "DoNext")),

        ProgressBar : GTK_PROGRESS_BAR(
            gtk_builder_get_object(builder, "ProgressBar")),

        path : NULL
    };

    gtk_widget_hide(GTK_WIDGET(w.NextPageButton));

    GtkButton* ApplyOnImage =
        GTK_BUTTON(gtk_builder_get_object(builder, "ApplyOnImage"));

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

    g_signal_connect(
        GTK_BUTTON(gtk_builder_get_object(builder, "Undo")),
        "clicked",
        G_CALLBACK(Undo),
        &w
        );

    g_signal_connect(DoNext, "clicked", G_CALLBACK(DoNextFunc), &w);
    g_signal_connect(DoAll, "clicked", G_CALLBACK(DoAllFunc), &w);

    gtk_main();

    return EXIT_SUCCESS;
}