#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <function>
#include <exception>
#include <boost/lockfree/spsc_queue>
#include <boost/program_options>
#include <portaudio.h>
#include <gtk/gtk.h>
namespace po = boost::program_options;

/* options:
api:
device:
buffer size: in frames
sample rate:
bit depth:
fragment size: in frames
dither:

*/

/* program options */
int sample_rate;
int bit_depth;
int channels;
enum {} api;

/* a frame is
typedef int framecount_t;
framecount_t buffer_size;
framecount_t fragment_size;

typedef int milliseconds_t;
milliseconds_t update_ui_rate;

/* */

int framesize;

std::list<std::string> song_list;

boost::spsc_queue<char> buffer;

Pa_Stream *stream;

GtkWidget *window;

int main(int argc, char **argv) {
	init_log();
	gtk_init(&argc, &argv);
	init_options();
	init_gui();
	init_portaudio();

	close_portaudio();
	close_log();
	exit(EXIT_SUCCESS);
}

void init_log(void) {
	clog << "\n";
	clog << "ffmpeg version: " << "\n";
	clog << Pa_GetVersionText();
	clog << std::endl;
}

void init_options(void) {
	po::options_description desc("Allowed options");

	desc.add_options()
		("help", "produce help message")
		("sample rate", po::value<int>(), "Set output sample rate")
		("bit depth", po::value<std::string>(), "Set output bit depth")
		("api", po::value<std::string>(), "API to use. It can be alsa, pulseaudio. Empty string uses default.")
		("device", po::value<std::string>(), "Device. It can be alsa, pulseaudio. Empty string uses default.")
;

}

void init_gui(void) {
	GtkBuilder *builder;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "gui.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show(window);
}

void init_portaudio(void) {
	pa_Error ret =
	Pa_Inititalize();
	if (ret != pa_NoError) {
		cerr << "Portaudio error: " << Pa_GetErrorText(ret) << "\n";
		exit(EXIT_FAILURE);
	}

	ret =
	if (ret != pa_NoError) {
		cerr << "Portaudio error: " << Pa_GetErrorText(ret) << "\n";
		exit(EXIT_FAILURE);
	}
	clog << "founded devices:\n";

	PaStreamParameters outputparam;
	outputparam
	ret =
	Pa_OpenStream(&stream, , , (double) sample_rate, paFramesPerBufferUnspecified, , , NULL);
	if (ret != pa_NoError) {
		cerr << "Portaudio error: " << Pa_GetErrorText(ret) << "\n";
		exit(EXIT_FAILURE);
	}
	clog << "opened device " << "" << "with following parameters:\n";
	clog << " - sample rate: " << sample_rate << " hz." << "\n";
	clog << " - bit depth: " << bit_depth << " bit." << "\n";
	clog << " - channels: " << channels << "." << "\n";
	clog << std::endl;
}

void process(void* out, framecount_t framecount) {

}

void callback(void) {
}

void update_ui(void) {

	wait(update_ui_rate);
}

void close_portaudio(void) {
	Pa_Terminate();
}

void close_log(void) {
	clog << "bye!" << std::endl;
}
