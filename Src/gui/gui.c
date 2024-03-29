#include "GUI/gui.h"

#define Blursize 3
#define BlurIntensity 1.5
#define AdaptiveThreshold 0.27
#define Splitsize 75000
#define DEFAULT_CELL_VALUE 0b0011001111111111


# define ROTATION_STEP                  0
# define GRAYSCALE_STEP                 1
# define GAUSSIAN_BLUR_STEP             2
# define ADAPTATIVE_THRESHOLDING_STEP   3
# define SOBEL_GRADIENT_STEP            4
# define HOUGH_TRANSFORM_STEP           5
# define CORRECTION_STEP                6
# define SPLIT_STEP                     7
# define NEURON_NETWORK_STEP            8
# define SOLVING_STEP                   9

typedef struct widgets {
    GtkBuilder* b;
    GtkWidget* w1;
    GtkWidget* w2;

    GtkImage* ImagePicking;
    GtkImage* ImageDisplay;

    GtkFileChooser* FilePicker;

    GtkButton* NextPageButton;
    GtkButton* DoAllButton;
    GtkButton* DoNextButton;
    GtkProgressBar* ProgressBar;
    GtkScale* Scale;
    char* path;
    int resetSlider;
    GtkEntry* forceInputs[81];

    GtkButton* OverrideWindowButton;
    GtkButton* backToSteps;
    GtkWindow* OverrideWindow;
} widgets;

int i = 0;
Quadrilateral* quad;//The quad used by FindAngle and returned by FindGrid
short digits[9][9];
short initialDigits[9][9];

Network *network;

// Writes 'digit' on the surface at (x, y)
void WriteDigit(SDL_Surface* s,
                int x,
                int y,
                int digit,
                int inRed)
{
    char* file;
    if (digit == 0)
        digit = 1;
    if (asprintf(&file, "templates/number%i.png", digit) == -1)
        errx(EXIT_FAILURE, "asprintf failed");
    SDL_Surface* inp = IMG_Load(file);
    if (inp == NULL) {
        printf("%s loading failed\n", file);
        return;
    }
    SDL_LockSurface(inp);
    SDL_LockSurface(s);
    Uint32* pixels = s->pixels;
    Uint32* inpPixels = inp->pixels;
    for (int j = 4; j < 36; j++)
        for (int k = 4; k < 36; k++) {
            Uint8 r, g, b;
            SDL_GetRGB(inpPixels[k + j * 39], inp->format, &r, &g, &b);
            pixels[x + k + (y + j) * 396] = SDL_MapRGB(s->format, r, g * (inRed ? 0 : 1),
                                            b * (inRed ? 0 : 1));
        }
    free(file);
    SDL_UnlockSurface(inp);
    SDL_UnlockSurface(s);
    SDL_FreeSurface(inp);
}

void print_image(Matrix *a)
{
    for (size_t i = 0; i < 28; ++i) {
        for (size_t j = 0; j < 28; ++j)
            if (a->values[0][i * 28 + j] > 0)
                //printf("%0.3f ", a->values[i * 28 + j][0]);
                printf("@ ");
            else
                //printf("%0.3f ", 0.f);
                printf(". ");
        printf("\n");
    }
}

void print_digits()
{
    for (size_t j = 0; j < 9; j++) {
        for (size_t i = 0; i < 9; i++)
            if (digits[i][j] >= 1 && digits[i][j] <= 9)
                printf("%i ", digits[i][j]);
            else
                printf(". ");
        printf("\n");
    }
}

gboolean Window3(GtkButton* btn, gpointer ptr)
{
    btn = btn;
    widgets* h = ptr;
    gtk_widget_show(GTK_WIDGET(h->OverrideWindow));
    for (int j = 0; j < 81; j++) {
        char text[2];
        if (digits[j % 9][j / 9] >= 1 && digits[j % 9][j / 9] <= 9) {
            text[0] = '0' + digits[j % 9][j / 9];
        } else {
            text[0] = ' ';
        }
        text[1] = '\0';
        gtk_entry_set_text(h->forceInputs[j], text);
    }
    print_digits();
    return FALSE;
}

gboolean Window2(GtkButton* btn, gpointer ptr)
{
    btn = btn;
    widgets* h = ptr;
    gtk_widget_hide(GTK_WIDGET(h->OverrideWindow));
    return FALSE;
}

gboolean ChangeInput(GtkEditable* self, gpointer ptr)
{
    widgets* h = ptr;
    size_t j = 1;
    for (; j <= 81; j++) {
        if (h->forceInputs[j - 1] == GTK_ENTRY(self)) {
            break;
        }
    }
    if (j == 82)
        return FALSE;
    printf("%zu\n", j);
    printf("----------------\n");
    char c = gtk_entry_get_text(GTK_ENTRY(self))[0];
    initialDigits[(j - 1) % 9][(j - 1) / 9] = 0;
    if (c >= '1' && c <= '9') {
        digits[(j - 1) % 9][(j - 1) / 9] = c - '0';
    } else if (c == ' ' || !c) {
        digits[(j - 1) % 9][(j - 1) / 9] = DEFAULT_CELL_VALUE;
    }
    print_digits();
    return FALSE;
}

gboolean DoNextFunc(GtkButton* btn, gpointer ptr)
{
    widgets* h = ptr;
    GtkImage* ImageDisplay = h->ImageDisplay;
    char* file;
    if (asprintf(&file, "temp%02i.png", i) == -1)
        errx(EXIT_FAILURE, "asprintf failed");
    SDL_Surface* img = IMG_Load(file);
    gtk_widget_hide(GTK_WIDGET(h->Scale));
    gtk_progress_bar_set_fraction(h->ProgressBar, (float)(i + 1) / 10);
    if (i == NEURON_NETWORK_STEP - 1 || i == NEURON_NETWORK_STEP + 1)
        gtk_widget_hide(GTK_WIDGET(h->OverrideWindowButton));
    SDL_Surface* img_for_split;
    switch (i) {
    case ROTATION_STEP:
        //img_for_split = IMG_Load(file);
        gtk_scale_set_digits(h->Scale, 0);
        gtk_widget_show(GTK_WIDGET(h->Scale));
        if (h->resetSlider) {
            gtk_range_set_range(GTK_RANGE(h->Scale), -180, 180);
            gtk_range_set_value(GTK_RANGE(h->Scale), 0);
        } else {
            float f = (float)(gtk_range_get_value(GTK_RANGE(h->Scale)));
            printf("%s\n", file);
            img = IMGA_Rotate(img, f);
            //img_for_split = IMGA_Rotate(img_for_split, f);
        }
        gtk_button_set_label(btn, "Next step (Grayscale)");
        break;
    case GRAYSCALE_STEP:
        img = IMGC_Grayscale(img);
        img = IMGC_Normalize_Brigthness(img);
        img = IMGC_Level_Colors(img,10);
        gtk_button_set_label(btn, "Next step (Gaussian blur)");
        break;
    case GAUSSIAN_BLUR_STEP:
        img = IMGA_GaussianBlur(img, Blursize, BlurIntensity);
        gtk_button_set_label(btn, "Next step (Adaptive thresholding)");
        break;
    case ADAPTATIVE_THRESHOLDING_STEP:
        gtk_widget_show(GTK_WIDGET(h->Scale));
        if (h->resetSlider) {
            gtk_range_set_value(GTK_RANGE(h->Scale), 0.27);
            gtk_scale_set_digits(h->Scale, 2);
            gtk_range_set_range(GTK_RANGE(h->Scale), 0.2, 0.5);
            //TODO set range between 0.2 and 0.5 properly
            img = IMGA_Sauvola(img, 10, 0.27);
        } else {
            double v = gtk_range_get_value(GTK_RANGE(h->Scale));
            printf("%f <--\n", v);
            img = IMGA_Sauvola(img, 10, v);
        }
        img = CheckInvert(img);
        img = IMGA_Erosion(IMGA_Erosion(img));
        gtk_button_set_label(btn, "Next step (Sobel gradient)");
        break;
    case SOBEL_GRADIENT_STEP:
        IMG_SavePNG(img, "temp_split05.png");
        img = sobel_gradient(img);
        gtk_button_set_label(btn, "Next step (Hough transform)");
        break;
    case HOUGH_TRANSFORM_STEP:
        quad = Find_Grid(&img);
        gtk_button_set_label(btn, "Next step (Correction)");
        break;
    case CORRECTION_STEP:
        if(!quad) return EXIT_FAILURE; //TODO ACTUAL ERROR
        int tempx = quad->p4.x;
        int tempy = quad->p4.y;
        quad->p4.x = quad->p3.x;
        quad->p4.y = quad->p3.y;
        quad->p3.x = tempx;
        quad->p3.y = tempy;
        Quadrilateral* copy = malloc(sizeof(Quadrilateral));
        copy->p1 = quad->p1;
        copy->p2 = quad->p2;
        copy->p3 = quad->p3;
        copy->p4 = quad->p4;
        img_for_split = CorrectImage(Padding(IMG_Load("temp04.png"), 10), copy);
        gtk_button_set_label(btn, "Next step (Split image)");
        IMG_SavePNG(img_for_split, "temp_for_split.png");
        SDL_FreeSurface(img);
        img = IMG_Load("temp_for_split.png");
        SDL_FreeSurface(img_for_split);
        free(copy);
        break;
    case SPLIT_STEP:
        mkdir("temp_split", S_IRWXU);
        img_for_split = IMG_Load("temp_for_split.png");
        Split(img_for_split, "temp_split");
        SDL_FreeSurface(img);
        img = SDL_CreateRGBSurface(0,28*9,28*9,32,0,0,0,0);
        Uint32* pixels = img->pixels;
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++) {
                char* name;
                if (asprintf(&name, "temp_split/split_%02i_28x28.png", j * 9 + k + 1) == -1)
                    errx(EXIT_FAILURE, "asprintf failed");
                SDL_Surface* su = IMG_Load(name);
                Uint32* suPixels = su->pixels;
                for (int jj = 0; jj < 28; jj++)
                    for (int kk = 0; kk < 28; kk++) {
                        pixels[(j*28 + jj) * 28*9 + k*28 + kk] = suPixels[jj * 28 + kk];
                    }
                SDL_FreeSurface(su);
            }
        gtk_button_set_label(btn, "Next step (Digit recognition)");
        SDL_FreeSurface(img_for_split);
        break;
    case NEURON_NETWORK_STEP:
        printf("Starting step 7");
        gtk_widget_show(GTK_WIDGET(h->OverrideWindowButton));
        Matrix *input = new_Matrix(1, 784);
        for (int j = 1; j <= 81; j++) {
            char* f;
            if (asprintf(&f, "temp_split/split_%02i_28x28.png", j) == -1)
                errx(EXIT_FAILURE, "asprintf failed");
            // put the pixels in a matrix
            SDL_Surface *s = IMG_Load(f);
            Uint32 *pixels = s->pixels;
            int margin = 0;
            for (int x = margin; x < 28 - margin; ++x)
                for (int y = margin; y < 28 - margin; ++y) {
                    Uint8 gray;
                    SDL_GetRGB(pixels[x * 28 + y], s->format, &gray, &gray, &gray);
                    input->values[0][x * 28 + y] = gray / 255.f;
                }
            // compute the output from the network
            Matrix *output = feedforward(network, input);
            // get maximum index of output
            short foundDigit = argmax_m(output);
            // if you want to verify how sure he is, check the index
            free_m(output);
            if (!foundDigit)
                foundDigit = DEFAULT_CELL_VALUE;
            else
                initialDigits[(j - 1) % 9][(j - 1) / 9] = 1;
            digits[(j - 1) % 9][(j - 1) / 9] = foundDigit;
            SDL_FreeSurface(s);
            free(f);
        }
        free_m(input);
        gtk_button_set_label(btn, "Next step (Solving)");
        break;
    case SOLVING_STEP:
        SLV_solve(digits);
        //digits is filled in place with values from the neural network
        SDL_Surface* new = IMG_Load("templates/grid.png");
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                if (digits[k][j] >= 1 && digits[k][j] <= 9) {
                    WriteDigit(new,
                               j * 44,//44 = 396 / 9 (396 = new->width)
                               k * 44,
                               digits[j][k],
                               initialDigits[j][k]);
                }
            }
        }
        SDL_FreeSurface(img);
        img = new;
        gtk_widget_hide(GTK_WIDGET(h->DoNextButton));
        gtk_widget_hide(GTK_WIDGET(h->DoAllButton));
        break;
    }
    i += 1;
    char* newFile;
    if (asprintf(&newFile, "temp%02i.png", i) == -1)
        errx(EXIT_FAILURE, "asprintf failed");
    IMG_SavePNG(img, newFile);
    if (asprintf(&newFile, "temp%02iUI.png", i) == -1)
        errx(EXIT_FAILURE, "asprintf failed");
    const int new_width = 500;
    int new_height = ceil((float)img->h * (float)new_width / img->w);
    img = downscale_resize(img, new_width, new_height);
    /*printf("Resized width from %ix%i to %ix%i\n",
            img->w, img->h, new_width, new_height);*/
    IMG_SavePNG(img, newFile);
    gtk_image_set_from_file(ImageDisplay, newFile);
    free(newFile);
    free(file);
    SDL_FreeSurface(img);
    return TRUE;
}

gboolean DoNextFuncHat(GtkButton* btn, gpointer ptr)
{
    widgets* h = ptr;
    h->resetSlider = 1;
    DoNextFunc(btn, ptr);
    return TRUE;
}

gboolean DoAllFunc(GtkButton* btn, gpointer ptr)
{
    btn = btn;
    widgets* h = ptr;
    for (; i <= SOLVING_STEP;) {
        DoNextFunc(h->NextPageButton, h);
        // since we are dealing with a heavy function,
        // refresh the ui after each step
        while (gtk_events_pending()) {
            gtk_main_iteration();
        }
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
    gtk_button_set_label(h->DoNextButton, "Next step (Rotation)");
    gtk_widget_show(GTK_WIDGET(h->DoNextButton));
    gtk_widget_show(GTK_WIDGET(h->DoAllButton));
    SDL_Surface *s = IMG_Load(h->path);
    int new_width = 1000;
    int new_height = ceil((float)s->h * (float)new_width / s->w);
    s = downscale_resize(s, new_width, new_height);
    printf("Saving\n");
    IMG_SavePNG(s, "temp00.png");
    new_width = 500;
    new_height = ceil((float)s->h * (float)new_width / s->w);
    s = downscale_resize(s, new_width, new_height);
    IMG_SavePNG(s, "temp00UI.png");
    SDL_FreeSurface(s);
    gtk_image_set_from_file(h->ImageDisplay, "temp00UI.png");
    gtk_progress_bar_set_fraction(h->ProgressBar, 0);
    return FALSE;
}

gboolean ShowImage(GtkFileChooser* file_picker, gpointer ptr)
{
    widgets* h = ptr;
    char* path = gtk_file_chooser_get_filename(file_picker);
    h->path = "temp00.png";
    SDL_Surface *s = IMG_Load(path);
    const int new_width = 500;
    int new_height = ceil((float)s->h * (float)new_width / s->w);
    s = downscale_resize(s, new_width, new_height);
    printf("Resized width from %ix%i to %ix%i\n",
           s->w, s->h, new_width, new_height);
    IMG_SavePNG(s, "temp00.png");
    SDL_FreeSurface(s);
    gtk_image_set_from_file(h->ImagePicking, "temp00.png");
    gtk_widget_show(GTK_WIDGET(h->NextPageButton));
    return TRUE;
}

gboolean Save()
{
    char* s;
    if (asprintf(&s, "temp%02i.png", i) == -1)
        errx(EXIT_FAILURE, "asprintf failed");
    IMG_SavePNG(IMG_Load(s), "output.png");
    free(s);
    printf("Saved into output.png\n");
    return FALSE;
}

void RefreshDisplay(widgets* h)
{
    if (i == 0) {
        //Can't go backwards to call DoNextFunc, so copy paste :/
        gtk_image_set_from_file(h->ImageDisplay, "temp00.png");
        gtk_button_set_label(h->DoNextButton, "Next step (Grayscale)");
        gtk_progress_bar_set_fraction(h->ProgressBar, 0);
    } else {
        i -= 1;
        DoNextFunc(h->DoNextButton, h);
        //DoNextFunc will handle the i increase, the image loading
        // and the button labels update
    }
}

gboolean Undo(GtkButton* btn, gpointer ptr)
{
    btn = btn;
    if (i == 0)
        return FALSE;
    widgets* h = ptr;
    i -= 1;
    if( i==SPLIT_STEP) {
        gtk_image_set_from_file(h->ImageDisplay,"temp07UI.png");
        gtk_button_set_label(h->DoNextButton, "Next step (Split image)");
        return FALSE;
    }
    gtk_widget_show(GTK_WIDGET(h->DoNextButton));
    gtk_widget_show(GTK_WIDGET(h->DoAllButton));
    h->resetSlider = 0;
    RefreshDisplay(h);
    return FALSE;
}

void RemoveTemps()
{
    remove("accumulator.png");
    for (int j = 0; j <= SOLVING_STEP; j++) {
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
    Free_Network(network);
    gtk_main_quit();
    return FALSE;
}

gboolean SliderAction(GtkRange* slider, gpointer user_data)
{
    widgets* h = user_data;
    gint v = gtk_range_get_value(slider);
    if (i - 1== ADAPTATIVE_THRESHOLDING_STEP) {
        //Thresholding
        SDL_Surface* s = IMG_Load("temp02.png");
        s = IMGA_ApplyThreshold(s, v, Splitsize);
        s = CheckInvert(s);
        s = IMGA_Erosion(s);
        IMG_SavePNG(s, "temp03.png");
        SDL_FreeSurface(s);
    } else if (i - 1 == ROTATION_STEP) {
        IMG_SavePNG(IMGA_Rotate(IMG_Load("temp00.png"), v), "temp01.png");
    } else
        return TRUE;
    h->resetSlider = 0;
    RefreshDisplay(h);
    return TRUE;
}

// Main function.
int main ()
{
    network = Load_Network("../neural/networko");
    // Initializes GTK.
    gtk_init(NULL, NULL);
    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return EXIT_FAILURE;
    }
    GtkScale* scale = GTK_SCALE(
                          gtk_builder_get_object(builder, "Scale"));
    gtk_range_set_range(GTK_RANGE(scale), 1, 6);
    // Gets the widgets.
    widgets h = {
b :
        builder,
w1 :
        GTK_WIDGET(
            gtk_builder_get_object(builder, "StartWindow")),

w2 :
        GTK_WIDGET(
            gtk_builder_get_object(builder, "gui")),

ImagePicking :
        GTK_IMAGE(
            gtk_builder_get_object(builder, "ImagePicking")),

ImageDisplay :
        GTK_IMAGE(
            gtk_builder_get_object(builder, "ImageDisplay")),

FilePicker :
        GTK_FILE_CHOOSER(
            gtk_builder_get_object(builder, "FilePicker")),

NextPageButton :
        GTK_BUTTON(
            gtk_builder_get_object(builder, "ApplyOnImage")),

DoAllButton :
        GTK_BUTTON(
            gtk_builder_get_object(builder, "DoAll")),

DoNextButton :
        GTK_BUTTON(
            gtk_builder_get_object(builder, "DoNext")),

ProgressBar :
        GTK_PROGRESS_BAR(
            gtk_builder_get_object(builder, "ProgressBar")),
Scale :
        scale,

path :
        NULL,
        resetSlider : 1,

OverrideWindowButton :
        GTK_BUTTON(gtk_builder_get_object(builder, "OverrideWindowButton")),
backToSteps :
        GTK_BUTTON(gtk_builder_get_object(builder, "backToSteps")),
OverrideWindow :
        GTK_WINDOW(gtk_builder_get_object(builder, "OverrideWindow")),
    };
    g_signal_connect(h.backToSteps, "clicked", G_CALLBACK(Window2), &h);
    gtk_widget_hide(GTK_WIDGET(h.OverrideWindowButton));
    for (int i = 0; i < 81; i++) {
        char* s;
        if (asprintf(&s, "input%i", i + 1) == -1)
            errx(EXIT_FAILURE, "asprintf failed");
        h.forceInputs[i] = GTK_ENTRY(gtk_builder_get_object(builder, s));
        g_signal_connect(h.forceInputs[i], "changed", G_CALLBACK(ChangeInput), &h);
        free(s);
    }
    g_signal_connect(h.OverrideWindowButton, "clicked", G_CALLBACK(Window3), &h);
    gtk_widget_hide(GTK_WIDGET(scale));
    g_signal_connect(h.Scale, "value-changed", G_CALLBACK(SliderAction), &h);
    gtk_widget_hide(GTK_WIDGET(h.NextPageButton));
    GtkButton* ApplyOnImage =
        GTK_BUTTON(gtk_builder_get_object(builder, "ApplyOnImage"));
    g_signal_connect(ApplyOnImage, "clicked", G_CALLBACK(ChangeWindow), &h);
    GtkButton* DoNext = GTK_BUTTON(gtk_builder_get_object(builder, "DoNext"));
    GtkButton* DoAll = GTK_BUTTON(gtk_builder_get_object(builder, "DoAll"));
    // Connects signal handlers.
    g_signal_connect(h.w1, "destroy", G_CALLBACK(MyQuit), NULL);
    g_signal_connect(h.w2, "destroy", G_CALLBACK(MyQuit), NULL);
    g_signal_connect(h.FilePicker, "file-set", G_CALLBACK(ShowImage), &h);
    g_signal_connect(
        GTK_BUTTON(gtk_builder_get_object(builder, "CancelButton")),
        "clicked",
        G_CALLBACK(GoBack),
        &h
    );
    g_signal_connect(
        GTK_BUTTON(gtk_builder_get_object(builder, "SaveButton")),
        "clicked",
        G_CALLBACK(Save),
        &h
    );
    g_signal_connect(
        GTK_BUTTON(gtk_builder_get_object(builder, "Undo")),
        "clicked",
        G_CALLBACK(Undo),
        &h
    );
    g_signal_connect(DoNext, "clicked", G_CALLBACK(DoNextFuncHat), &h);
    g_signal_connect(DoAll, "clicked", G_CALLBACK(DoAllFunc), &h);
    gtk_main();
    return EXIT_SUCCESS;
}
